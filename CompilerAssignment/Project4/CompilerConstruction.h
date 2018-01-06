#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include <cstdio>
#include "Parser.h"
using namespace std;
static int index = 0;
//static int index_2 = 0;
class CompilerConstruction
{
public:
	CompilerConstruction();
	~CompilerConstruction();
	
	struct LEX {
		string TOKEN;
		string LEXEME;
	};
	string ids;
	bool TOKEN_MAKER(string& lex, LEX& TOK_LEX);
	bool SYMBOL_IDENTIFIER(string lex);
	bool IS_IN(string a, string b[], int l);
	bool IS_IN_NL(char a);
	bool IS_IN_D(char a);
	bool IS_IN_D(string& s);
	bool IS_IN_DT(string st);
	bool IS_COMMENT(string st);
	int lexical_output(int argc, char* argv[]);
	bool IS_IN_AO(char a);
	//2nd phase
	void find_sentence();                         // This finds sentence line by line
	


};

