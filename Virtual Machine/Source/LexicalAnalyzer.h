#ifndef LexicalAnalyzer_H
#define LexicalAnalyzer_H

#include <string>
#include <vector>
#include "Token.h"
#include "Entity.h"
#include "Pattern.h"

using namespace std;

class LexicalAnalyzer{
private:
  vector<Token*> tokens;
  vector<Entity*> symbolTable;
  Pattern rules;

public:
  LexicalAnalyzer();
  string getSubStr(int start, int end, string str);
  void tokenize(string filename);
  void printTokens();
  void addEntity(string str);
  void printSymbolTable();
};

#endif
