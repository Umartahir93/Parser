#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include <cstdio>

using namespace std;

class Parser{
private:
	struct LEX_ANAL{
		string TOK;
		string LEX;
	};
	LEX_ANAL lookAhead;
	int tabs;
	int CURRENT_TOKEN;
	string IO_ERROR, SYNTAX_ERROR, ASSIGN_ERROR;
	string IDENTIFIERS;
	string KEYWORDS[6];

public:
	Parser(){
		lookAhead.TOK = "";
		lookAhead.LEX = "";
		tabs = 0;
		CURRENT_TOKEN = 0;
		IO_ERROR = "[IO ERROR]";
		SYNTAX_ERROR = "[SYNTAX ERROR]";
		ASSIGN_ERROR = "[ASSIGN ERROR]";
		KEYWORDS[0] = "GET"; KEYWORDS[1] = "IF"; KEYWORDS[2] = "ELSE"; KEYWORDS[3] = "WHILE"; KEYWORDS[4] = "RETURN";
		KEYWORDS[5] = "PRINT";

	}
	void startParser(){
		//start the work
		DL();
	}
	bool SYMBOL_IDENTIFIER(string lex) {
		int checker = 0;
		int i = 0;
		for (int i = 0; i < 6; ++i) {
			if (lex == KEYWORDS[i]) {
				return true;
			}
		}
		return false;
	}
	bool isEQUALS(string TOK){
		if (TOK == "="){
			return true;
		}
	}
	bool isOperator(string TOK){
		if (TOK == "-" || TOK == "+" || TOK == "*" || TOK == "/"){
			return true;
		}
		return false;
	}
	bool isRO(){
		if (lookAhead.TOK == "RO"){
			if (lookAhead.LEX == "GE" || lookAhead.LEX == "GT" || lookAhead.LEX == "LE" || lookAhead.LEX == "LT" ||
				lookAhead.LEX == "EE" || lookAhead.LEX == "NE"){
				if (lookAhead.LEX == "GE")
					lookAhead.LEX = ">=";
				else if (lookAhead.LEX == "GT")
					lookAhead.LEX = ">";
				else if (lookAhead.LEX == "LE")
					lookAhead.LEX = "<=";
				else if (lookAhead.LEX == "LT")
					lookAhead.LEX = "<";
				else if (lookAhead.LEX == "EE")
					lookAhead.LEX = "==";
				else if (lookAhead.LEX == "NE")
					lookAhead.LEX = "!=";
				return true;
			}
		}
		return false;
	}
	void match(string token, std::istream &LEX){
		if (lookAhead.TOK == token){
			lookAhead = nextTok(LEX);
		}
	}
	LEX_ANAL nextTok(std::istream &LEX){
		if (CURRENT_TOKEN == 0){

			string temp;
			getline(LEX, temp);
			//Sort out TOKENS and LEXEMES
			string TOKEN, LEXEME;
			bool seenTab = false;
			for (int i = 0; (unsigned)i < temp.size(); i++){
				if (!seenTab && temp[i] != '\t'){
					TOKEN += temp[i];
				}
				else if (temp[i] == '\t' || temp[i] == ' '){
					seenTab = true;
					continue;
				}
				else{
					LEXEME += temp[i];
				}
			}
			//PARSER << TOKEN << endl << LEXEME << endl;
			LEX_ANAL t1;
			t1.TOK = TOKEN;
			t1.LEX = LEXEME;
			CURRENT_TOKEN++;
			return t1;
		}
		else{
			string temp;
			getline(LEX, temp);
			//Sort out TOKENS and LEXEMES
			string TOKEN, LEXEME;
			bool seenTab = false;
			for (int i = 0; i < temp.size(); i++){
				if (!seenTab && temp[i] != '\t'){
					TOKEN += temp[i];
				}
				else if (temp[i] == '\t' || temp[i] == ' '){
					seenTab = true;
					continue;
				}
				else{
					LEXEME += temp[i];
				}
			}
			//PARSER << TOKEN << endl << LEXEME << endl;
			LEX_ANAL t1;
			t1.TOK = TOKEN;
			t1.LEX = LEXEME;
			CURRENT_TOKEN++;
			return t1;
		}
	}
	void printTabs(std::ostream &PARSER){
		for (int i = 0; i < tabs; i++){
			PARSER << '\t';
		}
	}
	bool IS_IN_AO(string a){
		string NONLETTERS[4] = { "*", "/", "+", "-" };
		for (int i = 0; i < 4; i++){
			if (a == NONLETTERS[i]){
				return true;
			}
			
		}
		return false;
	}
	bool IS_IN_NL(char a){
		string NONLETTERS[20] = { "=", ",", "+", "-", "*", "/", "<", "<=", ">", ">=", "==", "{", "}", "[", "]", "(", ")", " ", "\t", "\n" };
		for (int i = 0; i < 20; i++){
			for (int j = 0; j < NONLETTERS[i].size(); j++){
				if (a == NONLETTERS[i][j]){
					return true;
				}
			}
		}
		return false;
	}
	bool IS_IN_D(char a){
		if (a > 48 && a < 59){
			return true;
		}
		return false;
	}
	bool IS_IN_D(string& s){
		bool has_only_digits = true;
		for (size_t n = 0; n < s.length(); n++)
		{
			if (!isdigit(s[n]))
			{
				has_only_digits = false;
				return false;
			}
		}
		return true;
	}
	bool IS_IN_DT(string st){
		string WORD[] = { "int", "char" };
		for (int i = 0; i < 2; i++){
			if (st == WORD[i]){
				return true;
			}
		}
		return false;
	}
	bool IS_COMMENT(LEX_ANAL st, std::ostream &PARSER){
		if (st.TOK == "#"){
			printTabs(PARSER);
			PARSER << "Comment" << "\t" << lookAhead.LEX << endl;
			return true;
		}
		return false;
	}
	void DL(){
		ofstream PARSER("parse_tree.txt");
		ifstream LEX("lex_output.txt");
		//read first pair
		lookAhead = nextTok(LEX);
		//Declaration List
		printTabs(PARSER);
		PARSER << "Declaration List" << endl;
		tabs++;
		bool SYNTAX_ERROR = true;
		//If a Syntax Error is found, the whole program will shut at the exact moment and that will be it!
		while (!LEX.eof() && SYNTAX_ERROR){
			while (IS_COMMENT(lookAhead, PARSER)){
				match(lookAhead.TOK, LEX);
			}
			if (lookAhead.TOK == "DEF"){
				printTabs(PARSER);
				PARSER << "Declaration" << endl;
				tabs++;
				printTabs(PARSER);
				PARSER << lookAhead.TOK << endl;
				match(lookAhead.TOK, LEX);
				SYNTAX_ERROR = FD(LEX, PARSER); //Function Declaration
				if (SYNTAX_ERROR){
					while (IS_COMMENT(lookAhead, PARSER)){
						match(lookAhead.TOK, LEX);
					}
					if (lookAhead.TOK == "{"){
						printTabs(PARSER);
						PARSER << "Compound Statement" << endl;
						tabs++;
						printTabs(PARSER);
						PARSER << lookAhead.TOK << endl;
						printTabs(PARSER);
						PARSER << "Local Statements List" << endl;
						tabs++;
						match(lookAhead.TOK, LEX);
						while (lookAhead.TOK != "}" && SYNTAX_ERROR){
							SYNTAX_ERROR = IS(SYNTAX_ERROR, LEX, PARSER);
						}
						if (lookAhead.TOK == "}"){
							tabs--;
							tabs--;
							printTabs(PARSER);
							PARSER << lookAhead.TOK << endl;
							match(lookAhead.TOK, LEX);
							if (lookAhead.TOK == "}")
								SYNTAX_ERROR = false;
							else
								SYNTAX_ERROR = true;
						}
					}
				}
			}
			else if (lookAhead.TOK == "INT" || lookAhead.TOK == "CHAR"){
				printTabs(PARSER);
				PARSER << "Declaration" << endl;
				tabs++;
				printTabs(PARSER);
				PARSER << lookAhead.TOK << endl;
				string temp = lookAhead.TOK;
				match(lookAhead.TOK, LEX);
				SYNTAX_ERROR = VD(temp, LEX, PARSER); //Variable Declaration
			}
		}
		if (!SYNTAX_ERROR){
			printTabs(PARSER);
			PARSER << "SYNTAX ERROR" << endl;
		}
	}
	bool FD(std::istream &LEX, std::ostream &PARSER){
		printTabs(PARSER);
		if (IS_COMMENT(lookAhead, PARSER)){
			return false;
		}
		if (lookAhead.TOK == "ID"){
			PARSER << lookAhead.LEX << endl;
			match(lookAhead.TOK, LEX);
			if (lookAhead.TOK == "("){
				printTabs(PARSER);
				PARSER << lookAhead.TOK << endl;
				match(lookAhead.TOK, LEX);
				printTabs(PARSER);
				PARSER << "Parameter List" << endl;
				tabs++;
				if (lookAhead.TOK == ")"){
					printTabs(PARSER);
					PARSER << "VOID" << endl;
					tabs--;
					printTabs(PARSER);
					PARSER << lookAhead.TOK << endl;
					match(lookAhead.TOK, LEX);
					return true;
				}
				else if (VDL(LEX, PARSER)){
					if (lookAhead.TOK == ")"){
						tabs--;
						printTabs(PARSER);
						PARSER << lookAhead.TOK << endl;
						match(lookAhead.TOK, LEX);
						return true;
					}
					else
						return false;
				}
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
		return true;
	}
	bool VDL(std::istream &LEX, std::ostream &PARSER){
		if (lookAhead.TOK == "INT" || lookAhead.TOK == "CHAR"){
			printTabs(PARSER);
			PARSER << lookAhead.TOK << endl;
			match(lookAhead.TOK, LEX);
			BR(LEX, PARSER);
			if (lookAhead.TOK == "ID"){
				printTabs(PARSER);
				PARSER << lookAhead.LEX << endl;
				match(lookAhead.TOK, LEX);
				if (lookAhead.TOK == ","){
					tabs--;
					printTabs(PARSER);
					PARSER << lookAhead.TOK << endl;
					tabs++;
					match(lookAhead.TOK, LEX);
					if (VDL(LEX, PARSER)){
						return true;
					}
					else
						return false;
				}
				else if (lookAhead.TOK == ")"){
					return true;
				}
				else
					return false;
			}
			else
				return false;
		}
	}
	bool VD(string tok, std::istream &LEX, std::ostream &PARSER){
		if (BR(LEX, PARSER)){
			if (lookAhead.TOK == "ID"){
				printTabs(PARSER);
				PARSER << lookAhead.LEX << endl;
				match(lookAhead.TOK, LEX);
				if (lookAhead.TOK == "ID"){
					return false;
				}
				else if (lookAhead.TOK == ","){
					while (lookAhead.TOK == ","){
						tabs--;
						printTabs(PARSER);
						PARSER << lookAhead.TOK << endl;
						tabs++;
						match(lookAhead.TOK, LEX);
						if (lookAhead.TOK == ",")
							return false;
						if (lookAhead.TOK == "ID"){
							printTabs(PARSER);
							PARSER << tok << endl;
							printTabs(PARSER);
							PARSER << lookAhead.LEX << endl;
							match(lookAhead.TOK, LEX);
							
						}
					}
				}
				else if (lookAhead.TOK == "="){
					printTabs(PARSER);
					PARSER << lookAhead.TOK << endl;
					match(lookAhead.TOK, LEX);
					if (lookAhead.TOK == "NUM"){
						printTabs(PARSER);
						PARSER << lookAhead.LEX << endl;
						return true;
					}
					else if (lookAhead.TOK == "CHAR"){
						printTabs(PARSER);
						PARSER << lookAhead.LEX << endl;
						return true;
					}
					else if (lookAhead.TOK == "ID"){
						printTabs(PARSER);
						PARSER << lookAhead.LEX << endl;
						return true;
					}
					else
						return false;
				}
			}
			else
				return false;
		}
		else if(lookAhead.TOK == "ID"){
			printTabs(PARSER);
			PARSER << lookAhead.LEX << endl;
			match(lookAhead.TOK, LEX);
			if (lookAhead.TOK == "ID"){
				return false;
			}
			else if(lookAhead.TOK == ","){
				while (lookAhead.TOK == ","){
					tabs--;
					printTabs(PARSER);
					PARSER << lookAhead.TOK << endl;
					tabs++;
					match(lookAhead.TOK, LEX);
					if (lookAhead.TOK == ",")
						return false;
					if (lookAhead.TOK == "ID"){
						printTabs(PARSER);
						PARSER << tok << endl;
						printTabs(PARSER);
						PARSER << lookAhead.LEX << endl;
						match(lookAhead.TOK, LEX);
					}
				}
				return true;
			}
			else if (lookAhead.TOK == "="){
				printTabs(PARSER);
				PARSER << lookAhead.TOK << endl;
				match(lookAhead.TOK, LEX);
				if (lookAhead.TOK == "NUM"){
					printTabs(PARSER);
					PARSER << lookAhead.LEX << endl;
					return true;
				}
				else if (lookAhead.TOK == "CHAR"){
					printTabs(PARSER);
					PARSER << lookAhead.LEX << endl;
					return true;
				}
				else if (lookAhead.TOK == "ID"){
					printTabs(PARSER);
					PARSER << lookAhead.LEX << endl;
					return true;
				}
				else
					return false;
			}
		}
		else
			return false;
	}
	bool BR(std::istream &LEX, std::ostream &PARSER){
		if (lookAhead.TOK == "["){
			printTabs(PARSER);
			PARSER << lookAhead.TOK << endl;
			match(lookAhead.TOK, LEX);
			if (lookAhead.TOK == "ID"){
				printTabs(PARSER);
				PARSER << lookAhead.LEX << endl;
				match(lookAhead.TOK, LEX);
				if (lookAhead.TOK == "]"){
					printTabs(PARSER);
					PARSER << lookAhead.TOK << endl;
					match(lookAhead.TOK, LEX);
					return true;
				}
				else
					return false;
			}
			else if (lookAhead.TOK == "NUM"){
				printTabs(PARSER);
				PARSER << lookAhead.LEX << endl;
				match(lookAhead.TOK, LEX);
				if (lookAhead.TOK == "]"){
					printTabs(PARSER);
					PARSER << lookAhead.TOK << endl;
					match(lookAhead.TOK, LEX);
					return true;
				}
				else
					return false;
			}
			else if(lookAhead.TOK == "]"){
				printTabs(PARSER);
				PARSER << lookAhead.TOK << endl;
				match(lookAhead.TOK, LEX);
				return true;
			}
			else
				return false;
		}
	}
	bool IS(bool &SYNTAX_ERROR, std::istream &LEX, std::ostream &PARSER){
		bool wow = false;
		while (!LEX.eof() && SYNTAX_ERROR && !wow){
			if (lookAhead.TOK == "WHILE"){
				//tabs--;
				printTabs(PARSER);
				tabs++;
				PARSER << "While Statement" << endl;
				SYNTAX_ERROR = S(LEX, PARSER);
				if (lookAhead.TOK == "{"){
					printTabs(PARSER);
					PARSER << lookAhead.TOK << endl;
					tabs++;
					match(lookAhead.TOK, LEX);
					while (lookAhead.TOK != "}" && SYNTAX_ERROR){
						printTabs(PARSER);
						//tabs++;
						PARSER << "Statement" << endl;
						SYNTAX_ERROR = S(LEX, PARSER);
						if (lookAhead.TOK == "WHILE"){
							SYNTAX_ERROR = IS(SYNTAX_ERROR, LEX, PARSER);
						}
					}
					if (SYNTAX_ERROR && lookAhead.TOK == "}"){
						tabs--;
						printTabs(PARSER);
						PARSER << lookAhead.TOK << endl;
						match(lookAhead.TOK, LEX);
						if (lookAhead.TOK == "}")
							wow = false;
						else
							wow = true;
						
					}
					else
						return false;
				}
			}
			else if (lookAhead.TOK == "IF"){
				printTabs(PARSER);
				PARSER << "If Statement" << endl;
				tabs++;
				SYNTAX_ERROR = S(LEX, PARSER);
				//Check if one lined IF statement
				if (lookAhead.TOK == "ID")
					SYNTAX_ERROR = S(LEX, PARSER);
				//Multi Lined IF Statement
				else if (lookAhead.TOK == "{"){
					printTabs(PARSER);
					PARSER << lookAhead.TOK << endl;
					tabs++;
					match(lookAhead.TOK, LEX);
					while (lookAhead.TOK != "}" && SYNTAX_ERROR){
						printTabs(PARSER);
						//tabs++;
						PARSER << "Statement" << endl;
						SYNTAX_ERROR = S(LEX, PARSER);
						if (lookAhead.TOK == "IF"){
							SYNTAX_ERROR = IS(SYNTAX_ERROR, LEX, PARSER);
						}
					}
					if (SYNTAX_ERROR && lookAhead.TOK == "}"){
						tabs--;
						printTabs(PARSER);
						PARSER << lookAhead.TOK << endl;
						match(lookAhead.TOK, LEX);
						
							wow = true;
					}
					else
						return false;
				}
				//IF ELSE followed IF without any intermediate statement, ERROR occurs
				else if (lookAhead.TOK == "ELSE"){
					wow = true;
				}
				if (lookAhead.TOK == "ELSE"){
					printTabs(PARSER);
					PARSER << lookAhead.TOK << endl;
					tabs++;
					match(lookAhead.TOK, LEX);
					if (lookAhead.TOK == "ID")
						SYNTAX_ERROR = S(LEX, PARSER);
					//Multi Lined IF Statement
					else if (lookAhead.TOK == "{"){
						printTabs(PARSER);
						PARSER << lookAhead.TOK << endl;
						tabs++;
						match(lookAhead.TOK, LEX);
						while (lookAhead.TOK != "}" && SYNTAX_ERROR){
							printTabs(PARSER);
							//tabs++;
							PARSER << "Statement" << endl;
							SYNTAX_ERROR = IS(SYNTAX_ERROR, LEX, PARSER);
							
						}
						if (SYNTAX_ERROR && lookAhead.TOK == "}"){
							tabs--;
							printTabs(PARSER);
							PARSER << lookAhead.TOK << endl;
							match(lookAhead.TOK, LEX);
							if (lookAhead.TOK == "}")
								wow = false;
							else
								wow = true;
						}
						else
							return false;
					}
				}
			}
			else{
			//	tabs--;
				printTabs(PARSER);
			
				PARSER << "Statement" << endl;
				//tabs++;
				SYNTAX_ERROR = S(LEX, PARSER);
			}
		}
		return SYNTAX_ERROR;
	}
	bool S(std::istream &LEX, std::ostream &PARSER){
		int x = 0;
		while (IS_COMMENT(lookAhead, PARSER)){
			match(lookAhead.TOK, LEX);
		}
		if (x == 1){
			return true;
		}
		if (lookAhead.TOK == "INT" || lookAhead.TOK == "CHAR"){
			tabs++;
			printTabs(PARSER);
			PARSER << lookAhead.TOK << endl;
			string temp = lookAhead.TOK;
			match(lookAhead.TOK, LEX);
			return VD(lookAhead.TOK, LEX, PARSER);
		}
		else if (lookAhead.TOK == "ID"){
			tabs++;
			printTabs(PARSER);
			PARSER << lookAhead.LEX << endl;
			match(lookAhead.TOK, LEX);
			BR(LEX, PARSER);
			if (lookAhead.TOK == "="){
				printTabs(PARSER);
				PARSER << lookAhead.TOK << endl;
				match(lookAhead.TOK, LEX);
				if (E(LEX, PARSER)){
					tabs--;
					//tabs--;
					return true;
				}
				else
					return false;
			}
		}
		else if (lookAhead.TOK == "WHILE"){
			printTabs(PARSER);
			PARSER << lookAhead.TOK << endl;
			match(lookAhead.TOK, LEX);
			if (CS(LEX, PARSER)){
				while (IS_COMMENT(lookAhead, PARSER)){
					match(lookAhead.TOK, LEX);
				}
				return true;
			}
			else
				return false;
		}
		else if (lookAhead.TOK == "IF"){
			printTabs(PARSER);
			PARSER << lookAhead.TOK << endl;
			match(lookAhead.TOK, LEX);
			if (CS(LEX, PARSER)){
				tabs++;
				if (S(LEX, PARSER)){
					tabs--;
					return true;
				}
				else
					return false;
			}
			if (lookAhead.TOK == "ELSE"){
				tabs++;
				if (S(LEX, PARSER)){
					tabs--;
					return true;
				}
				else
					return false;
			}
			else
				return true;
		}
	}
	bool E(std::istream &LEX, std::ostream &PARSER){
		if (lookAhead.TOK == "ID"){
			printTabs(PARSER);
			PARSER << lookAhead.LEX << endl;
			match(lookAhead.TOK, LEX);
			BR(LEX, PARSER);
			if (IS_IN_AO(lookAhead.TOK)){
				//tabs++;
				printTabs(PARSER);
				PARSER << lookAhead.TOK << endl;
				match(lookAhead.TOK, LEX);
				BR(LEX, PARSER);
				return E(LEX, PARSER);
			}
			else{
				return true;
			}

		}
		else if (lookAhead.TOK == "NUM" || lookAhead.TOK == "CHAR"){
			printTabs(PARSER);
			PARSER << lookAhead.LEX << endl;
			match(lookAhead.TOK, LEX);
			BR(LEX, PARSER);
			if (IS_IN_AO(lookAhead.TOK)){
				tabs++;
				printTabs(PARSER);
				PARSER << lookAhead.LEX << endl;
				match(lookAhead.TOK, LEX);
				BR(LEX, PARSER);
				return E(LEX, PARSER);
			}
			else{
				return true;
			}
		}
	}
	bool CS(std::istream &LEX, std::ostream &PARSER){
		if (lookAhead.TOK == "ID" || lookAhead.TOK == "NUM" || lookAhead.TOK == "CHAR"){
			printTabs(PARSER);
			PARSER << lookAhead.LEX << endl;
			match(lookAhead.TOK, LEX);
			if (isRO()){
				printTabs(PARSER);
				PARSER << lookAhead.LEX << endl;
				match(lookAhead.TOK, LEX);
				if (lookAhead.TOK == "ID" || lookAhead.TOK == "NUM" || lookAhead.TOK == "CHAR"){
					printTabs(PARSER);
					PARSER << lookAhead.LEX << endl;
					match(lookAhead.TOK, LEX);
					BR(LEX, PARSER);
					return true;
				}
				else
					return false;
			}
			else if (BR(LEX, PARSER)){
				if (isRO()){
					printTabs(PARSER);
					PARSER << lookAhead.LEX << endl;
					match(lookAhead.TOK, LEX);
					if (lookAhead.TOK == "ID" || lookAhead.TOK == "NUM" || lookAhead.TOK == "CHAR"){
						printTabs(PARSER);
						PARSER << lookAhead.LEX << endl;
						match(lookAhead.TOK, LEX);
						BR(LEX, PARSER);
						return true;
					}
					else
						return false;
				}
			}
		}
		return false;
	}
};