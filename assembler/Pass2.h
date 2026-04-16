#ifndef PASS2_H
#define PASS2_H

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstdio>
#include "Pass1.h"
#include "OpCode.h"
#include "Code.h"

struct LiteralEntry {
    std::string name;       // content between quotes, e.g. "EOF"
    std::string operandHex; // hex-encoded bytes, e.g. "454F46"
    int address;
    int length;
};

class Pass2 {
private:
    static int mBaseReg;
    static std::vector<LiteralEntry> mLiteralTable;
    static std::string mProgName;
    static int mProgStart;
    static int mProgEnd;

    static void CollectLiterals(const std::vector<SourceLine>& lines);

public:
    static bool GenerateOutput(const std::string& sourceFile);
};

#endif
