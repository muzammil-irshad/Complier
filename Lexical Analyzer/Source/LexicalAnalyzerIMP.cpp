#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "LexicalAnalyzer.h"


using namespace std;

LexicalAnalyzer::LexicalAnalyzer() {
}

string LexicalAnalyzer::getSubStr(int start, int end, string str) {
	string ss;
	for (int i = start; i <= end; ++i) {
		ss = ss + str[i];
	}
	return ss;
}


void LexicalAnalyzer::tokenize(string filename) {
	string str;
	string temp;
	string lex;
	Token *token;
	ifstream fin;
	int line = 1;
	bool singleMultiLineCmnt = false;

	fin.open(filename);
	if (!fin.is_open()) {
		cout << "Can't open file" << endl;
		exit(EXIT_FAILURE);
	}
	while (!fin.eof()) {
		getline(fin, str, '\n');
		str.append("\n"); //to get complete line

		//tokenize the string
		int i = 0, j = 0;
		for (; i < str.size() && j < str.size();) {
			temp = getSubStr(i, j, str);

			if (rules.isDataType(temp)) {
				temp.resize(temp.size() - 1); //remove other
				token = new Token();
				token->type = temp;
				token->lexeme = "^";
				tokens.push_back(token);
				i = j;
			}

			else if (rules.isKeyword(temp)) {
				temp.resize(temp.size() - 1); //remove other
				token = new Token();
				token->type = temp;
				token->lexeme = "^";
				tokens.push_back(token);
				i = j;
			}


			else if (rules.isArithmeticOperator(temp)) {
				temp.resize(temp.size() - 1); //remove other
				token = new Token();
				token->type = temp;
				token->lexeme = "^";
				tokens.push_back(token);
				i = j;
			}

			else if (rules.isRelationalOperator(temp, lex)) {
				temp.resize(temp.size() - 1); //remove other
				token = new Token();
				token->type = "RO";
				token->lexeme = lex;
				tokens.push_back(token);
				i = j;
			}

			else if (rules.isIdentifier(temp)) {
				temp.resize(temp.size() - 1); //remove other
				token = new Token();
				token->type = "ID";
				token->lexeme = temp;
				tokens.push_back(token);
				i = j;
			}

			else if (rules.isNumericConstant(temp)) {
				temp.resize(temp.size() - 1); //remove other
				token = new Token();
				token->type = "NC";
				token->lexeme = temp;
				tokens.push_back(token);
				i = j;
			}

			else if (rules.isLiteralConstant(temp)) {
				temp.resize(temp.size() - 1); //remove other
				token = new Token();
				token->type = "LC";
				token->lexeme = temp;
				tokens.push_back(token);
				i = j;
			}

			else if (rules.isStringConstant(temp)) {
				temp.resize(temp.size() - 1); //remove other
				token = new Token();
				token->type = "SC";
				token->lexeme = temp;
				tokens.push_back(token);
				i = j;
			}

			else if (rules.isAssignmentOperator(temp)) {
				temp.resize(temp.size() - 1); //remove other
				token = new Token();
				token->type = temp;
				token->lexeme = "^"; //Assignment operator
				tokens.push_back(token);
				i = j;
			}

			else if (rules.isSymbol(temp)) {
				temp.resize(temp.size() - 1); //remove other
				token = new Token();
				token->type = temp;
				token->lexeme = "^";
				tokens.push_back(token);
				i = j;
			}

			//remove tab
			if (i == j && str[i] == '\t') {
				i++;
				j++;
			}

			//remove \n
			else if (i == j && str[i] == '\n') {
				i++;
				j++;
			}

			//remove spaces
			else if (i == j && str[i] == ' ') {
				i++;
				j++;
			}

			//remove single line comment   //## This would be a single line comment ## int abc;
			else if (str[i] == '#') {
				// remove multiple lines comment
				singleMultiLineCmnt = false;
				if (i + 1 < str.size() && str[i + 1] == '#') {
					//## This is single multi line comment ##
					int count = 0;
					int k = i + 2;
					for (; k < str.size() && count < 2; ++k) {
						if (str[k] == '#')
							count++;
					}
					if (count == 2) {
						i = k;
						j = k;
						singleMultiLineCmnt = true;
					}

					else {
						while (!fin.eof() && fin.get() != '#') {}
						if (fin.eof() || fin.get() != '#') {
							j = i + 1; //to point out error
							break;
						}
					}
				}
				if (!singleMultiLineCmnt) {
					//to exit from the loop in case of single line comment
					i = str.size();
					j = str.size();

				}
			}

			else
				j++;
		}

		if (i < j) {
			cout << "Error: line " << line << endl;
			cout << "Press enter to continue.." << endl;
			cin.get();
		}
		line++;
	}
	fin.close();
}

void LexicalAnalyzer::printTokens() {
	ofstream fout;
	fout.open("words.txt");
	for (int i = 0; i < tokens.size(); i++) {
		fout << tokens[i]->type << "	" << tokens[i]->lexeme << endl;
	}
	fout.close();
}

bool LexicalAnalyzer::repeat(string str) {
	for (int i = 0; i < identifiers.size(); ++i) {
		if (str == identifiers[i])
			return true;
	}
	identifiers.push_back(str);
	return false;
}

void LexicalAnalyzer::printSymbolTable() {
	ofstream fout;
	fout.open("symboltable.txt");
	for (int i = 0; i < tokens.size(); i++) {
		if (tokens[i]->type == "ID" && !repeat(tokens[i]->lexeme))
			fout << tokens[i]->lexeme << endl;
	}
	fout.close();
}
