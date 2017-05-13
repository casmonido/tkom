#ifndef PARSER
#define PARSER
#include "Scan.h"
#include "Tree.h"


class Parser
{
	Scan& scanner; // Moduł scanera
	LexicalAtom current_symbol;
	ParseTree *parse_tree;
	Node *last_open_node;

	void NextSymbol() // Pobranie następnego symbolu
	{ 
		current_symbol=scanner.NextSymbol(); 
	}


	void SyntaxError(int expected_atom) 
	{ 
		//scn.ScanError(FirstSyntaxError+atom, "Spodziewany atom: ", AT[atom]);
		//i powinno przeleciec do expected_atom?
	}

	void SyntaxError1(int unexpected_atom) 
	{ 
		//scn.ScanError(FirstSyntaxError+atom, "Nieoczekiwany current_symbol: ", AT[atom]);
	}

	void accept(LexicalAtom atom, Node *parent)
	{ 
		if (parent == nullptr)
			return; //RZUC WYJATEK CZY COS
		if (current_symbol==atom) 
		{
			NextSymbol(); 
			parent->addChild(new Node(parent, atom));
		}
	};

	void modelDecl();

public:
	Parser(Scan &s): scanner(s)
	{ 
		NextSymbol(); // Pobranie 1-go atomu
		last_open_node = new Node(nullptr, LexicalAtom::transKw); //transKw to nie to głupku
		parse_tree = new ParseTree(last_open_node);
	};

	~Parser() 
	{
		delete parse_tree;
	};

	void Transformation(); // Główna procedura rozbioru
}; 



#endif