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

	void print(std::list<Node*> *collection)
	{
		while (!collection->empty())
		{
			Node *n = collection->front();
			collection->pop_front();
			std::cout << atoms[n->atom] << '\t';
			for (std::list<Node*>::iterator i = n->children.begin(); i != n->children.end(); i++)
				collection->push_back(*i);
		}
		delete collection;
	}

public:
	ParseTree(Node *r) : root(r) {};

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
