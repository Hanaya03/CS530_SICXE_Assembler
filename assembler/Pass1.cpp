#include "Pass1.h"

std::vector<SourceLine> Pass1::mLines;
std::vector<Token> Pass1::mTokens;
std::unordered_map<std::string, int> Pass1::mSymTab;
std::unordered_map<std::string, Label> Pass1::dSymTab;
Label* Pass1::pLabel;
Code* Pass1::pCode;

Pass1::Pass1() {
}

bool Pass1::ReadFile(std::string filename) {
    std::ifstream inFile(filename);

    if (!inFile) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        return false;
    }

    OpCode::PopulateTable();
    int locCtr = 0;

    std::string line;

    SourceLine s;
    while (std::getline(inFile, line)) {
	Token t;

        // Check if comment line
        if (line.size() > 0 && line[0] == '.') {
            s.isComment = true;
		mTokens.push_back(t);
            mLines.push_back(s);
            continue;
        }

        // Skip completely blank lines
        if (line.empty()) {
		mTokens.push_back(t);
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
            s.operand = second;
        }
        else {
            s.label = first;
            s.opcode = second;
            s.operand = third;
        }

	s.opcode = ParseOperation(s.opcode, t);

        s.address = locCtr;

        // Handle START first so label gets correct starting address
        if (s.opcode == "START") {
            locCtr = std::stoi(s.operand, nullptr, 16);
            s.address = locCtr;
        }

	t.SetAddress(locCtr);
        // Add label to SYMTAB
        if (!s.label.empty()) {
            if (mSymTab.find(s.label) != mSymTab.end()) {
                std::cerr << "Error: Duplicate label " << s.label << std::endl;
            }
            else {
                mSymTab[s.label] = s.address;
            }
        }

        // Update LOCCTR for directives and instructions
        if (s.opcode == "WORD") {
            locCtr += 3;
        }
        else if (s.opcode == "RESW") {
            locCtr += 3 * std::stoi(s.operand);
        }
        else if (s.opcode == "RESB") {
            locCtr += std::stoi(s.operand);
        }
        else if (s.opcode == "BYTE") {
            if (s.operand.size() >= 3 && s.operand[0] == 'C' && s.operand[1] == '\'') {
                locCtr += s.operand.size() - 3;
            }
            else if (s.operand.size() >= 3 && s.operand[0] == 'X' && s.operand[1] == '\'') {
                locCtr += (s.operand.size() - 3) / 2;
            }
        }
        else if (OpCode::ValidateOperation(s.opcode)) {
            locCtr += 3;
		pCode = OpCode::GetCode(s.opcode);
        }

	

        mLines.push_back(s);
	
        mTokens.push_back(t);
    }

    return true;
}

std::string Pass1::ParseOperation(const std::string& in, Token t){
	if (!in.empty() && in[0] == '+') {
       		t.GetFlagBits().SetE(1);
    	}
	
	return in.substr(1);
}

std::string Pass1::ParseOperand(const std::string& in, Token t){
	std::string tmp = in;

	if (!tmp.empty() && tmp[0] == '@') {
                t.GetFlagBits().SetN(1); 
                t.GetFlagBits().SetI(0); 
		tmp  = tmp.substr(1);
	}
	if (!tmp.empty() && tmp[0] == '#') {
                t.GetFlagBits().SetN(0); 
                t.GetFlagBits().SetI(1); 
		tmp  = tmp.substr(1);
	}

	if (tmp.size() >= 2 && tmp.substr(tmp.size() - 2) == ",X"){
                t.GetFlagBits().SetX(1); 
		tmp = tmp.substr(0, tmp.size() - 2);
	}

	if(isNumber(tmp)){
		
	}
        	
	return tmp;
}

bool Pass1::isNumber(const std::string& s) {
    if (s.empty()) return false;

    for (char c : s) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

std::vector<SourceLine> Pass1::GetLines() {
    return mLines;
}
