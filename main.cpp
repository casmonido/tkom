#include <iostream>
#include <cstdlib>
#include "Source.h"
#include "Scan.h"
#include "Parser.h"
using namespace std;

int main(int argv, char ** argc) 
{
	if (argv < 3)
	{
		cout << "Przykladowe wywolanie: ./start 1 test1.txt\n";
		return 1;
	}
	char *file = argc[2];
	Source *s = new Source(file);
	Scan *sc = new Scan(*s);
	Parser *parser = new Parser(*sc);

	parser->Transformation();

	delete parser;
	delete sc;
	delete s;
}

