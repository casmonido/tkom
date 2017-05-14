#ifndef SCAN
#define SCAN
#include "Source.h"
#include <string>
#include <unordered_map>
#include <limits.h> 
#define MAX_ID_LEN 20


enum LexicalAtom 
{ 
	// keywords, for KeywordTable:
	ifKw, thenKw, elseKw, endifKw, 
	setKw, bagKw, seqKw, collKw, ordsetKw, 
	boolKw, intKw, realKw, strKw, 
	domKw, domPKw, domEKw, domCKw,
	transKw, topKw, relKw, querKw, whenKw, whereKw, //17-22
	trueKw, falseKw, 
	andOp, orOp, 
	nullKw, //27
	NUM_OF_KEYWORDS,
	// the rest (consts, operators): 
	lparent, rparent, lbracket, rbracket, //29-32
	colon, comma, semicol, dot,
	eqOp, ltOp, gtOp, leOp, notOp, geOp, 
	plusOp, minusOp, multOp, divOp, 
	namspOp, //47
	NUM_OF_VISIBLE,

	endfile, unidentSymbol, nonFinalSymbol,
	simpleName, //52
	StringLit, IntegerLit, RealLit,
	MAX_SYMBOL = RealLit - 2
};

extern std::string atoms[];

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
    static const std::unordered_map<std::string, LexicalAtom> table; // <key_type: string, mapped_type: LexicalAtom>
};


class Scan
{
	const std::unordered_map<std::string, LexicalAtom> table =  KeywordTable::Initialise();

	enum ScanError { 
		INT_OUT_OF_BOUNDS,
		//LOSS_OF_PRECISION, //more like a warning?
		IDENTIFIER_TOO_LONG,
		INCOMPLETE_REAL_CONST, 
		STRANGE_CHAR 
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
	void ScanError(int error_number, bool digraph = false);
	LexicalAtom NextSymbol(); // main functionality of this class

	int IntConst() { return last_int_constant; }
	float FloatConst() { return last_float_constant; }
	std::string StringConstant() { return last_string_constant; }
	const char* LastIdentifier() { return last_identifier; }

	bool isNameStartChar(char cur_char);
	bool isNameChar(char cur_char);
	bool isSimpleName(char *str, int len);
	bool isSpace(char cur_char);

	LexicalAtom scanIdentifierOrKeyword();
	LexicalAtom scanReal(long l, int multiplier, bool over);
	LexicalAtom scanIntegerOrReal(int multiplier);
	LexicalAtom scanStringConst();
};

#endif