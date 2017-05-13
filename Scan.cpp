#include <iostream>
#include <cctype>
#include <cstring>
#include <climits>

#include "Scan.h"
using namespace std;


std::string atoms[MAX_SYMBOL+1] =
{
	// keywords, for KeywordTable:
	"if", "then", "else", "endif", 
	"Set", "Bag", "Sequence", "Collection", "OrderedSet", 
	"Boolean", "Integer", "Real", "String", 
	"domain", "primitive", "enforce", "checkonly",
	"transfromation", "top", "relation", "query", "when", "where", 
	"true", "false", 
	"and", "or", 
	"null" /*,
	NUM_OF_KEYWORDS,
	// the rest (consts, operators): 
	"EOF", unidentSymbol, 
	simpleName,
	StringLit, IntegerLit, RealLit,
	"(", ")", "{", "}", 
	":", ",", ";", ".",
	"=", "<", ">", "<=", "<>", ">=", 
	"+", "-", "*", "/", 
	"::", 

	**/
	//MAX_SYMBOL //
};

	LexicalAtom Scan::NextSymbol()
	{ 
		// discard whitespaces and comments, but watch out for - operator
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
					return scanIntegerOrReal(-1);
				else
					if (current_char == '-') // it's a comment, but it may end in one of two ways
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
			return scanIdentifierOrKeyword(); 
		else
			if (isdigit(current_char))
				return scanIntegerOrReal(1);
			else
				switch(current_char)
				{
					case '\'':
							return scanStringConst();
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
					default:
							// do not change this order, it will mess up the charater position count 
							while (current_char < 0)
								Nextc(); // it is PROBABLY a character encoded on two byes //or more? 
							Nextc();
							ScanError(STRANGE_CHAR, true); 
				}
		return unidentSymbol; 
	}


	void Scan::ScanError(int error_number, bool digraph)
	{
		static const char *ScnErr[] = {
			"Integer out of bounds", // 0
			//"Possible loss of precision",
			"Identifier too long - it needs to be up o 20 characters",
			"Incorrect declaration of a real constant", // e.g.: 57.
			"Unrecognized char used"
		};  
		src.Error(error_number, atom_position, ScnErr[error_number], digraph); 
	}


	bool Scan::isNameStartChar(char cur_char) 
	{
		if ((cur_char >= 'A' && cur_char <= 'Z') || (cur_char >= 'a' && cur_char <= 'z'))
			return true;
		return false;
	}


	bool Scan::isNameChar(char cur_char) 
	{
		if (isNameStartChar(cur_char) || cur_char == '_' || (cur_char > '0' && cur_char < '9')) 
			return true;
		return false;
	}


	bool Scan::isSimpleName(char *str, int len)
	{
		if (!(isNameStartChar(str[0])))
			return false;
		for (int i = 0; i < len; ++i)
			if (!(isNameChar(str[i])))
				return false;
		return true;
	}


	bool Scan::isSpace(char cur_char)
	{
		if (current_char == '\n' || current_char == '\t' || current_char == ' ') 
			return true;
		return false;
	}


	LexicalAtom Scan::scanIdentifierOrKeyword()
	{
		unsigned int len = 0;
		bool over = false;
		while (isNameChar(current_char))
		{ 
			if (len < MAX_ID_LEN) 
			{
				last_identifier[len] = current_char;
				len++;
			}
			else
				over = true;
			Nextc(); //ends just after simplename
		};
		last_identifier[len] = '\0';
		if (over)
			ScanError(IDENTIFIER_TOO_LONG);

		if (table.find(last_identifier) != table.end())
			return table.find(last_identifier)->second;
		else 
			return simpleName;	
	}


	LexicalAtom Scan::scanReal(long l, int multiplier, bool over)
	{
		double whole_double = (double) l;
		double one_char_double = 0;
		int num_of_zeroes_after_dot = 1;
		Nextc();
		if (!(isdigit(current_char)))
		{	
			ScanError(INCOMPLETE_REAL_CONST);
			return unidentSymbol;
		}
		while (isdigit(current_char))
		{
			one_char_double = current_char - '0';
			for (int i = 0; i < num_of_zeroes_after_dot; ++i)
				one_char_double = one_char_double / 10;
			num_of_zeroes_after_dot += 1;
			whole_double += one_char_double;
			Nextc();
		}
		if (over)
			ScanError(INT_OUT_OF_BOUNDS); //but i want it to be scanned...
		last_float_constant = whole_double*multiplier;
		return RealLit;
	}


	LexicalAtom Scan::scanIntegerOrReal(int multiplier)
	{
		long l = 0;
		bool over = false;
		while (isdigit(current_char))
		{ 
			l = l*10 + (current_char-'0');
			Nextc();
			if (l > INT_MAX)
				over = true;
			if (current_char == '.') //real.
				return scanReal(l, multiplier, over);
		}
		if (over)
			ScanError(INT_OUT_OF_BOUNDS);
		l *= multiplier;
		last_int_constant = (int)l;
		return IntegerLit;
	}


	LexicalAtom Scan::scanStringConst()
	{
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
		return StringLit;
	}