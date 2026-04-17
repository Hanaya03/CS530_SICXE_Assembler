#include "Pass1.h"

std::vector<SourceLine> Pass1::mLines;
std::unordered_map<std::string, int> Pass1::mSymTab;
std::unordered_map<std::string, Label> Pass1::dSymTab;

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
            printf("Line is a comment. Skipping.\n %s\n", line.c_str());
            continue;
        }
        if (line.size() > 0 && line[0] == '*') {
            mLines.push_back(s);
		printf("IDK what this star is suppose to represent but I know we don't need to worry about it.\n %s\n", line.c_str());
            continue;
        }

        // Skip completely blank lines
        if (line.empty()) {
            mLines.push_back(s);
		printf("this line is empty. skipping.\n");
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
		printf("Objects found for line:\n%s\nLabel: empty, Operation:%s, Operand:%s\n", line.c_str(), s.opcode.c_str(), s.mOperand.mLabel.c_str());
        }
        else {
            s.label = first;
            s.opcode = second;
            s.mOperand.mLabel = third;
		printf("Objects found for line:\n%s\nLabel: %s, Operation:%s, Operand:%s\n", line.c_str(), s.label.c_str(), s.opcode.c_str(), s.mOperand.mLabel.c_str());
        }

	printf("Parsing operation field.\n");
	ParseOperation(&s);
	printf("Parsed operation: %s\n", s.opcode.c_str());	


        // Handle START first so label gets correct starting address
        if (s.opcode == "START") {
            locCtr = std::stoi(s.mOperand.mLabel, nullptr, 16);
            // PBlocks::InsertBlockData(s.label);
            PBlocks::SetCurrentBlock("(default)", std::stoi(s.mOperand.mLabel, nullptr, 16));
            
            s.address = PBlocks::GetDataPtr()->GetCtr();
        }else{
            printf("Parsing operand field\n");

        s.address = PBlocks::GetDataPtr()->GetCtr();
		
        ParseOperand(&s);
		if(s.mOperand.isLabel){
			printf("Operand is a label: %s\n", s.mOperand.mLabel.c_str());
		}else{
			printf("Operand is a value: %d\n", s.mOperand.mValue);
		}
	}

        // Add label to SYMTAB
        if (!s.label.empty()) {
            if (mSymTab.find(s.label) != mSymTab.end()) {
                std::cerr << "Error: Duplicate label " << s.label << std::endl;
            }
            else {
                mSymTab[s.label] = locCtr;
            }
        }


        // Update LOCCTR for directives and instructions
        if (s.opcode == "USE") {
            PBlocks::SetCurrentBlock(s.label);
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

        printf("\n");

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
    if (!label.empty() && label[0] == '=') {
        // literal: simple addressing - the operand is the address of the literal
        s->mBits.n = 1;
        s->mBits.i = 1;
        s->mOperand.isLiteral = true;
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
