#ifndef SYMBOL_ARRAY
#define SYMBOL_ARRAY
#include <list>
//powinna byc chyba jedna na poziom

//adderal

class SymbolArrayElement
{
	String name;
	String type;
	Node *value;

	SymbolArrayElement(String n, String t, Node *v):
	name(n), type(t), value(v) {};
}

class SymbolArray
{
	std::list<SymbolArrayElement> symbol_array;

public:
	void addElement(SymbolArrayElement e)
	{
		symbol_array.push_back(e);
	}
}

#endif
