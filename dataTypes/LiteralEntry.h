#ifndef LITERALENTRY_H
#define LITERALENTRY_H

#include <string>

struct LiteralEntry {
	bool assigned;
        std::string raw;       // what was in the operand field, e.g "=c'EOF'"
        std::string name;       // content between quotes, e.g. "EOF"
	std::string operandHex; // hex-encoded bytes, e.g. "454F46"
	int address;
	int length;
};
#endif
