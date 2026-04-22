/************************************************
* LiteralEntry.h								*
*												*
* Hector Anaya 826734851        				*
* Emiliano Nolasco 130310255        			*
* Ethan Fudge 827570933         				*
*												*
* CS530, Spring 2026							*
* Assignment #2, SICXE Assembler				*
*************************************************/
#ifndef LITERALENTRY_H
#define LITERALENTRY_H

#include <string>

struct LiteralEntry {
	bool assigned;			// has this literal been emitted?
    std::string raw;       	// what was in the operand field, e.g "=c'EOF'"
    std::string name;       // content between quotes, e.g. "EOF"
	std::string operandHex; // hex-encoded bytes, e.g. "454F46"
	int address;			// literal's assigned offset
	int length;				// length of the literal
	int blockNumber;		// the block that the literal was used in
};
#endif
