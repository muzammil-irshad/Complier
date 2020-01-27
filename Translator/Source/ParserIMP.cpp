#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "Parser.h"

using namespace std;

Parser::Parser() {
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
	fin.close();

	index = 0;
	opCount = 0;
	tempCount = 0;
	h2l = false;
	look = nextTok();
	
	//start parser
	P();
	fout.close();

	
}

void Parser::P() {
	if (index >= tokens.size())
		return;
	
	//print graph
	fout << "P :\n" << "\tD\n" << "\tP\n"  << endl;
	
	D();
	P();
}

void Parser::D() {
	//print graph
	fout << "D :\n" << "\tVD\n" << "\tFD\n" << endl;
	
	VD();
	FD();
}

void Parser::FD() {
	if (look->type == "def") {
		match("def");
		
		//generate three Address Code
		statement = new string("START FUNC");
		threeAddressCode.push_back(statement);

		//add entity in symbol table
		entity = new Entity();
		entity->dataType = "fun";
		entity->id = look->lexeme;
		addEntity(entity);

		//print graph
		fout << "FD :\n" << "\tdef\n" << "\tid (" << look->lexeme << ")\n" << "\t(OPL)\n" << "\t{SL}\n" << endl;

		string s = look->lexeme; //save function name

		match("id");
		match("(");
		OPL();

		//generate three Address Code
		statement = new string(s + ", " + to_string(opCount));
		threeAddressCode.push_back(statement);


		match(")");
		match("{");
		SL();
		match("}");

		//generate three Address Code
		statement = new string("END FUNC");
		threeAddressCode.push_back(statement);
	}
}

void Parser::SL() {
	//print graph
	fout << "SL :\n" << "\tS\n" << "\tSL\n" << endl;

	S();
	if (look->type != "}")
		SL();
}

void Parser::S() {
	//print graph
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
	//print graph
	fout << "OPL :\n" << "\tPL\n" << endl;
	
	PL();
}

void Parser::PL() {
	//print graph
	fout << "PL :\n" << "\tPAR\n" << "\tR1\n" << endl;
	
	PAR();
	
	//count parameters
	opCount++;
	
	R1();
}

void Parser::R1() {
	if (look->type == ",") {
		//print graph
		fout << "R1 :\n" << "\t,\n" << "\tPAR\n" << "\tR1\n" << endl;
		
		match(",");
		PAR();
		
		//count OPL
		opCount++;
		
		R1();
	}
}

void Parser::PAR() {
	string s = look->type;
	if (look->type == "int" || look->type == "char") {

		T();

		//print graph
		fout << "PAR :\n" << "\tT\n" << "\tid (" << look->lexeme << ")\n" << endl;
		fout << "T :\n" << "\t" << s << '\n' << endl;

		//add entity in symbol table
		entity = new Entity();
		entity->dataType = s;
		entity->id = look->lexeme;
		addEntity(entity);
		
		//generate three address code
		statement = new string(look->lexeme);
		threeAddressCode.push_back(statement);

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

		//print graph
		fout << "VD :\n" << "\tT\n" << "\tid (" << look->lexeme << ")\n" << "\tR2\n" << "\t;\n" << endl;
		fout << "T :\n" << "\t" << s << '\n' << endl;

		//add entity in symbol table
		entity = new Entity();
		entity->dataType = s;
		entity->id = look->lexeme;
		addEntity(entity);

		//generate three address code
		statement = new string(look->lexeme);
		threeAddressCode.push_back(statement);

		match("id");
		R2();
		match(";");
	}
}

void Parser::R2() {
	string s = entity->dataType;
	if (look->type == ",") {
		match(",");

		//print graph
		fout << "R2 :\n" << "\t,\n" << "\tid (" << look->lexeme << ")\n" << "\tR2\n" << endl;

		//add entity in symbol table
		entity = new Entity();
		entity->dataType = s;
		entity->id = look->lexeme;
		addEntity(entity);
		
		//generate three address code
		statement = new string(look->lexeme);
		threeAddressCode.push_back(statement);

		match("id");
		R2();
	}
}

void Parser::IN() {
	if (look->type == "read") {

		//print graph
		fout << "IN :\n" << "\tread\n" << "\tid (" << look->lexeme << ")\n" << "\t;\n" << endl;

		match("read");

		//generate three address code
		statement = new string("in " + look->lexeme);
		threeAddressCode.push_back(statement);

		match("id");
		match(";");
	}
}

void Parser::OUT() {
	if (look->type == "print") {
		//print graph
		fout << "OUT :\n" << "\tprint\n" << "\tCH1\n" << "\t;\n" << endl;
		
		match("print");
		
		//generate three address code
		statement = new string("out " + look->lexeme);
		threeAddressCode.push_back(statement);

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
		//print graph
		fout << "A :\n" << "\tid (" << look->lexeme << ")\n" << "\t<-\n" << "\tAE\n" << "\t;\n" << endl;
		
		//generate three address code
		statement = new string(look->lexeme + " = " + "t" + to_string(tempCount));
		string *s = statement;
		
		match("id");
		match("<-");
		AE();

		threeAddressCode.push_back(s);

		match(";");
	}
}

void Parser::COND() {
	if (look->type == "if") {
		//print graph
		fout << "COND :\n" << "\tif\n" << "\t(BE)\n" << "\tCH2\n" << "\tELSE\n" << endl;
		
		match("if");
		match("(");
		
		statement = new string("if (");
		
		BE();
		
		statement->append(") goto " + to_string(threeAddressCode.size() + 2));
		threeAddressCode.push_back(statement);
		

		statement = new string("goto");
		threeAddressCode.push_back(statement);


		match(")");
		CH2();
		ELSE();
	}
}

void Parser::ELSE() {
	if (look->type == "else") {
		
		backPatch(threeAddressCode.size(), threeAddressCode.size() + 1);

		statement = new string("goto");
		threeAddressCode.push_back(statement);
		
		fout << "ELSE :\n" << "\telse\n" << "\tCH2\n" << endl;
		match("else");
		CH2();

		backPatch(threeAddressCode.size(), threeAddressCode.size());
	}
	backPatch(threeAddressCode.size(), threeAddressCode.size());
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
		
		int line = threeAddressCode.size();

		statement = new string("if (");
		
		BE();
		
		statement->append(") goto " + to_string(threeAddressCode.size() + 2));
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

void Parser::RET() {
	if (look->type == "ret") {
		//print graph
		fout << "RET :\n" << "\tret\n" << "\tCH1\n" << "\t;\n" << endl;
		
		match("ret");
		
		//generate three address code
		statement = new string("ret " + look->lexeme);
		threeAddressCode.push_back(statement);

		CH1();
		match(";");
	}
}

void Parser::AE() {
	//print graph
	fout << "AE :\n" << "\tAE1\n" << "\tR3\n" << endl;
	
	h2l = false;
	
	AE1();
	R3();
}

void Parser::R3() {
	if (look->type == "+") {
		//print graph
		fout << "R3 :\n" << "\t+\n" << "\tAE1\n" << "\tR3\n" << endl;
		string *s;
		if (h2l) {
			string *temp = new string (*threeAddressCode[threeAddressCode.size() - 1]);
			temp->erase(temp->find("="), temp->size());
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
		fout << "R3 :\n" << "\t-\n" << "\tAE1\n" << "\tR3\n" << endl;

		string *s;
		if (h2l) {
			string *temp = new string (*threeAddressCode[threeAddressCode.size() - 1]);
			temp->erase(temp->find("="), temp->size());
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

void Parser::AE1() {
	fout << "AE1 :\n" << "\tAE2\n" << "\tR4\n" << endl;

	AE2();
	R4();
}

void Parser::R4() {
	if (look->type == "*") {
		fout << "R4 :\n" << "\t*\n" << "\tAE2\n" << "\tR4\n" << endl;

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
		fout << "R4 :\n" << "\t/\n" << "\tAE2\n" << "\tR4\n" << endl;

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

void Parser::AE2() {
	if (look->type == "id") {
		
		fout << "AE2 :\n" << "\tid (" << look->lexeme << ")\n" << endl;
		
		statement = new string("t" + to_string(tempCount++) + " = " + look->lexeme);
		threeAddressCode.push_back(statement);
		
		match("id");
	}
	else if (look->type == "nc") {
		fout << "AE2 :\n" << "\tnc (" << look->lexeme << ")\n" << endl;
		
		statement = new string("t" + to_string(tempCount++) + " = " + look->lexeme);
		threeAddressCode.push_back(statement);
		
		match("nc");
	}
	else if (look->type == "lc") {
		fout << "AE2 :\n" << "\tlc (" << look->lexeme << ")\n" << endl;
		
		statement = new string("t" + to_string(tempCount++) + " = " + look->lexeme);
		threeAddressCode.push_back(statement);
		
		match("lc");
	}
	else if (look->type == "(") {
		fout << "AE2 :\n" << "\t(AE)\n" << endl;
		
		bool prev_state = h2l;

		match("(");
		AE();
		match(")");

		h2l = prev_state;

		string *temp = new string(*threeAddressCode[threeAddressCode.size() - 1]);
		temp->erase(temp->find("="), temp->size());
		statement = new string(*temp + " = " + *temp);
		threeAddressCode.push_back(statement);
	}
}

void Parser::BE() {
	if (look->type == "id" || look->type == "nc" || look->type == "lc") {
		
		//print graph
		fout << "BE :\n" << "\tCH3\n" << "\tro\n" << "\tCH3\n" << endl;

		statement->append(look->lexeme + " ");
		CH3();
		statement->append(look->lexeme + " ");
		match("ro");
		statement->append(look->lexeme);
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


void Parser::backPatch(int index, int line) {
	for (int i = index - 1; i >= 0; i--) {
		if (threeAddressCode[i]->compare("goto") == 0) {
			threeAddressCode[i]->append(" " + to_string(line));
			break;
		}
	}
}

void Parser::addTempVar() {
	//as all the temporary variables will be integer

	for (int i = 0; i < tempCount; i++) {
		Entity *entity = new Entity();
		entity->dataType = "int";
		entity->id = "t" + to_string(i);
		addEntity(entity);
	}
}

void Parser::setInitialValue() {

}

void Parser::printSymbolTable() {
	fout.open("parser-symboltable.txt");
	for (int i = 0; i < symbolTable.size(); i++) {
		fout << symbolTable[i]->id << '\t' << symbolTable[i]->dataType << endl;
	}
	fout.close();

	//Addon
	addTempVar();
	setInitialValue();

	fout.open("translator-symboltable.txt");
	for (int i = 0; i < symbolTable.size(); i++) {
		if (symbolTable[i]->dataType != "fun")
			fout << symbolTable[i]->id << '\t' << symbolTable[i]->dataType << '\t' << symbolTable[i]->value << endl;
	}
	fout.close();
}

void Parser::printThreeAddressCode() {
	fout.open("tac.txt");
	for (int i = 0; i < threeAddressCode.size(); i++) {
		fout << i  << '\t' <<  *threeAddressCode[i] << endl;
	}
	fout.close();
}