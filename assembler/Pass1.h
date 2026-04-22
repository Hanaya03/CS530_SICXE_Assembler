/************************************************
* Pass1.h                                       *
*						*
* Hector Anaya 826734851        		*
* Emiliano Nolasco 130310255    		*
* Ethan Fudge 827570933         		*
*						*
* CS530, Spring 2026				*
* Assignment #2, SICXE Assembler		*
*************************************************/
#ifndef PASS1_H
#define PASS1_H

#include <string>
#include <vector>
#include <cctype>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>
#include "OpCode.h"
#include "Code.h"
#include "Label.h"
#include "PBlocks.h"
#include "BData.h"
#include "Pass2.h"
#include "LiteralEntry.h"
#include <cstdio>

struct Bits{
        int n, i, x, b, p, e;

        Bits(){
                n = 1;
                i = 1;
                x = 0;
                b = 0;
                p = 0;
                e = 0;
        }

        void SetN(int val){ n = val; }
        void SetI(int val){ i = val; }
        void SetX(int val){ x = val; }
        void SetB(int val){ b = val; }
        void SetP(int val){ p = val; }
        void SetE(int val){ e = val; }
};

struct Operand{
        bool isLabel = false;
	bool isLiteral = false;
        bool isExpression = false;
        std::string mLabel = "";
        int mValue = 0;

        void SetValue(int v){
                mValue = v;
        }

        void SetLabel(std::string l){
                mLabel = l;
        }
};

struct SourceLine {
        int address = 0;
        int mBlock;
        Bits mBits;
        std::string label;
        std::string opcode;
        Code* pCode;
        Operand mOperand;
        std::string originalLine;
        
        bool isComment = false;
};

class Pass1 {
        private:
            static std::vector<SourceLine> mLines;
            static std::unordered_map<std::string, int> mSymTab;
            static std::unordered_map<std::string, Label> lSymTab;
            static std::unordered_map<std::string, LiteralEntry> mLitTab;
            static std::vector<LiteralEntry> mLitVec;
            static bool mHadError;
        
            static void ParseOperation(SourceLine*);
            static void ParseOperand(SourceLine*);
            static bool IsNumber(const std::string&);
            public:
            static bool ReadFile(std::string filename);
            static bool HadError();
            static void ClearTables();
            static std::vector<LiteralEntry> GetLitTab();
            static std::vector<SourceLine> GetLines();
            static std::vector<SourceLine> GetAllLines();
            static std::unordered_map<std::string, Label> GetSymTab();
        };

#endif
