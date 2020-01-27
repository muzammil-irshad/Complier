#include <iostream>
#include "LexicalAnalyzer.h"


using namespace std;

int main(int argc, char **argv) {

	string filename;
	if (argc > 1 && argc <= 2)
		filename = argv[1];
	else {
		cout << "Enter single filename e.g. cmm test.cm" << endl;
		return 0;
	}

	LexicalAnalyzer lex;
	lex.tokenize(filename);
	lex.printTokens();
	lex.printSymbolTable();

	return 0;
}
