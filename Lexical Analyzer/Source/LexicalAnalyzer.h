#ifndef LexicalAnalyzer_H
#define LexicalAnalyzer_H

#include <string>
#include <vector>
#include "Pattern.h"

using namespace std;

class LexicalAnalyzer{
private:
  struct Token{
    string type;
    string lexeme;
  };
  vector<Token*> tokens;
  vector<string> identifiers;
  Pattern rules;

public:
  LexicalAnalyzer();
  string getSubStr(int start, int end, string str);
  void tokenize(string filename);
  void printTokens();
  bool repeat(string str);
  void printSymbolTable();
};

#endif
