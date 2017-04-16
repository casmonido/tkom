#include <iostream>
#include <cctype>
#include <cstring>
#include <climits>
#include "Scan.h"
using namespace std;


Scan::KeywordDescriptor Scan::hashed_keyword_table[NUM_OF_KEYWORDS] = {
	{(char *)"and", andOp},
	{(char *)"Bag", bagKw},
	{(char *)"Collection", collKw},
	{(char *)"domain", domPKw},
	{(char *)"domain", domKw},
	{(char *)"enforce", domEKw},
	{(char *)"checkonly", domCKw},
	{(char *)"null", nullKw},
	{(char *)"if", ifKw},
	{(char *)"then", thenKw},
	{(char *)"else", elseKw},
	{(char *)"endif", endifKw},
	{(char *)"transformation", transKw}, 
	{(char *)"top", topKw},
	{(char *)"relation", relKw}, 
	{(char *)"when", whenKw},
	{(char *)"where", whereKw},
	{(char *)"query", querKw},
	{(char *)"Set", setKw},
	{(char *)"Sequence", seqKw},
	{(char *)"OrderedSet", ordsetKw},
	{(char *)"Integer", intKw}, 
	{(char *)"Real", realKw},
	{(char *)"String", strKw}, 
	{(char *)"or", orOp}, 
	{(char *)"true", trueKw},
	{(char *)"false", falseKw}
};

unsigned Scan::hash(char *s, unsigned int len)
{ 
	/**
	unsigned h;
	//if(strcmp(s, "not")==0) // Korekta kolizji
	//return 0;
	h = (s[0]>>1) * s[1] * 1241;
	if(len>=3) h += s[len-1];
	return h % NUM_OF_KEYWORDS;
	*/
	for (int i = 0; i < NUM_OF_KEYWORDS; ++i)
		if (hashed_keyword_table[i].keyword == nullptr)
			return 0;
		else
			if (strcmp(hashed_keyword_table[i].keyword, s) == 0)
				return i;
	return 0;
}

LexicalAtom Scan::NextSymbol() // Główna usługa scanera
{ 
	do
	{
		if (current_char == EOF) 
			return endfile;
		while (Space(current_char))
			Nextc();
		if (current_char == '-') 
		{
			Nextc();
			if (current_char != '-')
				return subtrOp;
			else //comment, but it may end in one of two ways
			{
				while (current_char != '\n' && current_char != EOF)
					Nextc();
				if (current_char == '\n')
					Nextc();
				else
					return endfile;
			}
		}
	} while (Space(current_char) || current_char == '-' || current_char == EOF);

	atom_position = src.GetPos();
	//---IDENTYFIKATOR LUB SŁOWO KLUCZOWE
	if (NameStartChar(current_char))
	{
		unsigned int len = 0, h;
		do
		{ 
			if (len < MAX_ID_LEN) 
			{
				last_identifier[len] = current_char;
				len++;
			}
			//else it just ignores rest of the SimpleName
			Nextc(); //ends just after simplename
		} while (NameChar(current_char));

		last_identifier[len] = '\0';
		h = hash(last_identifier,len);

		if (strcmp(hashed_keyword_table[h].keyword, last_identifier) == 0)
			return hashed_keyword_table[h].atom;
		else 
		{
			cout << "(" << last_identifier << " "; //trzeba to wlozyc w tablice symbolis
			return simpleName;	
		}
	}
	else
	//---STAŁA: INTEGER LUB REAL lub znaki
		if (isdigit(current_char))
		{ 
			long l = 0;
			do
			{ 
				l = l*10 + (current_char-'0');
				Nextc();
				if (current_char == '.') //real.
				{
					double whole_double = l;
					double one_char_double = 0;
					int num_of_zeroes_after_dot = 1;
					Nextc();
					if (!(isdigit(current_char))) //error: incomplete char 
						return unidentSymbol;
					while (isdigit(current_char))
					{
						one_char_double = current_char - '0';
						for (int i = 0; i < num_of_zeroes_after_dot; ++i)
							one_char_double = one_char_double / 10;
						num_of_zeroes_after_dot += 1;
						whole_double += one_char_double;
						Nextc();
					}
					last_float_constant = whole_double;
					cout << "(" << last_float_constant;
					return RealLit;
				};


			} while (isdigit(current_char));
			last_int_constant = (int)l;
			cout << "(" << last_int_constant << " ";
			return IntegerLit;
		}
		else
		{
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
						if (current_char == '=')
						{
							Nextc();
							return assignOp;
						}
						else
							if (current_char == ':')
							{
								Nextc();
								return namspOp;
							}
							return isOp;
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
						return addOp;
				case '-':
						Nextc();
						return subtrOp;
				case '*':
						Nextc();
						return multOp;
				case '/':
						Nextc();
						return divOp;
				case '.':
						Nextc();
						return dotOp;
			}
		}
	return unidentSymbol; 
}



void Scan::ScanError(int ec, const char *mt, const char *txt_atom)
{ 
}

