#ifndef PARSER
#define PARSER
#include "Scan.h"
#include "Tree.h"
#include <list>



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
		std::cout << "syntax error: expected " << expected_atom << ", got " << current_symbol << std::endl;
		//scn.ScanError(FirstSyntaxError+atom, "Spodziewany atom: ", AT[atom]);
	}

	void SyntaxError1(int unexpected_atom) 
	{ 
		//scn.ScanError(FirstSyntaxError+atom, "Nieoczekiwany current_symbol: ", AT[atom]);
	}

	bool accept(LexicalAtom atom, Node *parent, std::list<LexicalAtom> *skip_symbols=nullptr) 
	{ 
		if (parent == nullptr)
			std::cout << "Dearest, you forgot to pass a parent to a node. I can't ACCEPT that.\n";
		else
			if (current_symbol == atom) 
			{
				NextSymbol(); 
				parent->addChild(new Node(parent, atom));
				return true;
			}
			else
				if (skip_symbols != nullptr) // clearly there are paralell possibilities
				{
					skipToOneOfThese(skip_symbols);
					SyntaxError(atom);
				}
		return false;
	};


//should these be follow , the atom we re looking for, or what? 
	void skipToOneOfThese(std::list<LexicalAtom> *c)
	{
		while (current_symbol != LexicalAtom::endfile)
		{
			for (std::list<LexicalAtom>::iterator i = c->begin(); i != c->end(); i++)
				if (current_symbol == *i)
					return;
			NextSymbol();
		}
	}

	bool relOp(Node *);
	bool boolOp(Node *);
	bool arytmOp(Node *);
	bool realOp(Node *);
	bool collectionTypeIdentifier(Node *);
	bool primitiveType(Node *);
	bool BooleanLit(Node *);

	bool relation(Node *);
	bool query(Node *);
	bool when(Node *);
	bool where(Node *);
	bool primitiveTypeDomain(Node *);
	bool IfExp(Node *);
	bool domain(Node *);

	bool modelDecl(Node *);
	bool varDeclaration(Node *);
	bool call(Node *);
	bool propertyTemplate(Node *);
	bool para(Node *);

	bool pathName(Node *);
	bool pathName2(Node *);
	bool variable(Node *);
	bool variable2(Node *);
	bool type(Node *);
	bool collectionType(Node *);

	bool OclExpressionPart2(Node *);
	bool OclExpressionPart3(Node *);
	bool OclExpressionPart1(Node *);
	bool OclExpression1(Node *);
	bool OclExpression3(Node *);
	bool OclExpression2(Node *);
	bool OclExpressionPrim2(Node *);
	bool OclExpressionPrim(Node *);
	bool OclExpression(Node *);


	bool concatExpr(Node *);
	bool concatExpr2(Node *);
	bool intExpr(Node *);
	bool intExpr2(Node *);
	bool realExpr(Node *);
	bool realExpr2(Node *);

	bool boolExpr2(Node *);
	bool boolExpr(Node *);
	bool relExpr(Node *);
	bool objectTemplate(Node *);
	bool propertyTemplateList(Node *);

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