#include "Parser.h"
#include <iostream>
#include <list>


/**
 * ‘transformation’ <simpleName> ‘(‘ <modelDecl>‘,’ <modelDecl> ‘)’ ‘{‘ (<relation> | <query>)* ‘}’
 */
void Parser::Transformation() 
{
	accept(LexicalAtom::transKw, last_open_node, 1);
	accept(LexicalAtom::simpleName, last_open_node, 1);
	accept(LexicalAtom::lparent, last_open_node, 1); 
	modelDecl(last_open_node, 1); 
	accept(LexicalAtom::comma, last_open_node, 1); 
	modelDecl(last_open_node, 1);
	accept(LexicalAtom::rparent, last_open_node, 1); 
	accept(LexicalAtom::lbracket, last_open_node, 1);

	std::list<LexicalAtom> *skip_list = new std::list<LexicalAtom>(); 
	skip_list->push_back(LexicalAtom::querKw); 
	skip_list->push_back(LexicalAtom::topKw); 
	skip_list->push_back(LexicalAtom::relKw); 

	while (relation(last_open_node, skip_list, 0) || query(last_open_node, skip_list, 0)) {};

	accept(LexicalAtom::rbracket, last_open_node, 1);
	parse_tree->print();
	delete skip_list;
}


/**
 * <OclExpressionPart2> ::=
	  ‘:’ <pathName> ‘{‘ [<propertyTemplateList>] ‘}’
	| ‘/’ (<variable> | <realExpr> | <RealLit>) <realExpr2>
 */
bool Parser::OclExpressionPart2(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (accept(LexicalAtom::colon, n2, mode))
	{
		pathName(n2, 1);
		accept(LexicalAtom::lbracket, n2, 1);
		propertyTemplateList(n2, 0);
		accept(LexicalAtom::rbracket, n2, 1);
	}
	else 
		if (accept(LexicalAtom::divOp, n2, mode))
		{
			if (!variable(n2, 0))
				if (!realExpr(n2, 0))
					accept(LexicalAtom::RealLit, n2, 1);
			realExpr2(n2, 1);
		}
		else 
		{
			delete n2; 
			return false;
		}
	n->addChild(n2); 
	return true;
}


/**
 * <propertyTemplateList> ::= <propertyTemplate> (‘,’ <propertyTemplate>)*
 */
bool Parser::propertyTemplateList(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!propertyTemplateList(n2, mode))
	{
		delete n2; 
		return false;
	}
	while (accept(LexicalAtom::comma, n2, 0))
		propertyTemplateList(n2, 1);
	n->addChild(n2); 
	return true;
}


/**
 * <variable2> ::= <pathName2> ‘.’ <simpleName> | Ɛ
 */
bool Parser::variable2(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol);
	if (!pathName2(n2, 0)) 
	{
		n->addChild(n2); //Ɛ
		return true;
	}
	accept(LexicalAtom::dot, n2, 1);
	accept(LexicalAtom::simpleName, n2, 1);
	n->addChild(n2); 
	return true;
}


/**
 * <variable> ::= <simpleName> <variable2> 
 */
bool Parser::variable(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::simpleName, n2, mode))
	{
		delete n2; 
		return false;
	}
	variable2(n2, 1);
	n->addChild(n2); 
	return true;
}


/**
 * <objectTemplate> ::= <simpleName> <variable2> ‘:’ <pathName> ‘{‘ [<propertyTemplateList>] ‘}’ 
 */
bool Parser::objectTemplate(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::simpleName, n2, mode))
	{
		delete n2; //no tragedy
		return false;
	}
	variable2(n2, 1);
	accept(LexicalAtom::colon, n2, 1);
	pathName(n2, 1);
	accept(LexicalAtom::lbracket, n2, 1);
	propertyTemplate(n2, 0);
	accept(LexicalAtom::rbracket, n2, 1);
	n->addChild(n2); 
	return true;
}


/**
 * <relExpr> ::= <OclExpression> <relOp> <OclExpression> 
 */
bool Parser::relExpr(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!OclExpression(n2, mode))
	{
		delete n2; //no tragedy
		return false;
	}
	relOp(n2, 1);
	OclExpression(n2, 1);
	n->addChild(n2); 
	return true;
}


/**
 * <boolExpr2> ::= <boolOp> (<relExpr> | <boolExpr>) <boolExpr2> | Ɛ
 */
bool Parser::boolExpr2(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (boolOp(n2, 0))
	{
		if (!relExpr(n2, 0))
			boolExpr(n2, 1);
		boolExpr2(n2, 1);
	}
	n->addChild(n2); //Ɛ
	return true;
}


/**
 * <boolExpr> ::= <OclExpression> <relOp> <OclExpression> <boolOp> (<relExpr> | <boolExpr>) <boolExpr2>
			| <BooleanLit> 
 */
bool Parser::boolExpr(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (OclExpression(n2, mode))
	{
		relOp(n2, 1);
		OclExpression(n2, 1);
		boolOp(n2, 1);
		if (!relExpr(n2, 0))
			boolExpr(n2, 1);
		boolExpr2(n2, 1);
	}
	else
		if (!BooleanLit(n2, mode))
		{
			delete n2; 
			return false;
		}
	n->addChild(n2);
	return true;
}  


/**
 * <realExpr2> ::= <realOp> (<variable> | <realExpr> | <RealLit>) <realExpr2> | Ɛ
 */
bool Parser::realExpr2(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (realOp(n2, 0))
	{
		if (!accept(LexicalAtom::RealLit, n2, 0))
			if (!variable(n2, 0))
				realExpr(n2, 1);
		realExpr2(n2, 1);
	}
	n->addChild(n2); //Ɛ
	return true;
}


/**
 * <realExpr> ::= <simpleName> <variable2> <realOp> (<variable> | <realExpr> | <RealLit>) <realExpr2>
			| <RealLit> <realOp> (<variable> | <realExpr> | <RealLit>) <realExpr2>
 */
bool Parser::realExpr(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (accept(LexicalAtom::RealLit, n2, mode))
	{
		realOp(n2, 1);
		if (!realExpr(n2, 0))
			if (!accept(LexicalAtom::RealLit, n2, 0))
				variable(n2, 1);
		realExpr2(n2, 1);
	}
	else
		if (accept(LexicalAtom::simpleName, n2, mode))
		{
			variable2(n2, 1);
			realOp(n2, 1);
			if (!realExpr(n2, 0))
				if (!accept(LexicalAtom::RealLit, n2, 0))
					variable(n2, 1);
			realExpr2(n2, 1);
		}
		else
		{
			delete n2; 
			return false;
		}
	n->addChild(n2);
	return true;
}  


/**
 * <intExpr2> ::= <arytmOp> (<variable> | <intExpr> | <IntegerLit>) <intExpr2> | Ɛ
 */
bool Parser::intExpr2(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (arytmOp(n2, 0))
	{
		if (!accept(LexicalAtom::IntegerLit, n2, 0))
			if (!variable(n2, 0))
				intExpr(n2, 1);
		intExpr2(n2, 1);
	}
	n->addChild(n2); //Ɛ
	return true;
}


/**
 * <intExpr> ::= <IntegerLit> <arytmOp> (<variable> | <intExpr> | <IntegerLit>) <intExpr2>
			| <simpleName> <variable2> <arytmOp> (<variable> | <intExpr> | <IntegerLit>) <intExpr2>
 */
bool Parser::intExpr(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (accept(LexicalAtom::IntegerLit, n2, mode))
	{
		arytmOp(n2, 1);
		if (!intExpr(n2, 0))
			if (!accept(LexicalAtom::IntegerLit, n2, 0))
				variable(n2, 1);
		intExpr2(n2, 1);
	}
	else
		if (accept(LexicalAtom::simpleName, n2, mode))
		{
			variable2(n2, 1);
			arytmOp(n2, 1);
			if (!intExpr(n2, 0))
				if (!accept(LexicalAtom::IntegerLit, n2, 0))
					variable(n2, 1);
			intExpr2(n2, 1);
		}
		else
		{
			delete n2; 
			return false;
		}
	n->addChild(n2);
	return true;
}


/**
 * <concatExpr2> ::= ‘+’ (<concatExpr> | <StringLit> | <variable>) <concatExpr2> | Ɛ
 */
bool Parser::concatExpr2(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (accept(LexicalAtom::plusOp, n2, 0))
	{
		if (!accept(LexicalAtom::StringLit, n2, 0))
			if (!variable(n2, 0))
				concatExpr(n2, 1);
		concatExpr2(n2, 1);
	}
	n->addChild(n2); //Ɛ
	return true;
}


/**
 * <concatExpr> ::= <StringLit> ‘+’ (<concatExpr> | <StringLit> | <variable>)<concatExpr2>
				| <simpleName> <variable2> ‘+’ (<concatExpr> | <StringLit> | <variable>) <concatExpr2>
 */
bool Parser::concatExpr(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (accept(LexicalAtom::StringLit, n2, mode))
	{
		accept(LexicalAtom::plusOp, n2, 1);
		if (!concatExpr(n2, 0))
			if (!accept(LexicalAtom::StringLit, n2, 0))
				variable(n2, 1);
		concatExpr2(n2, 1);
	}
	else
		if (accept(LexicalAtom::simpleName, n2, mode))
		{
			variable2(n2, 1);
			accept(LexicalAtom::plusOp, n2, 1);
			if (!concatExpr(n2, 0))
				if (!accept(LexicalAtom::StringLit, n2, 0))
					variable(n2, 1);
			concatExpr2(n2, 1);
		}
		else
		{
			delete n2; 
			return false;
		}
	n->addChild(n2);
	return true;
}


/**
 * <collectionType> ::= <collectionTypeIdentifier> ‘(‘ <type> ‘)’
 */
bool Parser::collectionType(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!collectionTypeIdentifier(n2, mode))
	{
		delete n2; 
		return false;
	}
	accept(LexicalAtom::lparent, n2, 1);
	type(n2, 1);
	accept(LexicalAtom::rparent, n2, 1);
	n->addChild(n2);
	return true;
}


/**
 * <type> ::= (<simpleName> <pathName2>) | <collectionType> | <primitiveType> 
 */
bool Parser::type(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (accept(LexicalAtom::simpleName, n2, mode))
		pathName2(n2, 1);
	else 
		if (!collectionType(n2, mode))
			if (!primitiveType(n2, mode))
			{
				delete n2; 
				return false;
			}
	n->addChild(n2);
	return true;
}


/**
 * <OclExpressionPart3> ::=
		  ‘.’ <simpleName> <OclExpressionPart2>
		| Ɛ
 */
bool Parser::OclExpressionPart3(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (accept(LexicalAtom::dot, n2, 0))
	{
		accept(LexicalAtom::simpleName, n2, 1);
		OclExpressionPart2(n2, 1);
	}
	n->addChild(n2); //Ɛ
	return true;
}


/**
 * <OclExpressionPart1> ::=
	  	  <pathName2> <OclExpressionPart3>
		| <OclExpressionPart2>
		| ‘(‘ <variable> (‘,’ <variable>)* ‘)’
 */
bool Parser::OclExpressionPart1(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (pathName2(n2, mode))
		OclExpressionPart3(n2, 1);
	else
		if (accept(LexicalAtom::lparent, n2, mode))
		{
			variable(n2, 1);
			while (accept(LexicalAtom::comma, n2, 0))
				variable(n2, 1);
			accept(LexicalAtom::rparent, n2, 1);
		}
		else
			if (!OclExpressionPart2(n2, mode))
			{
				delete n2; 
				return false;
			}
	n->addChild(n2); 
	return true;
}


/**
 * <OclExpression1> ::= <arytmOp> (<variable> | <intExpr> | <IntegerLit>) <intExpr2> | Ɛ
 */
bool Parser::OclExpression1(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol);
	if (arytmOp(n2, 0)) 
	{
		if (!intExpr(n2, 0))
			if (!accept(LexicalAtom::IntegerLit, n2, 0))
				variable(n2, 1);
		intExpr2(n2, 1);
	}
	n->addChild(n2); // Ɛ
	return true;
}


/**
 * <OclExpression3> ::= <realOp> (<variable> | <realExpr> | <RealLit>) <realExpr2> | Ɛ
 */
bool Parser::OclExpression3(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol);
	if (realOp(n2, 0)) 
	{
		if (!realExpr(n2, 0))
			if (!accept(LexicalAtom::RealLit, n2, 0))
				variable(n2, 1);
		realExpr2(n2, 1);
	}
	n->addChild(n2); // Ɛ
	return true;
}


/**
 * <OclExpression2> ::= ‘+’ (<concatExpr> | <StringLit> | <variable>) <concatExpr2> | Ɛ
 */
bool Parser::OclExpression2(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol);
	if (accept(LexicalAtom::plusOp, n2, 0)) 
	{
		if (!concatExpr(n2, 0))
			if (!accept(LexicalAtom::StringLit, n2, 0))
				variable(n2, 1);
		concatExpr2(n2, 1);
	}
	n->addChild(n2); // Ɛ
	return true;
}


/**
 * <OclExpressionPrim2> ::= 
	  <relOp> <OclExpression> <OclExpressionPrim2>
	| <boolOp> (<relExpr> | <boolExpr>) <boolExpr2> <OclExpressionPrim2>
	| Ɛ
 */
bool Parser::OclExpressionPrim2(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol);
	if (relOp(n2, 0)) 
	{
		OclExpression(n2, 1);
		OclExpressionPrim2(n2, 1);
	}
	else 
		if (boolOp(n2, 0))
		{
			if (!relExpr(n2, 0))
				boolExpr(n2, 1);
			boolExpr2(n2, 1);
			OclExpressionPrim(n2, 1);
		}
	n->addChild(n2); // Ɛ
	return true;
}


/**
 * <OclExpressionPrim> ::=
	  <relOp> <OclExpression> <OclExpressionPrim2>
	| Ɛ
 */
bool Parser::OclExpressionPrim(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol);
	if (!relOp(n2, 0)) 
	{
		n->addChild(n2); //Ɛ
		return true;
	}
	OclExpression(n2, 1);
	OclExpressionPrim2(n2, 1);
	n->addChild(n2); 
	return true;
}


/**
 * <OclExpression> ::= 
 	| <simpleName> <OclExpressionPart1> <OclExpressionPrim>
	| <IfExp> <OclExpressionPrim>
	| <BooleanLit> <OclExpressionPrim>
	| <nullKw> <OclExpressionPrim>
	| <StringLit> <OclExpression2> <OclExpressionPrim>
	| <RealLit> <OclExpression3> <OclExpressionPrim>
	| <IntegerLit> <OclExpression1> <OclExpressionPrim>
 */
bool Parser::OclExpression(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol);
	if (accept(LexicalAtom::simpleName, n2, mode)) 
	{
		OclExpressionPart1(n2, 1);
		OclExpressionPrim(n2, 1);
		n->addChild(n2); 
		return true;
	}
	if (IfExp(n2, mode)) 
	{
		OclExpressionPrim(n2, 1);
		n->addChild(n2); 
		return true;
	}
	if (BooleanLit(n2, mode))  
	{
		OclExpressionPrim(n2, 1);
		n->addChild(n2); 
		return true;
	}
	if (accept(LexicalAtom::nullKw, n2, mode))
	{
		OclExpressionPrim(n2, 1);
		n->addChild(n2); 
		return true;
	}
	if (accept(LexicalAtom::StringLit, n2, mode)) 
	{
		OclExpression2(n2, 1);
		OclExpressionPrim(n2, 1);
		n->addChild(n2); 
		return true;
	}
	if (accept(LexicalAtom::RealLit, n2, mode)) 
	{
		OclExpression3(n2, 1);
		OclExpressionPrim(n2, 1);
		n->addChild(n2); 
		return true;
	}
	if (accept(LexicalAtom::IntegerLit, n2, mode))
	{
		OclExpression1(n2, 1);
		OclExpressionPrim(n2, 1);
		n->addChild(n2); 
		return true;
	}
	delete n2;
	return false;
}


/**
 * <pathName2> ::= ‘::’ <simpleName> <pathName2> | Ɛ
 */
bool Parser::pathName2(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol);
	if (!accept(LexicalAtom::namspOp, n2, 0)) 
	{
		n->addChild(n2); //Ɛ
		return true;
	}
	accept(LexicalAtom::simpleName, n2, 1);
	pathName2(n2, 1);
	n->addChild(n2); 
	return true;
}


/**
 * <pathName> ::= <simpleName> <pathName2>
 */
bool Parser::pathName(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::simpleName, n2, mode))
	{
		delete n2; 
		return false;
	}
	pathName2(n2, 1);
	n->addChild(n2); 
	return true;
}


/**
 * <para> ::= <simpleName> ‘:’ <type> 
 */
bool Parser::para(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::simpleName, n2, mode))
	{
		delete n2; 
		return false;
	}
	accept(LexicalAtom::colon, n2, 1);
	type(n2, 1);
	n->addChild(n2); 
	return true;
}


/**
 * <propertyTemplate> ::= <simpleName> ‘=’ <OclExpression> 
 */
bool Parser::propertyTemplate(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::simpleName, n2, mode))
	{
		delete n2; 
		return false;
	}
	accept(LexicalAtom::eqOp, n2, 1);
	OclExpression(n2, 1);
	n->addChild(n2); 
	return true;
}


/**
 * <call> ::= <simpleName> ‘(‘ [<variable> (‘,’ <variable>)*] ‘)’
 */
bool Parser::call(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::simpleName, n2, mode))
	{
		delete n2;
		return false;
	}
	accept(LexicalAtom::lparent, n2, 1);
	if (variable(n2, 0))
		while (accept(LexicalAtom::comma, n2, 0))
			variable(n2, 1);
	accept(LexicalAtom::rparent, n2, 1);
	n->addChild(n2);
	return true;
}


/**
 * <varDeclaration> ::= <simpleName> (',' <simpleName>)* ‘:’ <type> [‘=’ <OclExpression>] ‘;’
 */
bool Parser::varDeclaration(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (accept(LexicalAtom::simpleName, n2, mode))
		while (accept(LexicalAtom::comma, n2, 0))
			accept(LexicalAtom::simpleName, n2, 1);
	else
	{
		delete n2;
		return false;
	}
	accept(LexicalAtom::colon, n2, 1);
	type(n2, 1);
	if (accept(LexicalAtom::eqOp, n2, 0)) 
		OclExpression(n2, 1);
	accept(LexicalAtom::semicol, n2, 1);
	n->addChild(n2);
	return true;
}


/**
 * <modelDecl> ::= <simpleName> ‘:’ <simpleName>
 */
bool Parser::modelDecl(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); //start new small tree
	if (!accept(LexicalAtom::simpleName, n2, mode))
	{
		delete n2; 
		return false;
	}
	accept(LexicalAtom::colon, n2, 1);
	accept(LexicalAtom::simpleName, n2, 1);
	n->addChild(n2);
	return true;
}


/**
 * <domain> ::= (‘checkonly’ | ‘enforce’) ‘domain’ <simpleName> ':' <type> ‘;’
 */
bool Parser::domain(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::domCKw, n2, mode))
		if (!accept(LexicalAtom::domEKw, n2, mode))
		{
			delete n2; 
			return false;
		}
	accept(LexicalAtom::domKw, n2, 1);
	accept(LexicalAtom::simpleName, n2, 1);
	
	accept(LexicalAtom::colon, n2, 1);
	type(n2, 1);
	accept(LexicalAtom::semicol, n2, 1);
	n->addChild(n2); 
	return true;
}


/**
 * <IfExp> ::= ‘if’ ‘(‘ <OclExpression> ‘)’ ‘then’ <OclExpression> [‘else’ <OclExpression>] ‘endif’
 */
bool Parser::IfExp(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); //start new small tree
	if (!accept(LexicalAtom::ifKw, n2, mode))
	{
		delete n2; //in case of ORs higher up, we don't want many empty nodes attached. 
		return false; //on the other hand, if we do have SOMETHING, we may as well attach it. 
					//and reprot the error from here (from inside this function)
	}
	accept(LexicalAtom::lparent, n2, 1); //if there is any trouble at some point, report it and return false,
	OclExpression(n2, 1);
	accept(LexicalAtom::rparent, n2, 1); //maybe (?) try to parse it further
	accept(LexicalAtom::thenKw, n2, 1);
	OclExpression(n2, 1);
	if (accept(LexicalAtom::elseKw, n2, 0))
		OclExpression(n2, 1);
	accept(LexicalAtom::endifKw, n2, 1);
	n->addChild(n2); 
	return true;
}


/**
 * <primitiveTypeDomain> := ‘primitive’ ‘domain’ <simpleName> ‘:’ <type> ‘;’
 */
bool Parser::primitiveTypeDomain(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol);
	if (!accept(LexicalAtom::domPKw, n2, mode))
	{
		delete n2; 
		return false;
	}
	accept(LexicalAtom::domKw, n2, 1);
	accept(LexicalAtom::simpleName, n2, 1);
	accept(LexicalAtom::colon, n2, 1);
	type(n2, 1);
	accept(LexicalAtom::semicol, n2, 1);
	n->addChild(n2); 
	return true;
}


/**
 * <when> ::= ‘when’ ‘{‘ (<OclExpression> ‘;’)* ‘}’
 */
bool Parser::when(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::whenKw, n2, mode))
	{
		delete n2;
		return false;
	} 
	accept(LexicalAtom::lbracket, n2, 1);
	while (OclExpression(n2, 0))
		accept(LexicalAtom::semicol, n2, 1);
	accept(LexicalAtom::rbracket, n2, 1);
	n->addChild(n2); 
	return true;
}


/**
 * <where> ::= ‘where’ ‘{‘ (<OclExpression> ‘;’)* ‘}’
 */
bool Parser::where(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::whereKw, n2, mode))
	{
		delete n2; 
		return false;
	}
	accept(LexicalAtom::lbracket, n2, 1);
	while (OclExpression(n2, 0))
		accept(LexicalAtom::semicol, n2, 1);

	accept(LexicalAtom::rbracket, n2, 1);
	n->addChild(n2); 
	return true;
}


/**
 * <relation> ::= [‘top’] ‘relation’ <simpleName> 
 * ‘{‘ <varDeclaration>* (<domain> | <prmitiveTypeDomain>)+ [<when>] [<where>] ‘}’
 */
bool Parser::relation(Node *n, std::list<LexicalAtom> *skip_list, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 

	bool acc = accept(LexicalAtom::topKw, n2, 0);
	bool rel = accept(LexicalAtom::relKw, n2, mode);
	if (!acc && !rel)
	{
		delete n2; 
		return false;
	}
	n->addChild(n2); 
	if (!rel)
	{
		skipToOneOfThese(skip_list);
		return false;
	}
	if (!accept(LexicalAtom::simpleName, n2, 1))
	{
		skipToOneOfThese(skip_list);
		return false;
	}
	if (accept(LexicalAtom::lbracket, n2, 1))
	{
		skipToOneOfThese(skip_list);
		return false;
	}
	while (varDeclaration(n2, 0)) {};
	if (!domain(n2, 1) && !primitiveTypeDomain(n2, 0)) // ?
	{
		skipToOneOfThese(skip_list);
		return false;
	}
	while (domain(n2, 0) || primitiveTypeDomain(n2, 0)) {};
	when(n2, 0);
	where(n2, 0);
	if (accept(LexicalAtom::rbracket, n2, 1))
	{
		skipToOneOfThese(skip_list);
		return false;
	}
	return true;
}


/**
 * <query> ::= ‘query’ <simpleName> ‘(‘ [<para> (‘,’ <para>)*] ‘)’‘:’ <type> ‘{‘ (<OclExpression> ‘;’)* ‘}’
 */
bool Parser::query(Node *n, std::list<LexicalAtom> *skip_list, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::querKw, n2, mode))
	{
		delete n2; 
		return false;
	}
	accept(LexicalAtom::simpleName, n2, 1);
	accept(LexicalAtom::lparent, n2, 1); 
	
	if (para(n2, 0))
		while (accept(LexicalAtom::comma, n2, 0))
			para(n2, 1);

	accept(LexicalAtom::rparent, n2, 1); 
	accept(LexicalAtom::colon, n2, 1);
	type(n2, 1);
	accept(LexicalAtom::lbracket, n2, 1);
	
	while (OclExpression(n2, 0))
		accept(LexicalAtom::semicol, n2, 1);

	accept(LexicalAtom::rbracket, n2, 1);
	n->addChild(n2); 
	return true;
}



bool Parser::relOp(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::eqOp, n2, mode))
		if (!accept(LexicalAtom::ltOp, n2, mode))
			if (!accept(LexicalAtom::gtOp, n2, mode))
				if (!accept(LexicalAtom::leOp, n2, mode))
					if (!accept(LexicalAtom::notOp, n2, mode))
						if (!accept(LexicalAtom::geOp, n2, mode))
						{
							delete n2;
							return false;
						}
	n->addChild(n2); 
	return true;
}


bool Parser::boolOp(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::andOp, n2, mode))
		if (!accept(LexicalAtom::orOp, n2, mode))
		{
			delete n2;
			return false;
		}
	n->addChild(n2); 
	return true;
}


bool Parser::arytmOp(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::plusOp, n2, mode))
		if (!accept(LexicalAtom::minusOp, n2, mode))
			if (!accept(LexicalAtom::multOp, n2, mode))
			{
				delete n2;
				return false;
			}
	n->addChild(n2); 
	return true;
}


bool Parser::realOp(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::plusOp, n2, mode))
		if (!accept(LexicalAtom::minusOp, n2, mode))
			if (!accept(LexicalAtom::multOp, n2, mode))
				if (!accept(LexicalAtom::divOp, n2, mode))
				{
					delete n2;
					return false;
				}
	n->addChild(n2); 
	return true;
}


bool Parser::collectionTypeIdentifier(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::setKw, n2, mode))
		if (!accept(LexicalAtom::bagKw, n2, mode))
			if (!accept(LexicalAtom::seqKw, n2, mode))
				if (!accept(LexicalAtom::collKw, n2, mode))
					if (!accept(LexicalAtom::ordsetKw, n2, mode))
					{
						delete n2;
						return false;
					}
	n->addChild(n2); 
	return true;
}


bool Parser::primitiveType(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::boolKw, n2, mode))
		if (!accept(LexicalAtom::intKw, n2, mode))
			if (!accept(LexicalAtom::realKw, n2, mode))
				if (!accept(LexicalAtom::strKw, n2, mode))
				{
					delete n2;
					return false;
				}
	n->addChild(n2); 
	return true;

	  
		  
		
}


bool Parser::BooleanLit(Node *n, int mode)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::falseKw, n2, mode))
		if (!accept(LexicalAtom::trueKw, n2, mode))
		{
			delete n2;
			return false;
		}
	n->addChild(n2); 
	return true;
}