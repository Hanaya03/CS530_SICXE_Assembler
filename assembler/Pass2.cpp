#include "Pass2.h"
 
int                       Pass2::mBaseReg  = -1;
std::vector<LiteralEntry> Pass2::mLiteralTable;
std::string               Pass2::mProgName = "";
int                       Pass2::mProgStart = 0;
int                       Pass2::mProgEnd   = 0;
 
// Utilities 
static std::string toHex(int val, int digits) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%0*X", digits, val);
    return buf;
}
 
static int regNum(const std::string& r) {
    if (r=="A")  return 0; if (r=="X")  return 1; if (r=="L")  return 2;
    if (r=="B")  return 3; if (r=="S")  return 4; if (r=="T")  return 5;
    if (r=="F")  return 6; if (r=="PC") return 8; if (r=="SW") return 9;
    return 0;
}
 
// extract content between single quotes: "=C'EOF'" -> "EOF"
static std::string litContent(const std::string& raw) {
    size_t a = raw.find('\''), b = raw.rfind('\'');
    return (a != std::string::npos && b != a) ? raw.substr(a+1, b-a-1) : "";
}
 
// convert literal to hex bytes: =C'EOF' -> "454F46", =X'05' -> "05"
static std::string litToHex(const std::string& raw) {
    std::string content = litContent(raw);
    std::ostringstream ss;
    if (raw.size() > 1 && raw[1] == 'C') {
        for (char c : content)
            ss << std::uppercase << std::hex << std::setfill('0') << std::setw(2) << (int)(unsigned char)c;
    } else {
        for (char c : content) ss << (char)std::toupper(c);
    }
    return ss.str();
}
 
static int litLen(const std::string& raw) { //length in bytes of the literal
    std::string c = litContent(raw);
    return (raw.size() > 1 && raw[1] == 'C') ? (int)c.size() : (int)c.size() / 2;
}
 
// Literal pool: collect unique literals, assign addresses after last line or LTORG directive
 
void Pass2::CollectLiterals(const std::vector<SourceLine>& lines) {
    mLiteralTable.clear();
    int poolAddr = 0;
 
    for (auto& s : lines) {
        if (s.isComment || s.opcode.empty() || s.opcode == "END") {
            if (s.opcode == "END") break;
            continue;
        }
        // track the address
        if      (s.opcode == "RESW") poolAddr = s.address + s.mOperand.mValue * 3;
        else if (s.opcode == "RESB") poolAddr = s.address + s.mOperand.mValue;
        else if (s.opcode == "WORD") poolAddr = s.address + 3;
        else if (s.opcode == "BYTE") poolAddr = s.address + litLen(s.mOperand.mLabel);
        else if (s.mBits.e)          poolAddr = s.address + 4;
        else if (OpCode::ValidateOperation(s.opcode)) poolAddr = s.address + 3;
    }
 
    for (auto& s : lines) {
        if (!s.mOperand.isLiteral) continue;
        std::string name = litContent(s.mOperand.mLabel);
        bool dup = false;
        for (auto& e : mLiteralTable) if (e.name == name) { dup = true; break; }
        if (dup) continue;
        LiteralEntry e { name, litToHex(s.mOperand.mLabel), poolAddr, litLen(s.mOperand.mLabel) };
        poolAddr += e.length;
        mLiteralTable.push_back(e);
    }
    mProgEnd = poolAddr;
}
 
// Resolve target address (symbol or literal)
 
static int resolve(const SourceLine& s, const std::unordered_map<std::string,int>& sym,
                   const std::vector<LiteralEntry>& lits) {
    if (s.mOperand.isLiteral) {
        std::string name = litContent(s.mOperand.mLabel);
        for (auto& l : lits) if (l.name == name) return l.address;
    }
    if (s.mOperand.isLabel) {
        auto it = sym.find(s.mOperand.mLabel);
        if (it != sym.end()){ 
		std::cout << "Line:" << toHex(s.address, 4) << "Searching for " << s.mOperand.mLabel << "(" << toHex(it->second, 4) << ") with block offset: " <<  toHex(PBlocks::GetBlock(s.mBlock)->GetLength(),4) << "\n";
		return it->second + PBlocks::GetBlock(s.mBlock)->GetLength();
	    }
	}
    return s.mOperand.mValue;
}
 
// Instruction encoding (all formats)
 
static std::string encodeInstr(const SourceLine& s,
                                const std::unordered_map<std::string,int>& sym,
                                const std::vector<LiteralEntry>& lits,
                                int baseReg) {
    Code* code = s.pCode;
    int op  = code->GetHex();
    int fmt = code->GetFormat();
 
    // Format 4 (+prefix)
    if (s.mBits.e) {
        int addr = resolve(s, sym, lits);
        int b1 = (op & 0xFC) | (s.mBits.n << 1) | s.mBits.i;
        int b2 = (s.mBits.x << 7) | 0x10 | ((addr >> 16) & 0x0F);
        return toHex((b1 << 24) | (b2 << 16) | ((addr >> 8 & 0xFF) << 8) | (addr & 0xFF), 8);
    }
 
    // Format 1
    if (fmt == 1) return toHex(op, 2);
 
    // Format 2
    if (fmt == 2) {
        std::string ops = s.mOperand.mLabel;
        int r1 = 0, r2 = 0;
        size_t comma = ops.find(',');
        if (comma != std::string::npos) { r1 = regNum(ops.substr(0, comma)); r2 = regNum(ops.substr(comma+1)); }
        else r1 = regNum(ops);
        return toHex((op << 8) | (r1 << 4) | r2, 4);
    }
 
    // Format 3
    int n=s.mBits.n, i=s.mBits.i, x=s.mBits.x, b=0, p=0, disp=0;
    if (!s.mOperand.mLabel.empty() || s.mOperand.isLiteral) {
        if (!s.mOperand.isLabel && !s.mOperand.isLiteral) {
            disp = s.mOperand.mValue;                        // immediate numeric
        } else {
            int target = resolve(s, sym, lits);
            int pcDisp = target - (s.address + 3);
            if (pcDisp >= -2048 && pcDisp <= 2047) { p = 1; disp = pcDisp & 0xFFF; }
            else if (baseReg >= 0)                 { b = 1; disp = target - baseReg; }
            else{std::cerr << "Error:" << toHex(s.address, 4) << ": can't reach operand " << s.mOperand.mLabel << "\n"<< "PC relative displacement: " << toHex(pcDisp, 4)<< "\n";}
        }
    }
    int b1 = (op & 0xFC) | (n << 1) | i;
    int b2 = (x << 7) | (b << 6) | (p << 5) | ((disp >> 8) & 0x0F);
    return toHex((b1 << 16) | (b2 << 8) | (disp & 0xFF), 6);
}
 

// GenerateOutput
 
bool Pass2::GenerateOutput(const std::string& sourceFile) {
    // write to output/ folder
    std::string filename = sourceFile.substr(sourceFile.rfind('/')+1); // strip directory
    filename = filename.substr(0, filename.rfind('.'));                 // strip extension
    system("mkdir -p output");
    std::string outBase = "output/" + filename;
    std::ofstream lst(outBase + ".l"), st(outBase + ".st");
    if (!lst || !st) { std::cerr << "Error: cannot open output files\n"; return false; }
 
    auto lines  = Pass1::GetAllLines();
    auto symTab = Pass1::GetSymTab();
    CollectLiterals(lines);
    mBaseReg = -1;
 
    for (auto& s : lines) {
        if (s.isComment)    { lst << s.originalLine << "\n"; continue; }
        if (s.opcode.empty()) continue;
 
        std::string addr = toHex(s.address, 4);
        std::string obj  = "";
 
        if (s.opcode == "START") {
            mProgName = s.label; mProgStart = s.address;
            lst << addr << "\t" << s.label << "\t" << s.opcode << "\t" << s.mOperand.mLabel << "\n";
            continue;
        }
 
        if (s.opcode == "END") {
            // emit literal pool
            for (auto& lit : mLiteralTable)
                lst << toHex(lit.address,4) << "\t*\t=C'" << lit.name << "'\t\t" << lit.operandHex << "\n";
            lst << "\t\t" << s.opcode << "\t" << s.mOperand.mLabel << "\n";
            break;
        }
 
        if (s.opcode == "BASE") {
            auto it = symTab.find(s.mOperand.mLabel);
            if (it != symTab.end()) mBaseReg = it->second;
            lst << addr << "\t" << s.label << "\t" << s.opcode << "\t" << s.mOperand.mLabel << "\n";
            continue;
        }
 
        if (s.opcode == "NOBASE") { mBaseReg = -1; lst << addr << "\t\t" << s.opcode << "\n"; continue; }

        if (s.opcode == "LTORG") { mBaseReg = -1; lst << addr << "\t\t" << s.opcode << "\n"; continue; }
 
        if (s.opcode == "RESW" || s.opcode == "RESB") {
            lst << addr << "\t" << s.label << "\t" << s.opcode << "\t" << s.mOperand.mValue << "\n";
            continue;
        }
 
        if (s.opcode == "WORD") {
            int val = s.mOperand.isLabel ? symTab.count(s.mOperand.mLabel) ? symTab.at(s.mOperand.mLabel) : 0
                                         : s.mOperand.mValue;
            obj = toHex(val, 6);
        } else if (s.opcode == "BYTE") {
            obj = litToHex(s.mOperand.mLabel[0] == 'C' ? "=C'" + s.mOperand.mLabel.substr(2) : "=X'" + s.mOperand.mLabel.substr(2));
        } else if (OpCode::ValidateOperation(s.opcode)) {
            obj = encodeInstr(s, symTab, mLiteralTable, mBaseReg);
        }
 
        // build operand display string
        std::string opDisplay;
        if (s.mOperand.isLiteral)                          
            opDisplay = s.mOperand.mLabel;
        else if (s.mBits.n==1 && s.mBits.i==0)            
            opDisplay = "@" + s.mOperand.mLabel;
        else if (s.mBits.n==0 && s.mBits.i==1)            
            opDisplay = "#" + s.mOperand.mLabel;
        else                                               
            opDisplay = s.mOperand.mLabel;
        if (s.mBits.x) opDisplay += ",X";
 
        std::string prefix = s.mBits.e ? "+" : "";
        lst << addr << "\t" << s.label << "\t" << prefix << s.opcode << "\t" << opDisplay << "\t" << obj << "\t" << s.mBlock << "\n";
    }
 
    // Symbol table
    st << "Symbol\t\tValue\n";
    st << "----------------------\n";
    std::vector<std::pair<std::string,int>> syms(symTab.begin(), symTab.end());
    std::sort(syms.begin(), syms.end(), [](auto& a, auto& b){ return a.second < b.second; });
    for (auto& kv : syms)
        st << kv.first << "\t\t" << toHex(kv.second, 6) << "\n";
    st << "\nLiteral\t\tHex\t\tAddress\t\tLength\n";
    st << "----------------------------------------------\n";
    for (auto& lit : mLiteralTable)
        st << lit.name << "\t\t" << lit.operandHex << "\t\t" << toHex(lit.address,4) << "\t\t" << lit.length << "\n";
 
    std::cout << "Pass 2 complete: " << outBase << ".l / " << outBase << ".st\n";
    return true;
}
