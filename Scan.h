#ifndef SCAN
#define SCAN
#include "Source.h"
#include <string>
#define MAX_ID_LEN 20




enum LexicalAtom 
{ 
	// keywords:
	endfile,
	andOp, bagKw, collKw, boolKw,
	domPKw, domKw, domEKw, domCKw,
	nullKw, ifKw, thenKw, elseKw,
	endifKw, transKw, topKw, relKw,
	whenKw, whereKw, querKw, setKw,
	seqKw, ordsetKw, intKw, realKw,
	strKw, orOp, subtrOp, trueKw,
	falseKw, NUM_OF_KEYWORDS,
	// reszta: 
	simpleName, unidentSymbol,
	StringLit, BooleanLit, IntegerLit, RealLit,
	isOp, namspOp, lparent, rparent,
	lbracket, rbracket, comma, semicol,
	eqOp, ltOp, gtOp, leOp,
	notOp, geOp, addOp, multOp,
	divOp, dotOp, assignOp,
	MAX_SYMBOL
};




class Scan
{

	struct KeywordDescriptor // Deskryptor słowa kluczowego
	{ 
		char *keyword;
		LexicalAtom atom;
		//KeywordDescriptor(char *k, LexicalAtom a): keyword(k), atom(a) {};
	};

	static KeywordDescriptor hashed_keyword_table[NUM_OF_KEYWORDS];
	Source& src; 
	int current_char;
	TextPos atom_position; 

	int last_int_constant; 
	float last_float_constant; 
	std::string last_string_constant;
	char last_identifier[MAX_ID_LEN+1];

	void Nextc() { current_char=src.NextChar(); }
	static unsigned hash(char *s, unsigned int len);
	// Funkcja mieszająca dla hashed_keyword_table

public:
	Scan(Source &s):src(s) { Nextc(); }
	LexicalAtom NextSymbol();// Następny atom
	void ScanError(int ecode, const char *mtxt=0, const char *atxt=0);
	int IntConst() { return last_int_constant; }
	float FloatConst() { return last_float_constant; }
	const char* LastIdentifier(){ return last_identifier; }

	bool NameStartChar(char cur_char) 
	{
		if ((cur_char > 64 && cur_char < 91) || (cur_char > 96 && cur_char < 123))
			return true;
		return false;
	}

	bool NameChar(char cur_char) 
	{
		if (NameStartChar(cur_char) || cur_char == '_' || (cur_char > '0' && cur_char < '9')) 
			return true;
		return false;
	}

	bool SimpleName(char *str, int len)
	{
		if (!(NameStartChar(str[0])))
			return false;
		for (int i = 0; i < len; ++i)
			if (!(NameChar(str[i])))
				return false;
		return true;
	}

	bool Space(char cur_char)
	{
		if (current_char == '\n' || current_char == '\t' || current_char == ' ') 
			return true;
		return false;
	}

};

#endif