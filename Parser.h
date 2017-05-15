#ifndef PARSER
#define PARSER
#include "Scan.h"
#include "Tree.h"
#include <list>
#include <cstring>



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

		scanner.ScanError(expected_atom, false, 
			("Spodziewany atom: "+atoms[expected_atom]+", otrzymano "+atoms[current_symbol]).c_str());
	}

	void SyntaxError1(int unexpected_atom) 
	{ 
		//scn.ScanError(FirstSyntaxError+atom, "Nieoczekiwany current_symbol: ", AT[atom]);
	}

	bool accept(LexicalAtom atom, Node *parent, int mode=1)// std::list<LexicalAtom> *skip_symbols=nullptr) 
	{ 
		if (parent == nullptr)
			std::cout << "Dearest, you forgot to pass a parent to a node. I can't ACCEPT that.\n";
		else
			if (current_symbol == atom) 
			{
				if (atom == LexicalAtom::simpleName)
					parent->addChild(new Node(parent, atom, scanner.LastIdentifier()));
				else
					parent->addChild(new Node(parent, atom));
				NextSymbol(); 
				return true;
			};
		if (mode)
			SyntaxError(atom);
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

	bool relation(Node *, std::list<LexicalAtom> *, int mode=1);
	bool query(Node *,  std::list<LexicalAtom> *, int mode=1);
	bool when(Node *, int mode=1);
	bool where(Node *, int mode=1);
	bool primitiveTypeDomain(Node *, int mode=1);
	bool IfExp(Node *, int mode=1);
	bool domain(Node *, int mode=1);

	bool modelDecl(Node *, int mode=1);
	bool varDeclaration(Node *, int mode=1);
	bool call(Node *, int mode=1);
	bool propertyTemplate(Node *, int mode=1);
	bool para(Node *, int mode=1);

	bool pathName(Node *, int mode=1);
	bool pathName2(Node *, int mode=1);
	bool variable(Node *, int mode=1);
	bool variable2(Node *, int mode=1);
	bool type(Node *, int mode=1);
	bool collectionType(Node *, int mode=1);

	bool OclExpressionPart2(Node *, int mode=1);
	bool OclExpressionPart3(Node *, int mode=1);
	bool OclExpressionPart1(Node *, int mode=1);
	bool OclExpression1(Node *, int mode=1);
	bool OclExpression3(Node *, int mode=1);
	bool OclExpression2(Node *, int mode=1);
	bool OclExpressionPrim2(Node *, int mode=1);
	bool OclExpressionPrim(Node *, int mode=1);
	bool OclExpression(Node *, int mode=1);


	bool concatExpr(Node *, int mode=1);
	bool concatExpr2(Node *, int mode=1);
	bool intExpr(Node *, int mode=1);
	bool intExpr2(Node *, int mode=1);
	bool realExpr(Node *, int mode=1);
	bool realExpr2(Node *, int mode=1);

	bool boolExpr2(Node *, int mode=1);
	bool boolExpr(Node *, int mode=1);
	bool relExpr(Node *, int mode=1);
	bool objectTemplate(Node *, int mode=1);
	bool propertyTemplateList(Node *, int mode=1);

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