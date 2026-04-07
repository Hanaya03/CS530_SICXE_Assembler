#ifndef PASS1_H
#define PASS1_H

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>
#include "OpCode.h"
#include "Code.h"
#include "Tokens.h"
#include "Label.h"

struct SourceLine {
    int address;
    std::string label;
    std::string opcode;
    std::string operand;
    std::string originalLine;
    bool isComment;
};

class Pass1 {
private:
	static std::vector<SourceLine> mLines;
	static std::vector<Token> mTokens;
	static std::unordered_map<std::string, int> mSymTab;
	static std::unordered_map<std::string, Label> dSymTab;
	static Code* pCode;
	static Label* pLabel;
	static Token t;

public:
    Pass1();

    static bool ReadFile(std::string filename);

    std::vector<SourceLine> GetLines();
};

#endif
