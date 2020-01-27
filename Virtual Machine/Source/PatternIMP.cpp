#include "Pattern.h"

using namespace std;

bool Pattern::isDataType(string str) {
	for (int i = 0, st = 0; i < str.size(); ++i) {
		switch (st) {
		case 0:
			if (str[i] == 'i')
				st = 1;
			else if (str[i] == 'c')
				st = 3;
			else
				return false;
			break;

		case 1:
			if (str[i] == 'n')
				st = 2;
			else
				return false;
			break;

		case 2:
			if (str[i] == 't')
				st = 6;
			else
				return false;
			break;

		case 3:
			if (str[i] == 'h')
				st = 4;
			else
				return false;
			break;

		case 4:
			if (str[i] == 'a')
				st = 5;
			else
				return false;
			break;

		case 5:
			if (str[i] == 'r')
				st = 6;
			else
				return false;
			break;

		case 6:
			if ((str[i] == ' ' || str[i] == '\t') && i + 1 == str.size())
				return true;
			else
				return false;
			break;
		}
	}
	return false;
}

bool Pattern::isKeyword(string str) {
	for (int i = 0, st = 0; i < str.size(); ++i) {
		switch (st) {
		case 0:
			if (str[i] == 'd')
				st = 1;
			else if (str[i] == 'i')
				st = 3;
			else if (str[i] == 'e')
				st = 5;
			else if (str[i] == 'w')
				st = 9;
			else if (str[i] == 'r')
				st = 14;
			else if (str[i] == 'p')
				st = 16;
			else
				return false;
			break;

		case 1:
			if (str[i] == 'e')
				st = 2;
			else
				return false;
			break;

		case 2:
			if (str[i] == 'f')
				st = 21;
			else
				return false;
			break;

		case 3:
			if (str[i] == 'f')
				st = 4;
			else
				return false;
			break;

		case 4:
			if ((str[i] == ' ' || str[i] == '\t' || str[i] == '(') && i + 1 == str.size())
				return true;
			else
				return false;
			break;

		case 5:
			if (str[i] == 'l')
				st = 6;
			else
				return false;
			break;

		case 6:
			if (str[i] == 's')
				st = 7;
			else
				return false;
			break;

		case 7:
			if (str[i] == 'e')
				st = 8;
			else
				return false;
			break;

		case 8:
			if ((str[i] == '\n' || str[i] == ' ' || str[i] == '\t' || str[i] == '{') && i + 1 == str.size())
				return true;
			else
				return false;
			break;

		case 9:
			if (str[i] == 'h')
				st = 10;
			else
				return false;
			break;

		case 10:
			if (str[i] == 'i')
				st = 11;
			else
				return false;
			break;

		case 11:
			if (str[i] == 'l')
				st = 12;
			else
				return false;
			break;

		case 12:
			if (str[i] == 'e')
				st = 13;
			else
				return false;
			break;

		case 13:
			if ((str[i] == ' ' || str[i] == '\t' || str[i] == '(') && i + 1 == str.size())
				return true;
			else
				return false;
			break;

		case 14:
			if (str[i] == 'e')
				st = 15;
			else
				return false;
			break;

		case 15:
			if (str[i] == 't')
				st = 21;
			else if (str[i] == 'a')
				st = 20;
			else
				return false;
			break;

		case 16:
			if (str[i] == 'r')
				st = 17;
			else
				return false;
			break;

		case 17:
			if (str[i] == 'i')
				st = 18;
			else
				return false;
			break;

		case 18:
			if (str[i] == 'n')
				st = 19;
			else
				return false;
			break;

		case 19:
			if (str[i] == 't')
				st = 21;
			else
				return false;
			break;

		case 20:
			if (str[i] == 'd')
				st = 21;
			else
				return false;
			break;

		case 21:
			if ((str[i] == ' ' || str[i] == '\t') && i + 1 == str.size())
				return true;
			else
				return false;
			break;
		}
	}
	return false;
}

bool Pattern::isArithmeticOperator(string str) {
	for (int i = 0, st = 0; i < str.size(); ++i) {
		switch (st) {
		case 0:
			if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/')
				;//do nothing
			else
				return false;
			st = 1;
			break;

		case 1:
			if ((str[i] == ' ' || str[i] == '\t' || str[i] == '(' || isalpha(str[i]) != 0 || isdigit(str[i]) != 0) && i + 1 == str.size())
				return true;
			else
				return false;
			break;
		}
	}
	return false;
}

bool Pattern::isRelationalOperator(string str, string& lex) {
	for (int i = 0, st = 0; i < str.size(); ++i) {
		switch (st) {
		case 0:
			if (str[i] == '>') {
				lex = "GT";
				st = 1;
			}
			else if (str[i] == '<') {
				lex = "ST";
				st = 2;
			}
			else if (str[i] == '=')
				st = 3;
			else
				return false;
			break;

		case 1:
			if ((str[i] == ' ' || str[i] == '\t' || isalpha(str[i]) != 0 || isdigit(str[i]) != 0) && i + 1 == str.size())
				return true;
			else if (str[i] == '=') {
				lex = "GE";
				st = 4;
			}
			else
				return false;
			break;

		case 2:
			if ((str[i] == ' ' || isalpha(str[i]) != 0 || isdigit(str[i]) != 0) && i + 1 == str.size())
				return true;
			else if (str[i] == '=') {
				lex = "SE";
				st = 4;
			}
			else if (str[i] == '>') {
				lex = "NE";
				st = 4;
			}
			else
				return false;
			break;

		case 3:
			if (str[i] == '=') {
				lex = "EQ";
				st = 4;
			}
			else
				return false;
			break;

		case 4:
			if ((str[i] == ' ' || str[i] == '\t' || isalpha(str[i]) != 0 || isdigit(str[i]) != 0) && i + 1 == str.size())
				return true;
			else
				return false;
			break;
		}
	}
	return false;
}

bool Pattern::isIdentifier(string str) {
	for (int i = 0, st = 0; i < str.size(); ++i) {
		switch (st) {
		case 0:
			if (isalpha(str[i]) != 0)
				st = 1;
			else
				return false;
			break;

		case 1:
			if (isalpha(str[i]) != 0 || isdigit(str[i]) != 0)
				; //do nothing
			else if (i + 1 == str.size()) //other than alphabet or digit
				return true;
			else
				return false;
			break;
		}
	}
	return false;
}

bool Pattern::isNumericConstant(string str) {
	for (int i = 0, st = 0; i < str.size(); ++i) {
		switch (st) {
		case 0:
			if (isdigit(str[i]) != 0)
				st = 1;
			else
				return false;
			break;

		case 1:
			if (isdigit(str[i]) != 0)
				; //do nothing
			else if (i + 1 == str.size()) //other than alphabet or digit
				return true;
			else
				return false;
			break;
		}
	}
	return false;
}

bool Pattern::isLiteralConstant(string str) {
	for (int i = 0, st = 0; i < str.size(); ++i) {
		switch (st) {
		case 0:
			if (str[i] == '\'')
				st = 1;
			else
				return false;
			break;

		case 1:
			if (isalpha(str[i]) != 0)
				st = 2;
			else
				return false;
			break;

		case 2:
			if (str[i] == '\'')
				st = 3;
			else
				return false;
			break;

		case 3:
			if (i + 1 == str.size())
				return true;
			else
				return false;
			break;
		}
	}
	return false;
}

bool Pattern::isStringConstant(string str) {
	for (int i = 0, st = 0; i < str.size(); ++i) {
		switch (st) {
		case 0:
			if (str[i] == '"')
				st = 1;
			else
				return false;
			break;

		case 1:
			if (str[i] == '"')
				st = 2;
			break;

		case 2:
			if (i + 1 == str.size())
				return true;
			else
				return false;
			break;
		}
	}
	return false;
}

bool Pattern::isAssignmentOperator(string str) {
	for (int i = 0, st = 0; i < str.size(); ++i) {
		switch (st) {
		case 0:
			if (str[i] == '<')
				st = 1;
			else
				return false;
			break;

		case 1:
			if (str[i] == '-')
				st = 2;
			else
				return false;
			break;

		case 2:
			if (i + 1 == str.size())
				return true;
			else
				return false;
			break;
		}
	}
	return false;
}

bool Pattern::isSymbol(string str) {
	for (int i = 0, st = 0; i < str.size(); ++i) {
		switch (st) {
		case 0:
			if (str[i] == '(' || str[i] == ')' ||
				str[i] == '{' || str[i] == '}' ||
				str[i] == '[' || str[i] == ']' ||
				str[i] == ';' || str[i] == ',') {

				st = 1;
			}
			else
				return false;
			break;

		case 1:
			if (i + 1 == str.size())
				return true;
			else
				return false;
			break;
		}
	}
	
	return false;
}