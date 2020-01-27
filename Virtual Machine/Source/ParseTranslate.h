#ifndef ParseTranslate_H
#define ParseTranslate_H

#include <vector>
#include <fstream>
#include "Token.h"
#include "Entity.h"

using namespace std;

class ParseTranslate {
private:
	vector<Token*> tokens;
	vector<Entity*> symbolTable;
	vector<string*> threeAddressCode;
	Token* look;
	Entity* entity;
	string* statement;
	ofstream printTree;
	int opCount; //optional parameter count
	int tempCount; //temp variable count
	int index; //to check index of tokens
	bool h2l; //higher to lower precedence operator

public:
	ParseTranslate();
	void P();
	void D();
	void FD();
	void SL();
	void S();
	void OPL();
	void PL();
	void R1();
	void PAR();
	void T();
	void VD();
	void R2();
	void IN();
	void OUT();
	void CH1();
	void A();
	void COND();
	void ELSE();
	void CH2();
	void L();
	void RET();
	void AE();
	void R3();
	void AE1();
	void R4();
	void AE2();
	void BE();
	void CH3();
	void match(string s);
	Token* nextTok();
	void addEntity(Entity* e);
	void backPatch(int index, int line);
	void printSymbolTable();
	void printThreeAddressCode();
	string getType(string id);
	void addTempVar(int start, string type, string value);
};

#endif