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
		current_symbol = scanner.NextSymbol(); 
	}


	void SyntaxError(int expected_atom) 
	{ 
		std::cout << "syntax error \n";
		//scn.ScanError(FirstSyntaxError+atom, "Spodziewany atom: ", AT[atom]);
		//i powinno przeleciec do expected_atom?
	}

	void SyntaxError1(int unexpected_atom) 
	{ 
		//scn.ScanError(FirstSyntaxError+atom, "Nieoczekiwany current_symbol: ", AT[atom]);
	}

	bool accept(LexicalAtom atom, Node *parent)
	{ 
		if (parent == nullptr)
			SyntaxError(atom);
		else
			if (current_symbol == atom) 
			{
				NextSymbol(); 
				parent->addChild(new Node(parent, atom));
				return true;
			};
		return false;
	};


	bool relOp(Node *);
	bool boolOp(Node *);
	bool arytmOp(Node *);
	bool realOp(Node *);
	bool collectionTypeIdentifier(Node *);
	bool primitiveType(Node *);
	bool primitiveBool(Node *);
	bool modelDecl(Node *);
	bool relation(Node *);
	bool query(Node *);
	bool when(Node *);
	bool where(Node *);
	bool primitiveTypeDomain(Node *);
	bool IfExp(Node *);
	bool domain(Node *);

public:
	Parser(Scan &s): scanner(s)
	{ 
		NextSymbol(); // Pobranie 1-go atomu
		last_open_node = new Node(nullptr, LexicalAtom::nonFinalSymbol); 
		parse_tree = new ParseTree(last_open_node);
	};

	~Parser() 
	{
		delete parse_tree;
	};

	void Transformation(); // Główna procedura rozbioru
}; 



#endif