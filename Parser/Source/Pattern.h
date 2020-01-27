#ifndef Pattern_H
#define Pattern_H

#include <string>

using namespace std;

class Pattern{ 
public:
	bool isDataType(string str);
	bool isKeyword(string str);
	bool isArithmeticOperator(string str);
	bool isRelationalOperator(string str, string& typ);
	bool isIdentifier(string str);
	bool isNumericConstant(string str);
	bool isLiteralConstant(string str);
	bool isStringConstant(string str);
	bool isAssignmentOperator(string str);
	bool isSymbol(string str);
};

#endif
