#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "Parser.h"

using namespace std;

Parser::Parser() {
	ifstream fin;
	fin.open("words.txt");

	
	fout.open("parse-tree.txt");

	if (!fin.is_open()) {
		cout << "Can't open file" << endl;
		exit(EXIT_FAILURE);
	}

	while (!fin.eof()) {
		Token* token = new Token();
		getline(fin, token->type, '\t');
		getline(fin, token->lexeme, '\n');
		tokens.push_back(token);
	}

	index = 0;
	look = nextTok();
	P();
}

void Parser::P() {
	if (index >= tokens.size())
		return;
	fout << "P :\n" << "\tD\n" << "\tP\n"  << endl;
	D();
	P();
}

void Parser::D() {
	fout << "D :\n" << "\tVD\n" << "\tFD\n" << endl;
	VD();
	FD();
}

void Parser::FD() {
	if (look->type == "def") {
		match("def");
		
		//add entity in symbol table
		entity = new Entity();
		entity->dataType = "fun";
		entity->id = look->lexeme;
		addEntity(entity);

		fout << "FD :\n" << "\tdef\n" << "\tid (" << look->lexeme << ")\n" << "\t(OPL)\n" << "\t{SL}\n" << endl;


		match("id");
		match("(");
		OPL();
		match(")");
		match("{");
		SL();
		match("}");
	}
}

void Parser::SL() {
	fout << "SL :\n" << "\tS\n" << "\tSL\n" << endl;

	S();
	if (look->type != "}")
		SL();
}

void Parser::S() {
	fout << "S :\n" << "\tVD\n" << "\tIN\n" << "\tOUT\n" << "\tA\n" << "\tCOND\n" << "\tL\n" << "\tRET\n" << endl;
	VD();
	IN();
	OUT();
	A();
	COND();
	L();
	RET();
}

void Parser::OPL() {
	fout << "OPL :\n" << "\tPL\n" << endl;
	PL();
}

void Parser::PL() {
	fout << "PL :\n" << "\tPAR\n" << "\tR1\n" << endl;
	PAR();
	R1();
}

void Parser::R1() {
	if (look->type == ",") {
		fout << "R1 :\n" << "\t,\n" << "\tPAR\n" << "\tR1\n" << endl;
		match(",");
		PAR();
		R1();
	}
}

void Parser::PAR() {
	string s = look->type;
	if (look->type == "int" || look->type == "char") {

		T();

		fout << "PAR :\n" << "\tT\n" << "\tid (" << look->lexeme << ")\n" << endl;
		fout << "T :\n" << "\t" << s << '\n' << endl;

		//add entity in symbol table
		entity = new Entity();
		entity->dataType = s;
		entity->id = look->lexeme;
		addEntity(entity);
		
		match("id");
	}
}

void Parser::T() {
	if (look->type == "int") 
		match("int");
		
	else if (look->type == "char")
		match("char");
}

void Parser::VD() {
	string s = look->type;
	if (look->type == "int" || look->type == "char") {
		T();
		fout << "VD :\n" << "\tT\n" << "\tid (" << look->lexeme << ")\n" << "\tR2\n" << "\t;\n" << endl;
		fout << "T :\n" << "\t" << s << '\n' << endl;

		//add entity in symbol table
		entity = new Entity();
		entity->dataType = s;
		entity->id = look->lexeme;
		addEntity(entity);

		match("id");
		R2();
		match(";");
	}
}

void Parser::R2() {
	string s = entity->dataType;
	if (look->type == ",") {
		match(",");
		fout << "R2 :\n" << "\t,\n" << "\tid (" << look->lexeme << ")\n" << "\tR2\n" << endl;

		//add entity in symbol table
		entity = new Entity();
		entity->dataType = s;
		entity->id = look->lexeme;
		addEntity(entity);
		
		match("id");
		R2();
	}
}

void Parser::IN() {
	if (look->type == "read") {
		fout << "IN :\n" << "\tread\n" << "\tid (" << look->lexeme << ")\n" << "\t;\n" << endl;

		match("read");
		match("id");
		match(";");
	}
}

void Parser::OUT() {
	if (look->type == "print") {
		fout << "OUT :\n" << "\tprint\n" << "\tCH1\n" << "\t;\n" << endl;
		match("print");
		CH1();       
		match(";");
	}
}

void Parser::CH1() {
	if (look->type == "id") {
		fout << "CH1 :\n" << "\tid (" << look->lexeme << ")\n" << endl;
		match("id");
	}
	else if (look->type == "nc") {
		fout << "CH1 :\n" << "\tnc (" << look->lexeme << ")\n" << endl;
		match("nc");
	}
	else if (look->type == "lc") {
		fout << "CH1 :\n" << "\tlc (" << look->lexeme << ")\n" << endl;
		match("lc");
	}
	else if (look->type == "sc") {
		fout << "CH1 :\n" << "\tsc (" << look->lexeme << ")\n" << endl;
		match("sc");
	}
		

}

void Parser::A() {
	if (look->type == "id") {
		fout << "A :\n" << "\tid (" << look->lexeme << ")\n" << "\t<-\n" << "\tAE\n" << "\t;\n" << endl;
		match("id");
		match("<-");
		AE();
		match(";");
	}
}

void Parser::COND() {
	if (look->type == "if") {
		fout << "COND :\n" << "\tif\n" << "\t(BE)\n" << "\tCH2\n" << "\tELSE\n" << endl;
		match("if");
		match("(");
		BE();
		match(")");
		CH2();
		ELSE();
	}
}

void Parser::ELSE() {
	if (look->type == "else") {
		fout << "ELSE :\n" << "\telse\n" << "\tCH2\n" << endl;
		match("else");
		CH2();
	}
}

void Parser::CH2() {
	fout << "CH2 :\n" << "\t{SL}\n" << "\tS\n" << endl;
	if (look->type == "{") {
		match("{");
		SL();
		match("}");
	}
	else {
		S();
	}
}

void Parser::L() {
	if (look->type == "while") {
		fout << "L :\n" << "\twhile\n" << "\t(BE)\n" << "\tCH2\n" << endl;

		match("while");
		match("(");
		BE();
		match(")");
		CH2();
	}
}

void Parser::RET() {
	if (look->type == "ret") {
		fout << "RET :\n" << "\tret\n" << "\tCH1\n" << "\t;\n" << endl;
		match("ret");
		CH1();
		match(";");
	}
}

void Parser::AE() {
	fout << "AE :\n" << "\tAE1\n" << "\tR3\n" << endl;

	AE1();
	R3();
}

void Parser::R3() {
	if (look->type == "+") {
		fout << "R3 :\n" << "\t+\n" << "\tAE1\n" << "\tR3\n" << endl;

		match("+");
		AE1();
		R3();
	}
	else if (look->type == "-") {
		fout << "R3 :\n" << "\t-\n" << "\tAE1\n" << "\tR3\n" << endl;

		match("-");
		AE1();
		R3();
	}
}

void Parser::AE1() {
	fout << "AE1 :\n" << "\tAE2\n" << "\tR4\n" << endl;

	AE2();
	R4();
}

void Parser::R4() {
	if (look->type == "*") {
		fout << "R4 :\n" << "\t*\n" << "\tAE2\n" << "\tR4\n" << endl;

		match("*");
		AE2();
		R4();
	}
	else if (look->type == "/") {
		fout << "R4 :\n" << "\t/\n" << "\tAE2\n" << "\tR4\n" << endl;

		match("/");
		AE2();
		R4();
	}
}

void Parser::AE2() {
	if (look->type == "id") {
		fout << "AE2 :\n" << "\tid (" << look->lexeme << ")\n" << endl;
		match("id");
	}
	else if (look->type == "nc") {
		fout << "AE2 :\n" << "\tnc (" << look->lexeme << ")\n" << endl;
		match("nc");
	}
	else if (look->type == "lc") {
		fout << "AE2 :\n" << "\tlc (" << look->lexeme << ")\n" << endl;
		match("lc");
	}
	else if (look->type == "(") {
		fout << "AE2 :\n" << "\t(AE)\n" << endl;

		match("(");
		AE();
		match(")");
	}
}

void Parser::BE() {
	if (look->type == "id" || look->type == "nc" || look->type == "lc") {
		fout << "BE :\n" << "\tCH3\n" << "\tro\n" << "\tCH3\n" << endl;

		CH3();
		match("ro");
		CH3();
	}
}

void Parser::CH3() {
	if (look->type == "id") {
		fout << "CH3 :\n" << "\tid (" << look->lexeme << ")\n" << endl;
		match("id");
	}
	else if (look->type == "nc") {
		fout << "CH3 :\n" << "\tnc (" << look->lexeme << ")\n" << endl;
		match("nc");
	}
	else if (look->type == "lc") {
		fout << "CH3 :\n" << "\tlc (" << look->lexeme << ")\n" << endl;
		match("nc");
	}
}

void Parser::match(string s) {
	if (look->type == s)
		look = nextTok();
	else {
		cout << "Bad Token" << endl;
		cout << "Token Number : " << index << endl;
		exit(EXIT_FAILURE);
	}
}

Token* Parser::nextTok() {
	if (index > tokens.size()) {
		cout << "Bad Token" << endl;
		cout << "Index of bound "<< endl;
		exit(EXIT_FAILURE);
	}
	else if (index == tokens.size()) {
		Token *t = new Token();
		t->type = "^";
		t->lexeme = "^";
		return t;
	}
	else 
		return tokens[index++];
}

void Parser::addEntity(Entity *e) {
	symbolTable.push_back(e);
}

void Parser::printSymbolTable() {
	ofstream fout;
	fout.open("parser-symboltable.txt");
	for (int i = 0; i < symbolTable.size(); i++) {
		fout << symbolTable[i]->id << '\t' << symbolTable[i]->dataType << endl;
	}
	fout.close();
}
