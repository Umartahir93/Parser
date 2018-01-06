#include "CompilerConstruction.h"



CompilerConstruction::CompilerConstruction()
{
	ids = "";


}


CompilerConstruction::~CompilerConstruction()
{
}

bool CompilerConstruction::TOKEN_MAKER(string& lex, LEX& TOK_LEX)
{

	bool r_val = false;
	string WORD[] = { "int", "char", "+", "-", "*", "/", "=", "<", "<=", ">", ">=", "==", "{", "}", "[", "]", "def", "while", "if", "else", "(", ")", "," };
	string TOKENS[] = { "INT", "CHAR", "+", "-", "*", "/", "=", "RO", "RO", "RO", "RO", "RO", "{", "}", "[", "]", "DEF", "WHILE", "IF", "ELSE", "(", ")", "," };
	string LEXEMES[] = { "null", "null", "null", "null","null", "null", "null", "LT", "LE", "GT", "GE", "EE", "null", "null", "null",
		"null", "null", "null", "null", "null", "null", "null", "null" };
	for (int i = 0; i < 23; i++) {
		if (lex == WORD[i]) {
			TOK_LEX.LEXEME = LEXEMES[i];
			TOK_LEX.TOKEN = TOKENS[i];
			r_val = true;
			break;
		}
	}
	return r_val;

}

bool CompilerConstruction::SYMBOL_IDENTIFIER(string lex) {
	string KEYWORDS[7] = { "def", "if", "else", "while", "return", "print", "get" };
	int checker = 0;
	int i = 0;
	for (int i = 0; i < 7; ++i) {
		if (lex == KEYWORDS[i]) {
			return true;
		}
	}
	return false;
}

bool CompilerConstruction::IS_IN(string a, string b[], int l) {
	for (int i = 0; i < l; i++) {
		if (a == b[i]) {
			return true;
		}
	}
	return false;
}

bool CompilerConstruction::IS_IN_NL(char a) {
	string NONLETTERS[20] = { "=", ",", "+", "-", "*", "/", "<", "<=", ">", ">=", "==", "{", "}", "[", "]", "(", ")", " ","\t","\n" };
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < NONLETTERS[i].size(); j++) {
			if (a == NONLETTERS[i][j]) {
				return true;
			}
		}
	}
	return false;
}

bool CompilerConstruction::IS_IN_D(char a) {
	if (a > 48 && a < 59) {
		return true;
	}
	return false;
}

bool CompilerConstruction::IS_IN_D(string& s) {
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

bool CompilerConstruction::IS_IN_DT(string st) {
	string WORD[] = { "int", "char" };
	for (int i = 0; i < 2; i++) {
		if (st == WORD[i]) {
			return true;
		}
	}
	return false;
}


bool CompilerConstruction::IS_COMMENT(string st) {
	if (st[0] == '#') {
		return true;
	}
	return false;
}


bool CompilerConstruction::IS_IN_AO(char a) {
	string NONLETTERS[5] = { "*", "/", "+", "-" };
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < NONLETTERS[i].size(); j++) {
			if (a == NONLETTERS[i][j]) {
				return true;
			}
		}
	}
	return false;
}

int CompilerConstruction::lexical_output(int argc, char* argv[])
{
//	int check = 0;

	if (argc == 0)
	{
		cout << "You have not given command line arguments";
		system("pause");
		return 0;
	}
	string input_file = argv[1];
	ifstream prog(input_file); //the RUBY++ code
	ofstream ofs;
	ofstream lex("lex_output.txt");
	ofstream s_table("SymbolTable.txt");

	s_table << "All the Symbols are here: \n \n";
	//lex << "Token \t \t Lexeme \n" << endl;
	LEX LEX_TOK;

	string DIGITS[10] = { "1", "2", "3", "4", "5", "6", "7", "8","9", "0" };
	string DATA, temp = "", temp2 = "";

	LEX Lex;

	if (!prog.is_open()) {
		cout << "Could not open file(s) \n" << endl;
	}
	else {

		while (!prog.eof()) {

			getline(prog, DATA);
			for (int i = 0; i < DATA.size(); i++) {
				if (DATA[i] == ' ' || DATA[i] == '\t' || DATA[i] == '\n') {
					temp.clear();
					//skip spaces
				}
				//read comments
				else if (DATA[i] == '#') {
					i++;
					while (i < DATA.size()) {
						temp += DATA[i];
						i++;
					}
					lex << "#" << "\t \t" << temp << endl;
					break;
				}
				//read RO
				else if (DATA[i] == '=' || DATA[i] == '<' || DATA[i] == '>') {
					if (DATA[i + 1] == '=') {
						temp += DATA[i];
						temp += DATA[++i];
						TOKEN_MAKER(temp, LEX_TOK);
						lex << LEX_TOK.TOKEN << "\t \t" << LEX_TOK.LEXEME << endl;
						LEX_TOK.TOKEN.clear();
						LEX_TOK.LEXEME.clear();
						temp.clear();
					}
					else {
						temp += DATA[i];
						TOKEN_MAKER(temp, LEX_TOK);
						lex << LEX_TOK.TOKEN << "\t \t" << LEX_TOK.LEXEME << endl;
						LEX_TOK.TOKEN.clear();
						LEX_TOK.LEXEME.clear();
						temp.clear();
					}
				}
				//Read Brackets
				else if (DATA[i] == '(') {
					lex << "(" << "\t \t" << "null" << endl;
				}
				else if (DATA[i] == ')') {
					lex << ")" << "\t \t" << "null" << endl;
				}
				else if (DATA[i] == '{') {
					lex << "{" << "\t \t" << "null" << endl;
				}
				else if (DATA[i] == '}') {
					lex << "}" << "\t \t" << "null" << endl;
				}
				else if (DATA[i] == ',') {
					lex << "," << "\t \t" << "null" << endl;
				}
				else if (DATA[i] == '[') {
					lex << "[" << "\t \t" << "null" << endl;
				}
				else if (DATA[i] == ']') {
					lex << "]" << "\t \t" << "null" << endl;
				}
				else if (IS_IN_AO(DATA[i])) {
					temp += DATA[i];
					TOKEN_MAKER(temp, LEX_TOK);
					lex << LEX_TOK.TOKEN << "\t \t" << LEX_TOK.LEXEME << endl;
					temp.clear();
					LEX_TOK.LEXEME.clear();
					LEX_TOK.TOKEN.clear();
				}
				else {
					while (!IS_IN_NL(DATA[i]) && i < DATA.size()) {
						temp += DATA[i];
						i++;
					}
					if (i < DATA.size())
						i--;
					if (IS_IN_D(temp)) {
						lex << "NUM" << "\t \t" << temp << endl;
					}
					else if (IS_IN_DT(temp)) {
						if (temp == "int") {
							lex << "INT" << "\t \t" << "NULL" << endl;
						}
						else {
							lex << "CHAR" << "\t \t" << "NULL" << endl;
						}
					}
					else if (SYMBOL_IDENTIFIER(temp)) {
						if (TOKEN_MAKER(temp, LEX_TOK))
							lex << LEX_TOK.TOKEN << "\t \t" << LEX_TOK.LEXEME << endl;
					}
					else {
						lex << "ID" << "\t \t" << temp << endl;
					}
					temp.clear();
					LEX_TOK.LEXEME.clear();
					LEX_TOK.TOKEN.clear();
				}
			}
			temp.clear();
		}

		cout << DATA << endl;
		prog.close();
	}

	find_sentence();

	//cout << "The token lexeme and symbol file is created" << endl;

	//system("pause");

}



void CompilerConstruction::find_sentence()
{
	
	Parser obj;
	obj.startParser();
	system("Pause");
}