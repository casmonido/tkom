#ifndef SCAN
#define SCAN
#include "Source.h"
#include <string>
#include <unordered_map>
#include <limits.h> 
#define MAX_ID_LEN 20

enum LexicalAtom 
{ 
	// keywords:
	ifKw, thenKw, elseKw, endifKw, 
	setKw, bagKw, seqKw, collKw, ordsetKw, 
	boolKw, intKw, realKw, strKw, 
	domKw, domPKw, domEKw, domCKw,
	transKw, topKw, relKw, querKw, whenKw, whereKw, 
	trueKw, falseKw, 
	andOp, orOp, 
	nullKw, 
	NUM_OF_KEYWORDS,
	// reszta: 
	endfile, unidentSymbol, 
	simpleName,
	StringLit, IntegerLit, RealLit,
	lparent, rparent, lbracket, rbracket, 
	colon, comma, semicol, dot,
	eqOp, ltOp, gtOp, leOp, notOp, geOp, 
	plusOp, minusOp, multOp, divOp, 
	namspOp,
	MAX_SYMBOL = namspOp - 1
};

struct KeywordTable {
    static std::unordered_map<std::string, LexicalAtom> Initialise()
    {
        std::unordered_map<std::string, LexicalAtom> m;
          m["if"] = ifKw;
          m["then"] = thenKw;
          m["else"] = elseKw;
          m["endif"] = endifKw;
          m["Set"] = setKw;
          m["Bag"] = bagKw;
          m["Sequence"] = seqKw;
          m["Collection"] = collKw;
          m["OrderedSet"] = ordsetKw;
          m["Boolean"] = boolKw;
          m["Integer"] = intKw;
          m["Real"] = realKw;
          m["String"] = strKw;
          m["domain"] = domKw;
          m["primitive"] = domPKw;
          m["enforce"] = domEKw;
          m["checkonly"] = domCKw;
          m["transformation"] = transKw;
          m["top"] = topKw;
          m["relation"] = relKw;
          m["query"] = querKw;
          m["when"] = whenKw;
          m["where"] = whereKw;
          m["true"] = trueKw;
          m["false"] = falseKw;
          m["and"] = andOp;
          m["or"] = orOp;
          m["null"] = nullKw;
        return m;
    }
    static const std::unordered_map<std::string, LexicalAtom> table; ////<key_type, mapped_type>
};

class Scan
{
	const std::unordered_map<std::string, LexicalAtom> table =  KeywordTable::Initialise();

	enum ScanError { 
		INT_OUT_OF_BOUNDS,
		//LOSS_OF_PRECISION,
		IDENTIFIER_TOO_LONG,
		UNIDENTIFIED_SYMBOL
	};

	Source& src; 
	int current_char;
	TextPos atom_position; 

	int last_int_constant; 
	float last_float_constant; 
	std::string last_string_constant;
	char last_identifier[MAX_ID_LEN+1];

	void Nextc() { current_char=src.NextChar(); }

public:
	Scan(Source &s):src(s) { Nextc(); }
	LexicalAtom NextSymbol();// Następny atom
	int IntConst() { return last_int_constant; }
	float FloatConst() { return last_float_constant; }
	std::string StringConstant() { return last_string_constant; }
	const char* LastIdentifier(){ return last_identifier; }
	void ScanError(int error_number);

	bool isNameStartChar(char cur_char) 
	{
		if ((cur_char > 64 && cur_char < 91) || (cur_char > 96 && cur_char < 123))
			return true;
		return false;
	}

	bool isNameChar(char cur_char) 
	{
		if (isNameStartChar(cur_char) || cur_char == '_' || (cur_char > '0' && cur_char < '9')) 
			return true;
		return false;
	}

	bool isSimpleName(char *str, int len)
	{
		if (!(isNameStartChar(str[0])))
			return false;
		for (int i = 0; i < len; ++i)
			if (!(isNameChar(str[i])))
				return false;
		return true;
	}

	bool isSpace(char cur_char)
	{
		if (current_char == '\n' || current_char == '\t' || current_char == ' ') 
			return true;
		return false;
	}

	LexicalAtom scanIdentifier()
	{
		unsigned int len = 0;
		do
		{ 
			if (len < MAX_ID_LEN) 
			{
				last_identifier[len] = current_char;
				len++;
			}
			//else it just ignores rest of the SimpleName //ERROR!!!!
			Nextc(); //ends just after simplename
		} while (isNameChar(current_char));
		last_identifier[len] = '\0';

		if (table.find(last_identifier) != table.end())
			return table.find(last_identifier)->second;
		else 
		{
			std::cout << "(" << last_identifier << " "; //trzeba to wlozyc w tablice symbolis
			return simpleName;	
		}
	}

	LexicalAtom scanReal(long l, int multiplier)
	{
		double whole_double = (double) l;
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
		last_float_constant = whole_double*multiplier;
		std::cout << "(" << last_float_constant << " ";
		return RealLit;
	}

	LexicalAtom scanInteger(int multiplier)
	{
		long l = 0;
		while (isdigit(current_char))
		{ 
			l = l*10 + (current_char-'0');
			if (l > INT_MAX || l < INT_MIN)
				ScanError(INT_OUT_OF_BOUNDS);
			Nextc();
			if (current_char == '.') //real.
				return scanReal(l, multiplier);
		}
		l *= multiplier;
		last_int_constant = (int)l;
		std::cout << "(" << last_int_constant << " ";
		return IntegerLit;
	}
};

#endif