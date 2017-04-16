#include <iostream>
#include "Source.h"
#include "Scan.h"
using namespace std;

int main() 
{

	Source *s = new Source("plik.txt");
	/**
	int r = 0;
	while ((r=s->NextChar()) != EOF)
		std::cout << r << '\n'; */ 

	Scan *sc = new Scan(*s);

	LexicalAtom la;
	while((la = sc->NextSymbol())!=endfile)
	{
		
		switch (la)
		{
			case trueKw:
						cout << "true ";
						break;
			case falseKw:
						cout << "false ";
						break;
			case nullKw:
						cout << "null ";
						break;
			case ifKw:
						cout << "if ";
						break;
			case thenKw:
						cout << "then ";
						break;
			case elseKw:
						cout << "else ";
						break;
			case endifKw:
						cout << "endif ";
						break;
			case transKw:
						cout << "transformation ";
						break;
			case topKw:
						cout << "top ";
						break;
			case relKw:
						cout << "relation ";
						break;
			case whenKw:
						cout << "when ";
						break;
			case whereKw:
						cout << "where ";
						break;
			case domPKw:
						cout << "primitive ";
						break;
			case domKw:
						cout << "domain ";
						break;
			case domEKw:
						cout << "enforce ";
						break;
			case domCKw:
						cout << "checkonly ";
						break;
			case querKw:
						cout << "query ";
						break;
			case setKw:
						cout << "Set ";
						break;
			case bagKw:
						cout << "Bag ";
						break;
			case seqKw:
						cout << "Sequence ";
						break;
			case collKw:
						cout << "Collection ";
						break;
			case ordsetKw:
						cout << "OrderedSet ";
						break;
			case boolKw:
						cout << "Boolean ";
						break;
			case intKw:
						cout << "Integer ";
						break;
			case realKw:
						cout << "Real ";
						break;
			case strKw:
						cout << "String ";
						break;
			case isOp:
						cout << ": ";
						break;
			case namspOp:
						cout << "::";
						break;
			case lparent:
						cout << "(";
						break;
			case rparent:
						cout << ") \n";
						break;
			case lbracket:
						cout << "{ \n";
						break;
			case rbracket:
						cout << "} \n";
						break;
			case comma:
						cout << ", ";
						break;
			case semicol:
						cout << "; \n";
						break;
			case eqOp:
						cout << "= ";
						break;
			case ltOp:
						cout << "< ";
						break;
			case gtOp:
						cout << "> ";
						break;
			case leOp:
						cout << "<= ";
						break;
			case notOp:
						cout << "<> ";
						break;
			case geOp:
						cout << ">= ";
						break;
			case addOp:
						cout << "+ ";
						break;
			case subtrOp:
						cout << "- ";
						break;
			case multOp:
						cout << "* ";
						break;
			case divOp:
						cout << "/ ";
						break;
			case dotOp:
						cout << ".";
						break;
			case andOp:
						cout << "and ";
						break;
			case orOp:
						cout << "or ";
						break;
			case assignOp:
						cout << ":= ";
						break;
			case simpleName:
						cout << ":simpleName) ";
						break;
			case StringLit:
						cout << ":StringLit) ";
						break;
			case IntegerLit:
						cout << ":IntegerLit) ";
						break;
			case RealLit:
						cout << ":RealLit) ";
						break;
			default: 
						cout << "???? ";
		};

	}
	delete(sc);
	delete(s);
}