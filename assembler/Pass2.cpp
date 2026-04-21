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
std::string Pass2::litContent(const std::string& raw) {
    size_t a = raw.find('\''), b = raw.rfind('\'');
    return (a != std::string::npos && b != a) ? raw.substr(a+1, b-a-1) : "";
}

// convert literal to hex bytes: =C'EOF' -> "454F46", =X'05' -> "05"
std::string Pass2::litToHex(const std::string& raw) {
    std::string content = Pass2::litContent(raw);
    std::ostringstream ss;
    if (raw.size() > 1 && raw[1] == 'C') {
        for (char c : content)
            ss << std::uppercase << std::hex << std::setfill('0') << std::setw(2) << (int)(unsigned char)c;
    } else {
        for (char c : content) ss << (char)std::toupper(c);
    }
    return ss.str();
}

int Pass2::litLen(const std::string& raw) {
    std::string c = Pass2::litContent(raw);
    return (raw.size() > 1 && raw[1] == 'C') ? (int)c.size() : (int)c.size() / 2;
}

// Helper: get absolute address of a symbol, respecting flag
static int symAddr(const Label& lbl) {
    if (lbl.GetFlag() == 'A') return lbl.GetAddress();
    return lbl.GetAddress() + PBlocks::GetBlock(lbl.GetBlock())->GetStartAddr();
}

// Resolve target address (symbol or literal)
static int resolve(const SourceLine& s,
                   const std::unordered_map<std::string,Label>& sym,
                   const std::vector<LiteralEntry>& lits,
                   bool& found) {
    found = true;

    if (s.mOperand.isLiteral) {
        std::string name = Pass2::litContent(s.mOperand.mLabel);
        for (auto& l : lits) {
            if (l.name == name)
                return l.address + PBlocks::GetBlock(l.blockNumber)->GetStartAddr();
        }
        found = false;
        return 0;
    }

    if (s.mOperand.isLabel) {
        auto it = sym.find(s.mOperand.mLabel);
        if (it != sym.end())
            return symAddr(it->second);
        found = false;
        return 0;
    }

    return s.mOperand.mValue;
}

// evaluateExpression — public static so Pass1 can call it too
int Pass2::evaluateExpression(const std::string& expr,
                               const std::unordered_map<std::string, Label>& sym) {
    size_t plusPos  = expr.find('+');
    size_t minusPos = expr.find('-');
    size_t multPos  = expr.find('*');
    size_t divPos   = expr.find('/');

    char op = 0;
    size_t opPos = std::string::npos;

    // else-if so first operator wins (left-to-right)
    if      (plusPos  != std::string::npos) { op = '+'; opPos = plusPos;  }
    else if (minusPos != std::string::npos) { op = '-'; opPos = minusPos; }
    else if (multPos  != std::string::npos) { op = '*'; opPos = multPos;  }
    else if (divPos   != std::string::npos) { op = '/'; opPos = divPos;   }

    if (op == 0) {
        if (sym.count(expr)) return symAddr(sym.at(expr));
        return std::stoi(expr);
    }

    std::string left  = expr.substr(0, opPos);
    std::string right = expr.substr(opPos + 1);

    int leftVal  = sym.count(left)  ? symAddr(sym.at(left))  : std::stoi(left);
    int rightVal = sym.count(right) ? symAddr(sym.at(right)) : std::stoi(right);

    switch (op) {
        case '+': return leftVal + rightVal;
        case '-': return leftVal - rightVal;
        case '*': return leftVal * rightVal;
        case '/':
            if (rightVal == 0) {
                std::cerr << "Error: division by zero in expression: " << expr << "\n";
                return 0;
            }
            return leftVal / rightVal;
        default: return 0;
    }
}

// Instruction encoding (all formats)
static std::string encodeInstr(const SourceLine& s,
                                const std::unordered_map<std::string,Label>& sym,
                                const std::vector<LiteralEntry>& lits,
                                int baseReg) {
    Code* code = s.pCode;
    int op  = code->GetHex();
    int fmt = code->GetFormat();

    // Format 4 (+prefix)
    if (s.mBits.e) {
        bool found = false;
        int addr = resolve(s, sym, lits, found);
        if (!found) {
            std::cerr << "Error:" << toHex(s.address, 4)
                      << ": undefined symbol " << s.mOperand.mLabel << "\n";
            return "";
        }
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
        if (comma != std::string::npos) {
            r1 = regNum(ops.substr(0, comma));
            r2 = regNum(ops.substr(comma+1));
        } else {
            r1 = regNum(ops);
        }
        return toHex((op << 8) | (r1 << 4) | r2, 4);
    }

    // Format 3
    int n=s.mBits.n, i=s.mBits.i, x=s.mBits.x, b=0, p=0, disp=0;
    if (!s.mOperand.mLabel.empty() || s.mOperand.isLiteral) {
        if (!s.mOperand.isLabel && !s.mOperand.isLiteral) {
            disp = s.mOperand.mValue;   // immediate numeric
        } else {
            bool found = false;
            int target = resolve(s, sym, lits, found);
            if (!found) {
                std::cerr << "Error:" << toHex(s.address, 4)
                          << ": undefined symbol " << s.mOperand.mLabel << "\n";
                return "";
            }
            int pcDisp = target - (s.address + PBlocks::GetBlock(s.mBlock)->GetStartAddr() + 3);
            if      (pcDisp >= -2048 && pcDisp <= 2047) { p = 1; disp = pcDisp & 0xFFF; }
            else if (baseReg >= 0)                      { b = 1; disp = target - baseReg; }
            else {
                std::cerr << "Error:" << toHex(s.address, 4)
                          << ": can't reach operand " << s.mOperand.mLabel
                          << "\nPC relative displacement: " << toHex(pcDisp, 4) << "\n";
                return "";
            }
        }
    }
    int b1 = (op & 0xFC) | (n << 1) | i;
    int b2 = (x << 7) | (b << 6) | (p << 5) | ((disp >> 8) & 0x0F);
    return toHex((b1 << 16) | (b2 << 8) | (disp & 0xFF), 6);
}

// GenerateOutput
bool Pass2::GenerateOutput(const std::string& sourceFile) {
    std::string filename = sourceFile.substr(sourceFile.rfind('/')+1);
    filename = filename.substr(0, filename.rfind('.'));
    system("mkdir -p output");
    std::string outBase = "output/" + filename;
    std::ofstream lst(outBase + ".l"), st(outBase + ".st");
    if (!lst || !st) { std::cerr << "Error: cannot open output files\n"; return false; }

    auto lines  = Pass1::GetAllLines();
    auto symTab = Pass1::GetSymTab();
    mLiteralTable = Pass1::GetLitTab();
    mBaseReg = -1;

    for (auto& s : lines) {
        if (s.isComment) { lst << s.originalLine << "\n"; continue; }

        if (!s.originalLine.empty() && s.originalLine[0] == '*') {
            lst << toHex(s.address, 4) << "\t" << s.originalLine;
            continue;
        }

        if (s.opcode.empty()) continue;

        std::string addr = toHex(s.address, 4);
        std::string obj  = "";

        if (s.opcode == "START") {
            mProgName = s.label; mProgStart = s.address;
            lst << addr << "\t" << s.label << "\t" << s.opcode << "\t" << s.mOperand.mLabel << "\n";
            continue;
        }

        if (s.opcode == "END" || s.opcode == "LTORG") {
            lst << "\t\t" << s.opcode << "\t" << s.mOperand.mLabel << "\n";
            continue;
        }

        if (s.opcode == "BASE") {
            auto it = symTab.find(s.mOperand.mLabel);
            if (it != symTab.end())
                mBaseReg = symAddr(it->second);
            lst << addr << "\t" << s.label << "\t" << s.opcode << "\t" << s.mOperand.mLabel << "\n";
            continue;
        }

        if (s.opcode == "NOBASE") {
            mBaseReg = -1;
            lst << addr << "\t\t" << s.opcode << "\n";
            continue;
        }

        if (s.opcode == "USE") {
            lst << addr << "\t\t" << s.opcode << "\t" << s.mOperand.mLabel << "\n";
            continue;
        }

        if (s.opcode == "RESW" || s.opcode == "RESB") {
            lst << addr << "\t" << s.label << "\t" << s.opcode << "\t" << s.mOperand.mValue << "\n";
            continue;
        }

        if (s.opcode == "EQU") {
            lst << addr << "\t" << s.label << "\t" << s.opcode << "\t" << s.mOperand.mLabel << "\n";
            continue;
        }

        if (s.opcode == "WORD") {
            int val = 0;
            if (s.mOperand.isExpression)
                val = evaluateExpression(s.mOperand.mLabel, symTab);
            else if (s.mOperand.isLabel && symTab.count(s.mOperand.mLabel))
                val = symAddr(symTab.at(s.mOperand.mLabel));
            else
                val = s.mOperand.mValue;
            obj = toHex(val, 6);
        } else if (s.opcode == "BYTE") {
            obj = litToHex("=" + s.mOperand.mLabel);
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
        lst << addr << "\t" << s.label << "\t" << prefix << s.opcode << "\t" << opDisplay << "\t" << obj << "\n";
    }

    // Symbol table
    st << "CSect\tSymbol\tValue\tLENGTH\tFlags:\n";
    st << "--------------------------------------\n";
    st << mProgName <<"\t\t" << toHex(PBlocks::GetBlock(0)->GetStartAddr(), 6) << "\t" << toHex(PBlocks::GetProgramLength(), 6) << "\n";
    std::vector<std::pair<std::string,Label>> syms(symTab.begin(), symTab.end());
    std::sort(syms.begin(), syms.end(), [](auto& a, auto& b){
        return symAddr(a.second) < symAddr(b.second);
    });
    for (auto& kv : syms)
        st << "\t" << kv.first << "\t\t" << toHex(symAddr(kv.second), 6) << "\t" << kv.second.GetFlag() << "\n";

    // Literal table
    st << "\nLiteral\t\tHex\t\tAddress\t\tLength:\n";
    st << "-------------------------------------------------------\n";
    for (auto& lit : mLiteralTable)
        st << lit.name << "\t\t" << lit.operandHex << "\t\t"
           << toHex(lit.address + PBlocks::GetBlock(lit.blockNumber)->GetStartAddr(), 4)
           << "\t\t" << lit.length << "\n";

    mLiteralTable.clear();
    lines.clear();
    symTab.clear();

    return true;
}
