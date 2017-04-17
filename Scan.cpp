#include <iostream>
#include <cctype>
#include <cstring>
#include <climits>

#include "Scan.h"
using namespace std;


LexicalAtom Scan::NextSymbol() // Główna usługa scanera
{ 
	while (isSpace(current_char) || current_char == '-' || current_char == EOF)
	{
		if (current_char == EOF) 
			return endfile;
		while (isSpace(current_char))
			Nextc();
		if (current_char == '-') 
		{
			Nextc();
			if (isdigit(current_char))
				return scanInteger(-1);
			else
				if (current_char == '-') //comment, but it may end in one of two ways
					{
						while (current_char != '\n' && current_char != EOF)
							Nextc();
						if (current_char == '\n')
							Nextc();
						else
							return endfile;
					}
				else
					return minusOp;
		}
	};

	atom_position = src.GetPos();

	if (isNameStartChar(current_char))
		return scanIdentifier();
	else
		if (isdigit(current_char))
			return scanInteger(1);
		else
			switch(current_char)
			{
				case '\'':
						last_string_constant = "";
						while (1)
						{
							Nextc();
							if (current_char != '\'')
								last_string_constant += current_char;
							else
								break;
						} 
						Nextc();
						cout << "(" << last_string_constant << " ";
						return StringLit;
				case '<':
						Nextc();
						if (current_char == '=')
						{
							Nextc();
							return leOp;
						}
						else
							if (current_char == '>')
							{
								Nextc();
								return notOp;
							}
							else
								return ltOp;
				case '>':
						Nextc();
						if (current_char == '=')
						{
							Nextc();
							return geOp;
						}
						else
							return gtOp;
				case ':':
						Nextc();
						if (current_char == ':')
						{
							Nextc();
							return namspOp;
						}
						return colon;
				case '(':
						Nextc();
						return lparent;
				case ')':
						Nextc();
						return rparent;
				case '{':
						Nextc();
						return lbracket;
				case '}':
						Nextc();
						return rbracket;
				case ',':
						Nextc();
						return comma;
				case ';':
						Nextc();
						return semicol;
				case '=':
						Nextc();
						return eqOp;
				case '+':
						Nextc();
						return plusOp;
				case '*':
						Nextc();
						return multOp;
				case '/':
						Nextc();
						return divOp;
				case '.':
						Nextc();
						return dot;
			}
	return unidentSymbol; 
}



void Scan::ScanError(int error_number)
{
	static const char *ScnErr[] = {
		"Integer out of bounds", // 0
		//"Possible loss of precision",
		"Identifier too long - it needs to be up o 20 characters",
		"Unidentified symbol"
	};  
	src.Error(error_number, atom_position, ScnErr[error_number]); 
}

