#ifndef PARSE_TREE
#define PARSE_TREE
#include <iostream>
#include <string>
#include <list>
#include "Scan.h"


extern std::string atoms[];



class Node {
	friend class ParseTree;
private:
	Node *parent;
	LexicalAtom atom;
	std::list<Node *> children;
	bool open; //doesn't have children, isn't one of keywords / operators / other things Scanner could return 
public:

	Node(Node *p, LexicalAtom a): parent(p), atom(a) 
	{
		if (atom > NUM_OF_KEYWORDS)
			open = true;
		else
			open = false;
	};

	void addChild(Node *n)
	{
		n->open = false;
		children.push_back(n);
	};

	bool isOpen()
	{
		return open;
	}
};

//non-binary parse tree

class ParseTree {
	Node *root;
	Scan *r;

	void print(std::list<Node*> *collection)
	{
		int level_width = 1; //root jest 1
		int next_level_width = 0;
		while (!collection->empty())
		{
			level_width--;
			Node *n = collection->front();
			collection->pop_front();
			if (n->atom == LexicalAtom::endfile) //artificial node to make the tree look nice
			{
				std::cout << '\n';
				delete n;
			}
			else
			{
				if (n->atom > NUM_OF_VISIBLE)
				{
					if (n->atom == LexicalAtom::simpleName)
						std::cout << r->LastIdentifier() << '\t';
					else
						if (n->atom == LexicalAtom::StringLit)
							std::cout << r->StringConstant() << '\t';
						else
							if (n->atom == LexicalAtom::IntegerLit)
								std::cout << r->IntConst() << '\t';
							else 
								if (n->atom == LexicalAtom::RealLit)
									std::cout << r->FloatConst() << '\t';
								else
									std::cout << "." << '\t';
				}
				else
					std::cout << atoms[n->atom] << '\t';
				for (std::list<Node*>::iterator i = n->children.begin(); i != n->children.end(); i++)
				{
					collection->push_back(*i);
					next_level_width++;
				}
			}

			
			if (level_width == 0)
			{
				if (next_level_width == 0)
					break;
				level_width = next_level_width;
				next_level_width = 0;
				collection->push_front(new Node(nullptr, LexicalAtom::endfile));
				level_width += 1; // to compensate for that push_back just now
			}
		}
		delete collection;
	}

public:
	ParseTree(Node *r, Scan *rr) : root(r), r(rr) {};

	void delete_tree(Node *n)
	{
		for (std::list<Node*>::iterator i = n->children.begin(); i != n->children.end(); i++)
			delete_tree(*i);
		delete n;
	};

	~ParseTree()
	{
		delete_tree(root);
	};

	void print()
	{
		std::list<Node*> *collection = new std::list<Node*>();
		collection->push_back(root);
		print(collection);
	};

	Node *findLeftOpenNode(Node *n)
	{
		if (n->isOpen()) 
			return n;
		for (std::list<Node*>::iterator i = n->children.begin(); i != n->children.end(); i++)
		{
			findLeftOpenNode(*i);
		}
		return nullptr;
	};
};


#endif
