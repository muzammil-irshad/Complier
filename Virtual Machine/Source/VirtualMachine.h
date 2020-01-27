#ifndef VirtualMachine_H
#define VirtualMachine_H

#include <vector>
#include <map>
#include <sstream>
#include "Entity.h"

using namespace std;

class VirtualMachine {
private:
	vector<int*> q;
	int* inst;
	vector<int> ds;
	map<string, int> varIndex;

public:
	VirtualMachine();
	void getVariables();
	void getTAC();
	void identifyStatement(string st);
	void readStatement(vector<string> tokens);
	void printStatement(vector<string> tokens);
	void returnStatement(vector<string> tokens);
	void arithmeticStatement(vector<string> tokens);
	void assignmentStatement(vector<string> tokens);
	void conditionalStatement(vector<string> tokens);
	void jumpStatement(vector<string> tokens);
	void execute();
};

#endif