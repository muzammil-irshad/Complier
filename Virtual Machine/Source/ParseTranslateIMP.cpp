#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "ParseTranslate.h"

using namespace std;

ParseTranslate::ParseTranslate() {
	ifstream fin;
	fin.open("words.txt");

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
	fin.close();

	index = 0;
	opCount = 0;
	tempCount = 0;
	h2l = false;
	look = nextTok();
	
	//start ParseTranslate
	printTree.open("parse-tree.txt");
	P();
	printTree.close();
}

void ParseTranslate::P() {
	if (index >= tokens.size())
		return;

	printTree << "P :\n" << "\tD\n" << "\tP\n"  << endl;
	
	D();
	P();
}

void ParseTranslate::D() {
	printTree << "D :\n" << "\tVD\n" << "\tFD\n" << endl;
	
	VD();
	FD();
}

void ParseTranslate::FD() {
	if (look->type == "def") {
		match("def");
		
		printTree << "FD :\n" << "\tdef\n" << "\tid (" << look->lexeme << ")\n" << "\t(OPL)\n" << "\t{SL}\n" << endl;

		//add entity in symbol table
		entity = new Entity();
		entity->dataType = "fun";
		entity->id = look->lexeme;
		entity->value = "^";
		addEntity(entity);

		//generate three Address Code
		statement = new string("START FUNC");
		threeAddressCode.push_back(statement);

		string s = look->lexeme; //save function name

		match("id");
		match("(");
		OPL();

		statement = new string(s + ", " + to_string(opCount));
		threeAddressCode.push_back(statement);

		match(")");
		match("{");
		SL();
		match("}");

		statement = new string("END FUNC");
		threeAddressCode.push_back(statement);
	}
}

void ParseTranslate::SL() {
	printTree << "SL :\n" << "\tS\n" << "\tSL\n" << endl;

	S();
	if (look->type != "}")
		SL();
}

void ParseTranslate::S() {
	printTree << "S :\n" << "\tVD\n" << "\tIN\n" << "\tOUT\n" << "\tA\n" << "\tCOND\n" << "\tL\n" << "\tRET\n" << endl;
	
	VD();
	IN();
	OUT();
	A();
	COND();
	L();
	RET();
}

void ParseTranslate::OPL() {
	printTree << "OPL :\n" << "\tPL\n" << endl;
	
	PL();
}

void ParseTranslate::PL() {
	printTree << "PL :\n" << "\tPAR\n" << "\tR1\n" << endl;
	
	PAR();
	
	//count parameters
	opCount++;
	
	R1();
}

void ParseTranslate::R1() {
	if (look->type == ",") {
		printTree << "R1 :\n" << "\t,\n" << "\tPAR\n" << "\tR1\n" << endl;
		
		match(",");
		PAR();
		
		//count parameters
		opCount++;
		
		R1();
	}
}

void ParseTranslate::PAR() {
	if (look->type == "int" || look->type == "char") {
		entity = new Entity();

		T();

		printTree << "PAR :\n" << "\tT\n" << "\tid (" << look->lexeme << ")\n" << endl;
		printTree << "T :\n" << "\t" << entity->dataType << '\n' << endl;

		entity->id = look->lexeme;
		addEntity(entity);
		
		//Assume function parameter are input from user
		statement = new string("read " + look->lexeme);
		threeAddressCode.push_back(statement);

		match("id");
	}
}

void ParseTranslate::T() {
	if (look->type == "int") {
		match("int");

		entity->dataType = "int";
		entity->value = "0";
	}
		
	else if (look->type == "char") {
		match("char");

		entity->dataType = "char";
		entity->value = "a";
	}
}

void ParseTranslate::VD() {
	if (look->type == "int" || look->type == "char") {
		entity = new Entity();

		T();

		printTree << "VD :\n" << "\tT\n" << "\tid (" << look->lexeme << ")\n" << "\tR2\n" << "\t;\n" << endl;
		printTree << "T :\n" << "\t" << entity->dataType << '\n' << endl;

		entity->id = look->lexeme;
		addEntity(entity);

		statement = new string(look->lexeme);
		threeAddressCode.push_back(statement);

		match("id");
		R2();
		match(";");
	}
}

void ParseTranslate::R2() {
	if (look->type == ",") {
		match(",");

		printTree << "R2 :\n" << "\t,\n" << "\tid (" << look->lexeme << ")\n" << "\tR2\n" << endl;

		string type = entity->dataType;
		string value = entity->value;

		entity = new Entity();
		entity->dataType = type;
		entity->id = look->lexeme;
		entity->value = value;
		addEntity(entity);
		
		
		statement = new string(look->lexeme);
		threeAddressCode.push_back(statement);

		match("id");
		R2();
	}
}

void ParseTranslate::IN() {
	if (look->type == "read") {

		printTree << "IN :\n" << "\tread\n" << "\tid (" << look->lexeme << ")\n" << "\t;\n" << endl;

		match("read");

		//generate three address code
		statement = new string("read " + look->lexeme);
		threeAddressCode.push_back(statement);

		match("id");
		match(";");
	}
}

void ParseTranslate::OUT() {
	if (look->type == "print") {	
		printTree << "OUT :\n" << "\tprint\n" << "\tCH1\n" << "\t;\n" << endl;
		
		match("print");
		
		statement = new string("print " + look->lexeme);
		threeAddressCode.push_back(statement);

		CH1();       
		match(";");
	}
}

void ParseTranslate::CH1() {
	if (look->type == "id") {
		printTree << "CH1 :\n" << "\tid (" << look->lexeme << ")\n" << endl;
		match("id");
	}
	else if (look->type == "nc") {
		printTree << "CH1 :\n" << "\tnc (" << look->lexeme << ")\n" << endl;
		match("nc");
	}
	else if (look->type == "lc") {
		printTree << "CH1 :\n" << "\tlc (" << look->lexeme << ")\n" << endl;
		match("lc");
	}
	else if (look->type == "sc") {
		printTree << "CH1 :\n" << "\tsc (" << look->lexeme << ")\n" << endl;
		match("sc");
	}
}

void ParseTranslate::A() {
	if (look->type == "id") {
		printTree << "A :\n" << "\tid (" << look->lexeme << ")\n" << "\t<-\n" << "\tAE\n" << "\t;\n" << endl;
		
		statement = new string(look->lexeme + " = " + "t" + to_string(tempCount));
		string *s = statement;
		
		//info about temp variable
		int count = tempCount;
		string type = getType(look->lexeme);

		match("id");
		match("<-");
		AE();

		threeAddressCode.push_back(s);

		if (type == "int")
			addTempVar(count, type, "0");
		else
			addTempVar(count, type, "a");

		match(";");
	}
}

void ParseTranslate::COND() {
	if (look->type == "if") {
		printTree << "COND :\n" << "\tif\n" << "\t(BE)\n" << "\tCH2\n" << "\tELSE\n" << endl;
		
		match("if");
		match("(");
		
		statement = new string("if ");
		
		BE();
		
		statement->append(" goto " + to_string(threeAddressCode.size() + 2));
		threeAddressCode.push_back(statement);
		
		statement = new string("goto");
		threeAddressCode.push_back(statement);

		match(")");
		CH2();
		ELSE();
	}
}

void ParseTranslate::ELSE() {
	if (look->type == "else") {
		
		backPatch(threeAddressCode.size(), threeAddressCode.size() + 1);

		statement = new string("goto");
		threeAddressCode.push_back(statement);
		
		printTree << "ELSE :\n" << "\telse\n" << "\tCH2\n" << endl;
		match("else");
		CH2();

		backPatch(threeAddressCode.size(), threeAddressCode.size());
	}
	backPatch(threeAddressCode.size(), threeAddressCode.size());
}

void ParseTranslate::CH2() {
	printTree << "CH2 :\n" << "\t{SL}\n" << "\tS\n" << endl;
	if (look->type == "{") {
		match("{");
		SL();
		match("}");
	}
	else {
		S();
	}
}

void ParseTranslate::L() {
	if (look->type == "while") {
		printTree << "L :\n" << "\twhile\n" << "\t(BE)\n" << "\tCH2\n" << endl;

		match("while");
		match("(");
		
		int line = threeAddressCode.size();

		statement = new string("if ");
		
		BE();
		
		statement->append(" goto " + to_string(threeAddressCode.size() + 2));
		threeAddressCode.push_back(statement);

		statement = new string("goto");
		threeAddressCode.push_back(statement);
		
		match(")");
		CH2();
		
		statement = new string("goto " + to_string(line));
		threeAddressCode.push_back(statement);
		
		backPatch(threeAddressCode.size(), threeAddressCode.size());
	}
}

void ParseTranslate::RET() {
	if (look->type == "ret") {
		printTree << "RET :\n" << "\tret\n" << "\tCH1\n" << "\t;\n" << endl;
		
		match("ret");
		
		statement = new string("ret " + look->lexeme);
		threeAddressCode.push_back(statement);

		CH1();
		match(";");
	}
}

void ParseTranslate::AE() {
	printTree << "AE :\n" << "\tAE1\n" << "\tR3\n" << endl;
	
	h2l = false;
	
	AE1();
	R3();
}

void ParseTranslate::R3() {
	if (look->type == "+") {
		printTree << "R3 :\n" << "\t+\n" << "\tAE1\n" << "\tR3\n" << endl;
		
		string *s;
		if (h2l) {
			string *temp = new string (*threeAddressCode[threeAddressCode.size() - 1]);
			temp->erase(temp->find(" "), temp->size());
			s = new string(*temp + " = " + *temp);
			h2l = false;
		}
		else {
			s = threeAddressCode[threeAddressCode.size() - 1];
			threeAddressCode.pop_back();
		}

		match("+");

		s->append(" + ");
		s->append("t" + to_string(tempCount));
		
		AE1();
		R3();

		threeAddressCode.push_back(s);
	}
	else if (look->type == "-") {
		printTree << "R3 :\n" << "\t-\n" << "\tAE1\n" << "\tR3\n" << endl;

		string *s;
		if (h2l) {
			string *temp = new string (*threeAddressCode[threeAddressCode.size() - 1]);
			temp->erase(temp->find(" "), temp->size());
			s = new string(*temp + " = " + *temp);
			h2l = false;
		}
		else {
			s = threeAddressCode[threeAddressCode.size() - 1];
			threeAddressCode.pop_back();
		}

		match("-");

		s->append(" - ");
		s->append("t" + to_string(tempCount));

		AE1();
		R3();

		threeAddressCode.push_back(s);
	}
}

void ParseTranslate::AE1() {
	printTree << "AE1 :\n" << "\tAE2\n" << "\tR4\n" << endl;

	AE2();
	R4();
}

void ParseTranslate::R4() {
	if (look->type == "*") {
		printTree << "R4 :\n" << "\t*\n" << "\tAE2\n" << "\tR4\n" << endl;

		h2l = true;
		string *s = threeAddressCode[threeAddressCode.size() - 1];
		threeAddressCode.pop_back();


		match("*");
		
		s->append(" * ");
		s->append("t" + to_string(tempCount));

		AE2();
		R4();

		threeAddressCode.push_back(s);
	}
	else if (look->type == "/") {
		printTree << "R4 :\n" << "\t/\n" << "\tAE2\n" << "\tR4\n" << endl;

		h2l = true;
		string *s = threeAddressCode[threeAddressCode.size() - 1];
		threeAddressCode.pop_back();


		match("/");

		s->append(" / ");
		s->append("t" + to_string(tempCount));

		AE2();
		R4();

		threeAddressCode.push_back(s);
	}
}

void ParseTranslate::AE2() {
	if (look->type == "id") {
		
		printTree << "AE2 :\n" << "\tid (" << look->lexeme << ")\n" << endl;
		
		statement = new string("t" + to_string(tempCount++) + " = " + look->lexeme);
		threeAddressCode.push_back(statement);
		
		match("id");
	}
	else if (look->type == "nc") {
		printTree << "AE2 :\n" << "\tnc (" << look->lexeme << ")\n" << endl;
		
		statement = new string("t" + to_string(tempCount++) + " = " + look->lexeme);
		threeAddressCode.push_back(statement);
		
		match("nc");
	}
	else if (look->type == "lc") {
		printTree << "AE2 :\n" << "\tlc (" << look->lexeme << ")\n" << endl;
		
		statement = new string("t" + to_string(tempCount++) + " = " + look->lexeme);
		threeAddressCode.push_back(statement);
		
		match("lc");
	}
	else if (look->type == "(") {
		printTree << "AE2 :\n" << "\t(AE)\n" << endl;
		
		bool prev_state = h2l;

		match("(");
		AE();
		match(")");

		h2l = prev_state;

		string *temp = new string(*threeAddressCode[threeAddressCode.size() - 1]);
		temp->erase(temp->find(" "), temp->size());
		statement = new string(*temp + " = " + *temp);
		threeAddressCode.push_back(statement);
	}
}

void ParseTranslate::BE() {
	if (look->type == "id" || look->type == "nc" || look->type == "lc") {
		
		printTree << "BE :\n" << "\tCH3\n" << "\tro\n" << "\tCH3\n" << endl;

		statement->append(look->lexeme + " ");
		CH3();
		statement->append(look->lexeme + " ");
		match("ro");
		statement->append(look->lexeme);
		CH3();
	}
}

void ParseTranslate::CH3() {
	if (look->type == "id") {
		printTree << "CH3 :\n" << "\tid (" << look->lexeme << ")\n" << endl;
		match("id");
	}
	else if (look->type == "nc") {
		printTree << "CH3 :\n" << "\tnc (" << look->lexeme << ")\n" << endl;
		match("nc");
	}
	else if (look->type == "lc") {
		printTree << "CH3 :\n" << "\tlc (" << look->lexeme << ")\n" << endl;
		match("nc");
	}
}

void ParseTranslate::match(string s) {
	if (look->type == s)
		look = nextTok();
	else {
		cout << "Bad Token" << endl;
		cout << "Token Number : " << index << endl;
		exit(EXIT_FAILURE);
	}
}

Token* ParseTranslate::nextTok() {
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

void ParseTranslate::addEntity(Entity *e) {
	//check if it already exists
	symbolTable.push_back(e);
}


void ParseTranslate::backPatch(int index, int line) {
	for (int i = index - 1; i >= 0; i--) {
		if (threeAddressCode[i]->compare("goto") == 0) {
			threeAddressCode[i]->append(" " + to_string(line));
			break;
		}
	}
}

string ParseTranslate::getType(string id) {
	for (int i = 0; i < symbolTable.size(); i++) {
		if (symbolTable[i]->id == id)
			return symbolTable[i]->dataType;
	}
}

void ParseTranslate::addTempVar(int start, string type, string value) {

	for (int i = start; i < tempCount; i++) {
		Entity *entity = new Entity();
		entity->dataType = type;
		entity->value = value;
		entity->id = "t" + to_string(i);
		addEntity(entity);
	}
}

void ParseTranslate::printSymbolTable() {
	ofstream fout;
	fout.open("parser-symboltable.txt");
	
	for (int i = 0; i < symbolTable.size(); i++) {
		if (symbolTable[i]->id[0] != 't')
			fout << symbolTable[i]->id << '\t' << symbolTable[i]->dataType << endl;
	}
	fout.close();

	fout.open("translator-symboltable.txt");
	for (int i = 0; i < symbolTable.size(); i++) {
		
		if (symbolTable[i]->dataType != "fun" && i + 1 != symbolTable.size())
			fout << symbolTable[i]->id << '\t' << symbolTable[i]->dataType << '\t' << symbolTable[i]->value << endl;
		
		else if (symbolTable[i]->dataType != "fun" && i + 1 == symbolTable.size())
			fout << symbolTable[i]->id << '\t' << symbolTable[i]->dataType << '\t' << symbolTable[i]->value;

	}
	fout.close();
}

void ParseTranslate::printThreeAddressCode() {
	ofstream fout;
	fout.open("tac.txt");

	for (int i = 0; i < threeAddressCode.size(); i++) {
		if (i + 1 == threeAddressCode.size())
			fout << i << '\t' << *threeAddressCode[i];
		else
			fout << i << '\t' << *threeAddressCode[i] << endl;

	}
	fout.close();
}