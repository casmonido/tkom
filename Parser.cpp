#include "Parser.h"
#include <iostream>


/**
 * ‘transformation’ <simpleName> ‘(‘ <modelDecl>‘,’ <modelDecl> ‘)’ ‘{‘ (<relation> | <query>)* ‘}’
 */
void Parser::Transformation() // Transformation = "program" ident ';' Block '.' ;
{ 
	accept(LexicalAtom::transKw, last_open_node);
	accept(LexicalAtom::simpleName, last_open_node);
	accept(LexicalAtom::lparent, last_open_node); 
	modelDecl(last_open_node); 
	accept(LexicalAtom::comma, last_open_node); 
	modelDecl(last_open_node);
	accept(LexicalAtom::rparent, last_open_node); 
	accept(LexicalAtom::lbracket, last_open_node); 
	while (relation(last_open_node) == true || query(last_open_node) == true)
	{}
	accept(LexicalAtom::rbracket, last_open_node);
	parse_tree->print();
}



/**
 * <domain> ::= (‘checkonly’ | ‘enforce’) ‘domain’ <simpleName> <objectTemplate> ‘;’
 */
bool Parser::domain(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); //start new small tree
	if (accept(LexicalAtom::domCKw, n2) == false && accept(LexicalAtom::domEKw, n2) == false)
	{
		delete n2; //no tragedy
		return false;
	}
	accept(LexicalAtom::domKw, n2);
	accept(LexicalAtom::simpleName, n2);
	//
	accept(LexicalAtom::semicol, n2);
	n->addChild(n2); //if everything went ok, attach the tree to main tree
	//else destroy it
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
		delete n2; //no tragedy
		return false;
	}
	accept(LexicalAtom::lparent, n2);
	accept(LexicalAtom::rparent, n2);
	accept(LexicalAtom::thenKw, n2);
	accept(LexicalAtom::elseKw, n2);
	accept(LexicalAtom::endifKw, n2);
	n->addChild(n2); //if everything went ok, attach the tree to main tree
	//else destroy it
	return true;
}




/**
 * <primitiveTypeDomain> := ‘primitive’ ‘domain’ <simpleName> ‘:’ <type> ‘;’
 */
bool Parser::primitiveTypeDomain(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); //start new small tree
	if (!accept(LexicalAtom::domPKw, n2))
	{
		delete n2; //no tragedy
		return false;
	}
	accept(LexicalAtom::domKw, n2);
	accept(LexicalAtom::simpleName, n2);
	accept(LexicalAtom::colon, n2);
	//
	accept(LexicalAtom::semicol, n2);
	n->addChild(n2); //if everything went ok, attach the tree to main tree
	//else destroy it
	return true;
}




/**
 * <when> ::= ‘when’ ‘{‘ (<OclExpression> ‘;’)* ‘}’
 */
bool Parser::when(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); //start new small tree
	if (!accept(LexicalAtom::whenKw, n2))
	{
		delete n2; //no tragedy
		return false;
	}
	accept(LexicalAtom::lbracket, n2);
	//
	accept(LexicalAtom::rbracket, n2);
	n->addChild(n2); //if everything went ok, attach the tree to main tree
	//else destroy it
	return true;
}


/**
 * <where> ::= ‘where’ ‘{‘ (<OclExpression> ‘;’)* ‘}’
 */
bool Parser::where(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); //start new small tree
	if (!accept(LexicalAtom::whereKw, n2))
	{
		delete n2; //no tragedy
		return false;
	}
	accept(LexicalAtom::lbracket, n2);
	//
	accept(LexicalAtom::rbracket, n2);
	n->addChild(n2); //if everything went ok, attach the tree to main tree
	//else destroy it
	return true;
}


/**
 * <relation> ::= [‘top’] ‘relation’ <simpleName> 
 * ‘{‘ <varDeclaration>* (<domain> | <prmitiveTypeDomain>)+ [<when>] [<where>] ‘}’
 */
bool Parser::relation(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); //start new small tree
	accept(LexicalAtom::topKw, n2);
	if (!accept(LexicalAtom::relKw, n2))
	{
		delete n2; //no tragedy
		return false;
	}
	accept(LexicalAtom::simpleName, n2);
	accept(LexicalAtom::lbracket, n2);
	//
	accept(LexicalAtom::rbracket, n2);
	n->addChild(n2); //if everything went ok, attach the tree to main tree
	//else destroy it
	return true;
}


/**
 * <query> ::= ‘query’ <simpleName> ‘(‘ [<para> (‘,’ <para>)*] ‘)’‘:’ <type> ‘{‘ (<OclExpression> ‘;’)* ‘}’
 */
bool Parser::query(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); //start new small tree
	if (!accept(LexicalAtom::querKw, n2))
	{
		delete n2; //no tragedy
		return false;
	}
	accept(LexicalAtom::simpleName, n2);
	accept(LexicalAtom::lparent, n2); 
	//
	accept(LexicalAtom::rparent, n2); 
	accept(LexicalAtom::lbracket, n2);
	//
	accept(LexicalAtom::rbracket, n2);
	n->addChild(n2); //if everything went ok, attach the tree to main tree
	//else destroy it
	return true;
}


/**
 * <modelDecl> ::= <simpleName> ‘:’ <simpleName>
 */
bool Parser::modelDecl(Node *n)
{
	Node *n2 = new Node(n, LexicalAtom::nonFinalSymbol); //start new small tree
	accept(LexicalAtom::simpleName, n2);
	accept(LexicalAtom::colon, n2);
	accept(LexicalAtom::simpleName, n2);
	n->addChild(n2); //if everything went ok, attach the tree to main tree
	return true;
	//else destroy it
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
	if (current_symbol == LexicalAtom::boolKw || current_symbol == LexicalAtom::intKw 
		|| current_symbol == LexicalAtom::realKw || current_symbol == LexicalAtom::collKw
		|| current_symbol == LexicalAtom::strKw)
	{
		n->addChild(new Node(n, LexicalAtom::nonFinalSymbol));
		return true;
	}
	return false;
}


bool Parser::primitiveBool(Node *n)
{
	if (current_symbol == LexicalAtom::falseKw || current_symbol == LexicalAtom::trueKw)
	{
		n->addChild(new Node(n, LexicalAtom::nonFinalSymbol));
		return true;
	}
	return false;
}