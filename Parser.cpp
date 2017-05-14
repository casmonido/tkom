#include "Parser.h"
#include <iostream>


/**
 * ‘transformation’ <simpleName> ‘(‘ <modelDecl>‘,’ <modelDecl> ‘)’ ‘{‘ (<relation> | <query>)* ‘}’
 */
void Parser::Transformation() 
{
	//std::list<LexicalAtom> *skip_symbols = new std::list<LexicalAtom>();
	//skip_symbols->push_back(LexicalAtom::simpleName);
	accept(LexicalAtom::transKw, last_open_node);

	//skip_symbols->clear(); skip_symbols->push_back(LexicalAtom::lparent);
	accept(LexicalAtom::simpleName, last_open_node);

	//skip_symbols->clear(); skip_symbols->push_back(LexicalAtom::simpleName);
	accept(LexicalAtom::lparent, last_open_node); 

	modelDecl(last_open_node); 
	
	accept(LexicalAtom::comma, last_open_node); //same skip_symbols

	modelDecl(last_open_node);

	//skip_symbols->clear(); skip_symbols->push_back(LexicalAtom::simpleName);
	accept(LexicalAtom::rparent, last_open_node); 

	accept(LexicalAtom::lbracket, last_open_node); 

	while (relation(last_open_node) == true || query(last_open_node) == true)
	{}
	accept(LexicalAtom::rbracket, last_open_node);
	parse_tree->print();
	//delete skip_symbols;
}


/**
 * <OclExpression> ::= <call> | <IfExp> | <LiteralExp> | <variable> | <objectTemplate> | 
 *						<concatExpr> | <realExpr> | <intExpr> | <relExpr> | <boolExpr> | <pathName>
 */
bool Parser::OclExpression(Node *n)
{
	bool result = true;
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol);
	if (!call(n2) IfExp(n2) ) 
	{
		delete n2;
		return false;
	}
	n->addChild(n2); 
	return true;
}



/**
 * <pathName2> ::= ‘::’ <simpleName> <pathName2> | Ɛ
 */
bool Parser::pathName2(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol);
	if (!accept(LexicalAtom::namspOp, n2)) 
	{
		n->addChild(n2); //Ɛ
		return true;
	}
	accept(LexicalAtom::simpleName, n2);
	pathName2(n2);
	n->addChild(n2); 
	return true;
}


/**
 * <pathName> ::= <simpleName> <pathName2>
 */
bool Parser::pathName(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::simpleName, n2))
	{
		delete n2; 
		return false;
	}
	pathName2(n2);
	n->addChild(n2); 
	return true;
}



/**
 * <para> ::= <simpleName> ‘:’ <type> 
 */
bool Parser::para(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::simpleName, n2))
	{
		delete n2; //no tragedy
		return false;
	}
	accept(LexicalAtom::colon, n2);
	type(n2);
	n->addChild(n2); 
	return true;
}


/**
 * <propertyTemplate> ::= <simpleName> ‘=’ <OclExpression> 
 */
bool Parser::propertyTemplate(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::simpleName, n2))
	{
		delete n2; 
		return false;
	}
	accept(LexicalAtom::eqOp, n2);
	OclExpression(n2);
	n->addChild(n2); 
	return true;
}


/**
 * <queryCall> ::= <simpleName> ‘(‘ <variable> (‘,’ <variable>)* ‘)’
 */
bool Parser::call(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::simpleName, n2))
	{
		delete n2;
		return false;
	}
	accept(LexicalAtom::lparent, n2);
	variable(n2);
	while (accept(LexicalAtom::comma, n2))
		variable(n2);
	accept(LexicalAtom::rparent, n2);
	n->addChild(n2);
	return true;
}


/**
 * <varDeclaration> ::= <simpleName> (, <simpleName>)* ‘:’ <type> [‘=’ <OclExpression>] ‘;’
 */
bool Parser::varDeclaration(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::simpleName, n2))
	{
		delete n2;
		return false;
	}
	while (accept(LexicalAtom::comma, n2))
		accept(LexicalAtom::simpleName, n2);
	accept(LexicalAtom::colon, n2);
	type(n2);
	if (accept(LexicalAtom::eqOp, n2)) 
		OclExpression(n2);
	accept(LexicalAtom::semicol, n2);
	n->addChild(n2);
	return true;
}


/**
 * <modelDecl> ::= <simpleName> ‘:’ <simpleName>
 */
bool Parser::modelDecl(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); //start new small tree
	if (!accept(LexicalAtom::simpleName, n2))
	{
		delete n2; 
		return false;
	}
	accept(LexicalAtom::colon, n2);
	accept(LexicalAtom::simpleName, n2);
	n->addChild(n2);
	return true;
}


/**
 * <domain> ::= (‘checkonly’ | ‘enforce’) ‘domain’ <simpleName> <objectTemplate> ‘;’
 */
bool Parser::domain(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!( (accept(LexicalAtom::domCKw, n2) == false && accept(LexicalAtom::domEKw, n2) == true) 
		|| (accept(LexicalAtom::domCKw, n2) == true && accept(LexicalAtom::domEKw, n2) == false) ))
	{
		delete n2; 
		return false;
	}
	accept(LexicalAtom::domKw, n2);
	accept(LexicalAtom::simpleName, n2);
	
	objectTemplate(n2);

	accept(LexicalAtom::semicol, n2);
	n->addChild(n2); 
	return true;
}


/**
 * <IfExp> ::= ‘if’ ‘(‘ <OclExpression> ‘)’ ‘then’ <OclExpression> [‘else’ <OclExpression>] ‘endif’
 */
bool Parser::IfExp(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); //start new small tree
	if (!accept(LexicalAtom::ifKw, n2))
	{
		delete n2; //in case of ORs higher up, we don't want many empty nodes attached. 
		return false; //on the other hand, if we do have SOMETHING, we may as well attach it. 
					//and reprot the error from here (from inside this function)
	}
	accept(LexicalAtom::lparent, n2); //if there is any trouble at some point, report it and return false,
	OclExpression(n2);
	accept(LexicalAtom::rparent, n2); //maybe (?) try to parse it further
	accept(LexicalAtom::thenKw, n2);
	OclExpression(n2);
	if (accept(LexicalAtom::elseKw, n2))
		OclExpression(n2);
	accept(LexicalAtom::endifKw, n2);
	n->addChild(n2); 
	return true;
}


/**
 * <primitiveTypeDomain> := ‘primitive’ ‘domain’ <simpleName> ‘:’ <type> ‘;’
 */
bool Parser::primitiveTypeDomain(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol);
	if (!accept(LexicalAtom::domPKw, n2))
	{
		delete n2; 
		return false;
	}
	accept(LexicalAtom::domKw, n2);
	accept(LexicalAtom::simpleName, n2);
	accept(LexicalAtom::colon, n2);
	type(n2);
	accept(LexicalAtom::semicol, n2);
	n->addChild(n2); 
	return true;
}


/**
 * <when> ::= ‘when’ ‘{‘ (<OclExpression> ‘;’)* ‘}’
 */
bool Parser::when(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::whenKw, n2))
	{
		delete n2;
		return false;
	} 
	accept(LexicalAtom::lbracket, n2);
	while (OclExpression(n2))
		accept(LexicalAtom::semicol, n2);
	accept(LexicalAtom::rbracket, n2);
	n->addChild(n2); 
	return true;
}


/**
 * <where> ::= ‘where’ ‘{‘ (<OclExpression> ‘;’)* ‘}’
 */
bool Parser::where(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::whereKw, n2))
	{
		delete n2; 
		return false;
	}
	accept(LexicalAtom::lbracket, n2);

	while (OclExpression(n2))
		accept(LexicalAtom::semicol, n2);

	accept(LexicalAtom::rbracket, n2);
	n->addChild(n2); 
	return true;
}


/**
 * <relation> ::= [‘top’] ‘relation’ <simpleName> 
 * ‘{‘ <varDeclaration>* (<domain> | <prmitiveTypeDomain>)+ [<when>] [<where>] ‘}’
 */
bool Parser::relation(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	accept(LexicalAtom::topKw, n2);
	if (!accept(LexicalAtom::relKw, n2))
	{
		delete n2; 
		return false;
	}
	accept(LexicalAtom::simpleName, n2);
	accept(LexicalAtom::lbracket, n2);
	
	while (varDeclaration(n2)) {};

	domain(n2);
	primitiveTypeDomain(n2);

	while (domain(n2) || primitiveTypeDomain(n2)) {};

	when(n2);
	where(n2);
	accept(LexicalAtom::rbracket, n2);
	n->addChild(n2); 
	return true;
}


/**
 * <query> ::= ‘query’ <simpleName> ‘(‘ [<para> (‘,’ <para>)*] ‘)’‘:’ <type> ‘{‘ (<OclExpression> ‘;’)* ‘}’
 */
bool Parser::query(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); 
	if (!accept(LexicalAtom::querKw, n2))
	{
		delete n2; 
		return false;
	}
	accept(LexicalAtom::simpleName, n2);
	accept(LexicalAtom::lparent, n2); 
	
	para(n2);
	while (para(n2)) {};

	accept(LexicalAtom::rparent, n2); 
	accept(LexicalAtom::colon, n2);
	type(n2);
	accept(LexicalAtom::lbracket, n2);
	
	while (OclExpression(n2))
		accept(LexicalAtom::semicol, n2);

	accept(LexicalAtom::rbracket, n2);
	n->addChild(n2); 
	return true;
}



bool Parser::relOp(Node *n)
{
	if (current_symbol == LexicalAtom::eqOp || current_symbol == LexicalAtom::ltOp || current_symbol == LexicalAtom::gtOp ||
		current_symbol == LexicalAtom::leOp || current_symbol == LexicalAtom::notOp || current_symbol == LexicalAtom::geOp)
	{
		n->addChild(new Node(n, LexicalAtom::nonFinalSymbol));
		return true;
	}
	return false;
}


bool Parser::boolOp(Node *n)
{
	if (current_symbol == LexicalAtom::andOp || current_symbol == LexicalAtom::orOp)
	{
		n->addChild(new Node(n, LexicalAtom::nonFinalSymbol));
		return true;
	}
	return false;
}


bool Parser::arytmOp(Node *n)
{
	if (current_symbol == LexicalAtom::plusOp || current_symbol == LexicalAtom::minusOp 
		|| current_symbol == LexicalAtom::multOp)
	{
		n->addChild(new Node(n, LexicalAtom::nonFinalSymbol));
		return true;
	}
	return false;
}


bool Parser::realOp(Node *n)
{
	if (current_symbol == LexicalAtom::plusOp || current_symbol == LexicalAtom::minusOp 
		|| current_symbol == LexicalAtom::multOp || current_symbol == LexicalAtom::divOp)
	{
		n->addChild(new Node(n, LexicalAtom::nonFinalSymbol));
		return true;
	}
	return false;
}


bool Parser::collectionTypeIdentifier(Node *n)
{
	if (current_symbol == LexicalAtom::setKw || current_symbol == LexicalAtom::bagKw 
		|| current_symbol == LexicalAtom::seqKw || current_symbol == LexicalAtom::collKw
		|| current_symbol == LexicalAtom::ordsetKw)
	{
		n->addChild(new Node(n, LexicalAtom::nonFinalSymbol));
		return true;
	}
	return false;
}


bool Parser::primitiveType(Node *n)
{
	if (current_symbol == LexicalAtom::boolKw || current_symbol == LexicalAtom::intKw // in THIS case we can do simple OR,
		|| current_symbol == LexicalAtom::realKw || current_symbol == LexicalAtom::collKw //because LexicalAtom already IS concrete.
		|| current_symbol == LexicalAtom::strKw)
	{
		n->addChild(new Node(n, LexicalAtom::nonFinalSymbol));
		return true;
	}
	return false;
}


bool Parser::BooleanLit(Node *n)
{
	if (current_symbol == LexicalAtom::falseKw || current_symbol == LexicalAtom::trueKw)
	{
		n->addChild(new Node(n, LexicalAtom::nonFinalSymbol));
		return true;
	}
	return false;
}