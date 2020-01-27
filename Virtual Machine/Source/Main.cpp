#include <iostream>
#include "LexicalAnalyzer.h"
#include "ParseTranslate.h"
#include "VirtualMachine.h"

using namespace std;

int main(int argc, char **argv) {
	string filename;
	if (argc > 1 && argc <= 2)
		filename = argv[1];
	else {
		cout << "Enter single filename e.g. cmm test.cm" << endl;
		return 0;
	}

	//Lexical Analyzer
	LexicalAnalyzer lex;
	lex.tokenize("test.cm");
	lex.printTokens();
	lex.printSymbolTable();

	//Parser and Translator
	ParseTranslate parse;
	parse.printSymbolTable();
	parse.printThreeAddressCode();
	
	//VirtualMachine
	VirtualMachine VM;
	VM.execute();


	return 0;
}
