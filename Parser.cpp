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
	//while (relation() == true || query() == true)
	//{}
	accept(LexicalAtom::rbracket, last_open_node);

	parse_tree->print();
}

void Parser::modelDecl(Node * n)
{
	
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