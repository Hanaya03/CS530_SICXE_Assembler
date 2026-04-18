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
#include "BData.h"
#include "PBlocks.h"
#include "BData.h"
#include "LiteralEntry.h"


class Pass2 {
private:
    static int mBaseReg;
    static std::vector<LiteralEntry> mLiteralTable;
    static std::string mProgName;
    static int mProgStart;
    static int mProgEnd;

    //static void CollectLiterals(const std::vector<SourceLine>& lines);

public:
    static std::string litToHex(const std::string& raw);
    static int litLen(const std::string& raw);
    static std::string litContent(const std::string& raw);
    static bool GenerateOutput(const std::string& sourceFile);
};

#endif
