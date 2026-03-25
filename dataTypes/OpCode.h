#ifndef OPCODE_H
#define OPCODE_H

#include <string>
#include <unordered_map>
#include "Code.h"

class OpCode{
	private:
		static std::unordered_map<std::string, Code> mOpTable;
	public:
		static void PopulateTable();
		static bool IsCodeInTable(std::string);
};

#endif
