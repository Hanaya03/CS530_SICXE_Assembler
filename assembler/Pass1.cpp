#include "Pass1.h"

std::vector<SourceLine> Pass1::mLines;
std::unordered_map<std::string, int> Pass1::mSymTab;
std::unordered_map<std::string, Label> Pass1::lSymTab;
std::unordered_map<std::string, LiteralEntry> Pass1::mLitTab;
std::vector<LiteralEntry> Pass1::mLitVec;

bool Pass1::ReadFile(std::string filename) {
    std::ifstream inFile(filename);

    if (!inFile) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        return false;
    }

    OpCode::PopulateTable();
    int locCtr = 0;

    std::string line;

    while (std::getline(inFile, line)) {
        // strip Windows CRLF if present
        if (!line.empty() && line.back() == '\r') line.pop_back();
        SourceLine s;

        // Check if comment line
        if (line.size() > 0 && line[0] == '.') {
            s.isComment = true;
            s.originalLine = line;
            mLines.push_back(s);
            continue;
        }
        if (line.size() > 0 && line[0] == '*') {
            continue;
        }

        // Skip completely blank lines
        if (line.empty()) {
            mLines.push_back(s);
            continue;
        }

        std::string first, second, third;
        std::stringstream ss(line);
        ss >> first >> second >> third;

        // If line starts with whitespace, assume no label
        if (line.size() > 0 && (line[0] == ' ' || line[0] == '\t')) {
            s.label = "";
            s.opcode = first;
            s.mOperand.mLabel = second;
        }
        else {
            s.label = first;
            s.opcode = second;
            s.mOperand.mLabel = third;
        }

	ParseOperation(&s);


        // Handle START first so label gets correct starting address
        if (s.opcode == "START") {
            locCtr = std::stoi(s.mOperand.mLabel, nullptr, 16);
            // PBlocks::InsertBlockData(s.label);
            PBlocks::SetCurrentBlock("(default)", std::stoi(s.mOperand.mLabel, nullptr, 16));
            
            s.address = PBlocks::GetDataPtr()->GetCtr();
        }else{

        s.address = PBlocks::GetDataPtr()->GetCtr();
		
        ParseOperand(&s);
	}

        // Add label to SYMTAB
        if (!s.label.empty()) {
            if (mSymTab.find(s.label) != mSymTab.end()) {
                std::cerr << "Error: Duplicate label " << s.label << std::endl;
            }
            else {
                lSymTab[s.label] = Label(PBlocks::GetDataPtr()->GetCtr(), 'R', PBlocks::GetDataPtr()->GetBlockNumber());
                mSymTab[s.label] = PBlocks::GetDataPtr()->GetCtr();
            }
        }


        // Update LOCCTR for directives and instructions
        if (s.opcode == "END" || s.opcode == "LTORG") {
            // emit literal pool
            s.mBlock = PBlocks::GetDataPtr()->GetBlockNumber();
            mLines.push_back(s);
            for (auto& lit : mLitVec){
                if(!lit.assigned)
                {
                    SourceLine tmp;
                    tmp.address = PBlocks::GetDataPtr()->GetCtr();
                    lit.address = PBlocks::GetDataPtr()->GetCtr();
                    PBlocks::GetDataPtr()->IncrementCtr(lit.length);
                    tmp.originalLine = "*\t" + lit.raw + "\t\t" + lit.operandHex + "\n";
                    mLines.push_back(tmp);
                    lit.assigned = true;
                }
                
            }
		continue;
        }
        else if (s.opcode == "USE") {
            if(s.mOperand.mLabel.empty()){      PBlocks::SetCurrentBlock("(default)");}
            else{                               PBlocks::SetCurrentBlock(s.mOperand.mLabel);}
            
            s.address = PBlocks::GetDataPtr()->GetCtr();
            
        }else if (s.opcode == "WORD") {
            PBlocks::GetDataPtr()->IncrementCtr(3);
            locCtr += 3;
        }
        else if (s.opcode == "RESW") {
            PBlocks::GetDataPtr()->IncrementCtr(s.mOperand.mValue * 3);
            locCtr += s.mOperand.mValue * 3; // words are 3 bytes each
        }
        else if (s.opcode == "RESB") {
            PBlocks::GetDataPtr()->IncrementCtr(s.mOperand.mValue);
            locCtr += s.mOperand.mValue;
        }
        else if (s.opcode == "BYTE") {
            if (s.mOperand.mLabel.size() >= 3 && s.mOperand.mLabel[0] == 'C' && s.mOperand.mLabel[1] == '\'') {
            PBlocks::GetDataPtr()->IncrementCtr(s.mOperand.mLabel.size() - 3);
                locCtr += s.mOperand.mLabel.size() - 3;
            }
            else if (s.mOperand.mLabel.size() >= 3 && s.mOperand.mLabel[0] == 'X' && s.mOperand.mLabel[1] == '\'') {
            PBlocks::GetDataPtr()->IncrementCtr((s.mOperand.mLabel.size() - 3) / 2);
                locCtr += (s.mOperand.mLabel.size() - 3) / 2;
            }
        }
        else if (OpCode::ValidateOperation(s.opcode)) {
            PBlocks::GetDataPtr()->IncrementCtr((s.mBits.e == 1) ? 4 : 3);
            locCtr += (s.mBits.e == 1) ? 4 : 3;   // format 4 = 4 bytes, format 3 = 3 bytes
            s.pCode = OpCode::GetCode(s.opcode);
        }

        s.mBlock = PBlocks::GetDataPtr()->GetBlockNumber();

        mLines.push_back(s);
    }

    PBlocks::FinalizeBlocks();

    return true;
}

void Pass1::ParseOperation(SourceLine* s){
	if (!s->opcode.empty() && s->opcode[0] == '+') {
        	s->mBits.e = 1;
        	s->opcode = s->opcode.substr(1);
	}
}

void Pass1::ParseOperand(SourceLine* s) {
    std::string& label = s->mOperand.mLabel; 

    if (!label.empty() && label[0] == '@') {
        s->mBits.n = 1;
        s->mBits.i = 0;
        label = label.substr(1);
    }
    if (!label.empty() && label[0] == '#') {
        s->mBits.n = 0;
        s->mBits.i = 1;
        label = label.substr(1);
    }

    //star operand means this address
    if (label == "*") {
        s->mOperand.mValue = PBlocks::GetDataPtr()->GetCtr();
        return;
    }

    //look for arithmetic operations
    size_t minusPos = label.find('-');
    size_t plusPos  = label.find('+');
    size_t mulPos = label.find('*');
    size_t divPos  = label.find('/');

    if (minusPos != std::string::npos || plusPos != std::string::npos || mulPos != std::string::npos || divPos != std::string::npos) {
        s->mOperand.isExpression = true;
        s->mOperand.mLabel = label;
        return;
    }
    
    if (!label.empty() && label[0] == '=') {
        // literal: simple addressing - the operand is the address of the literal
        s->mBits.n = 1;
        s->mBits.i = 1;
        s->mOperand.isLiteral = true;

        std::string name = Pass2::litContent(s->mOperand.mLabel);
        for (auto& e : mLitVec) if (e.name == name) { return; }
        LiteralEntry e { false, s->mOperand.mLabel, name, Pass2::litToHex(s->mOperand.mLabel), PBlocks::GetDataPtr()->GetCtr(), Pass2::litLen(s->mOperand.mLabel), PBlocks::GetDataPtr()->GetBlockNumber()};
        mLitTab[name] = e;
        mLitVec.push_back(e);
        return;
    }
    if (label.size() >= 2 && label.substr(label.size() - 2) == ",X") {
        s->mBits.x = 1;
        label = label.substr(0, label.size() - 2); 
    }

    if (IsNumber(label)) {
        s->mOperand.mValue = std::stoi(label, nullptr, 10); // operands are decimal
    } else {
        s->mOperand.isLabel = true;
    }
}

std::vector<LiteralEntry> Pass1::GetLitTab(){return mLitVec;}


bool Pass1::IsNumber(const std::string& s) {
    if (s.empty()) return false;

    for (char c : s) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

std::vector<SourceLine> Pass1::GetLines() {
    return mLines;
}
