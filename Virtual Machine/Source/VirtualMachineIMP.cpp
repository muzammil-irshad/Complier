#include <iostream>
#include <fstream>
#include "VirtualMachine.h"

using namespace std;

VirtualMachine::VirtualMachine() {
	
	getVariables();
	getTAC();
}

void VirtualMachine::getVariables() {
	ifstream fin;
	fin.open("translator-symboltable.txt");

	if (!fin.is_open()) {
		cout << "Can't open file" << endl;
		exit(EXIT_FAILURE);
	}

	string str;
	int index = 0;
	while (!fin.eof()) {
		getline(fin, str, '\t');
		varIndex[str] = index;

		getline(fin, str, '\t');

		getline(fin, str, '\n');
		ds.push_back(stoi(str));

		index++;
	}
}

void VirtualMachine::getTAC() {
	ifstream fin;
	fin.open("tac.txt");

	if (!fin.is_open()) {
		cout << "Can't open file" << endl;
		exit(EXIT_FAILURE);
	}

	string str;
	while (!fin.eof()) {
		getline(fin, str, '\t'); //ignore line number
		getline(fin, str, '\n');
		identifyStatement(str);
	}
}

void VirtualMachine::identifyStatement(string st) {
	inst = new int[4];
	istringstream ss(st);
	string intermediate;
	vector<string> tokens;

	for (int i = 0; i < 4; i++)
		inst[i] = -1; //for nop

	while (getline(ss, intermediate, ' '))
		tokens.push_back(intermediate);

	//variable declaration
	if (st.find(" ") == string::npos)
		q.push_back(inst);

	//Start and End Function
	else if (st == "START FUNC" || st == "END FUNC")
		q.push_back(inst);

	//read statement
	else if (st.find("read") != string::npos)
		readStatement(tokens);

	//print statement
	else if (st.find("print") != string::npos)
		printStatement(tokens);

	//return statement
	else if (st.find("ret") != string::npos)
		returnStatement(tokens);

	//arithmetic statement
	else if (st.find("+") != string::npos || st.find("-") != string::npos || st.find("*") != string::npos || st.find("/") != string::npos)
		arithmeticStatement(tokens);

	//assignment statement
	else if (st.find("=") != string::npos)
		assignmentStatement(tokens);

	//conditional statement
	else if (st.find("if") != string::npos)
		conditionalStatement(tokens);

	//goto statement
	else if (st.find("goto") != string::npos)
		jumpStatement(tokens);

	else
		q.push_back(inst);

}

void VirtualMachine::readStatement(vector<string> tokens) {
	inst[0] = 1;
	inst[3] = varIndex[tokens[1]];

	q.push_back(inst);
}

void VirtualMachine::printStatement(vector<string> tokens) {
	string* st = new string();
	
	inst[0] = 2;
	
	inst[2] = varIndex[tokens[1]];

	q.push_back(inst);
}

void VirtualMachine::returnStatement(vector<string> tokens) {
	inst[0] = 4;
	inst[3] = varIndex[tokens[1]];
	q.push_back(inst);
}

void VirtualMachine::arithmeticStatement(vector<string> tokens) {
	if (tokens[3] == "+")
		inst[0] = 5;
	else if (tokens[3] == "-")
		inst[0] = 6;
	else if (tokens[3] == "*")
		inst[0] = 7;
	else if (tokens[3] == "/")
		inst[0] = 8;

	if (varIndex.find(tokens[2]) == varIndex.end()) {
		inst[1] = ds.size();
		ds.push_back(stoi(tokens[2]));
	}
	else {
		inst[1] = varIndex[tokens[2]];
	}

	if (varIndex.find(tokens[4]) == varIndex.end()) {
		inst[2] = ds.size();
		ds.push_back(stoi(tokens[4]));
	}
	else {
		inst[2] = varIndex[tokens[4]];
	}

	inst[3] = varIndex[tokens[0]];

	q.push_back(inst);
}

void VirtualMachine::assignmentStatement(vector<string> tokens) {

	inst[0] = 9;

	if (varIndex.find(tokens[2]) == varIndex.end()) {
		inst[1] = ds.size();
		ds.push_back(stoi(tokens[2]));
	}
	else {
		inst[1] = varIndex[tokens[2]];
	}

	inst[3] = varIndex[tokens[0]];

	q.push_back(inst);
}

void VirtualMachine::conditionalStatement(vector<string> tokens) {
	if (tokens[2] == "GT")
		inst[0] = 10;
	else if (tokens[2] == "ST")
		inst[0] = 11;
	else if (tokens[2] == "GE")
		inst[0] = 12;
	else if (tokens[2] == "SE")
		inst[0] = 13;
	else if (tokens[2] == "EQ")
		inst[0] = 14;
	else if (tokens[2] == "NE")
		inst[0] = 15;

	if (varIndex.find(tokens[1]) == varIndex.end()) {
		inst[1] = ds.size();
		ds.push_back(stoi(tokens[1]));
	}
	else {
		inst[1] = varIndex[tokens[1]];
	}

	if (varIndex.find(tokens[3]) == varIndex.end()) {
		inst[2] = ds.size();
		ds.push_back(stoi(tokens[3]));
	}
	else {
		inst[2] = varIndex[tokens[3]];
	}

	inst[3] = stoi(tokens[5]);

	q.push_back(inst);
}

void VirtualMachine::jumpStatement(vector<string> tokens) {
	inst[0] = 3;
	inst[3] = stoi(tokens[1]);
	q.push_back(inst);
}


void VirtualMachine::execute() {
	for (int pc = 0; pc < q.size();) {
		
		switch (q[pc][0]) {
		
		case -1:
			pc++;
			break;

		case 1:
			cin >> q[pc][3];
			pc++;
			break;

		case 2:
			cout << ds[q[pc][2]] << endl;
			pc++;
			break;

		case 3:
			pc = q[pc][3];
			break;

		case 4:
			cout << "Output : " << q[pc][1] << endl;
			pc++;
			break;

		case 5:
			ds[q[pc][3]] = ds[q[pc][1]] + ds[q[pc][2]];
			pc++;
			break;

		case 6:
			ds[q[pc][3]] = ds[q[pc][1]] - ds[q[pc][2]];
			pc++;
			break;

		case 7:
			ds[q[pc][3]] = ds[q[pc][1]] * ds[q[pc][2]];
			pc++;
			break;

		case 8:
			ds[q[pc][3]] = ds[q[pc][1]] / ds[q[pc][2]];
			pc++;
			break;

		case 9:
			ds[q[pc][3]] = ds[q[pc][1]];
			pc++;
			break;

		case 10:
			if (ds[q[pc][1]] > ds[q[pc][2]])
				pc = q[pc][3];
			else
				pc++;
			break;

		case 11:
			if (ds[q[pc][1]] < ds[q[pc][2]])
				pc = q[pc][3];
			else
				pc++;
			break;

		case 12:
			if (ds[q[pc][1]] >= ds[q[pc][2]])
				pc = q[pc][3];
			else
				pc++;
			break;

		case 13:
			if (ds[q[pc][1]] <= ds[q[pc][2]])
				pc = q[pc][3];
			else
				pc++;
			break;

		case 14:
			if (ds[q[pc][1]] == ds[q[pc][2]])
				pc = q[pc][3];
			else
				pc++;
			break;

		case 15:
			if (ds[q[pc][1]] != ds[q[pc][2]])
				pc = q[pc][3];
			else
				pc++;
			break;
		}
	}
}