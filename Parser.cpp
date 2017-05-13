#include "Parser.h"
#include <iostream>


/**
 * ‘transformation’ <simpleName> ‘(‘ <modelDecl>‘,’ <modelDecl> ‘)’ ‘{‘ (<relation> | <query>)* ‘}’
 */
void Parser::Transformation() // Transformation = "program" ident ';' Block '.' ;
{ 
	accept(LexicalAtom::transKw, last_open_node);
	return;
	accept(LexicalAtom::simpleName, last_open_node);
	accept(LexicalAtom::lparent, last_open_node);
	modelDecl(); 
	accept(LexicalAtom::comma, last_open_node); 
	modelDecl();
	accept(LexicalAtom::rparent, last_open_node);
	accept(LexicalAtom::lbracket, last_open_node);
	//while (relation() == true || query() == true)
	//{}
	accept(LexicalAtom::rbracket, last_open_node);
}

void Parser::modelDecl()
{

}