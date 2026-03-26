#ifndef PASS1_H
#define PASS1_H

#include <string>
#include <vector>
#include <unordered_map>

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
    std::vector<SourceLine> mLines;
    std::unordered_map<std::string, int> mSymTab;

public:
    Pass1();

    bool ReadFile(std::string filename);

    std::vector<SourceLine> GetLines();
};

#endif