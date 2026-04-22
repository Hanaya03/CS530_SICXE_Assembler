/************************************************
* OpCode.h										*
*												*
* Hector Anaya 826734851        				*
* Emiliano Nolasco 130310255        			*
* Ethan Fudge 827570933         				*
*												*
* CS530, Spring 2026							*
* Assignment #2, SICXE Assembler				*
*************************************************/
#ifndef OPCODE_H
#define OPCODE_H

#include <string>
#include <unordered_map>
#include "Code.h"

/********************************************************************************
 * Static class to hold hash table of the mnemonic operations and their 
 * associated Code object.
 *
 * Member variables:
 * 	mOpTable - Hash table with std::strings as the key. Associated value is 
 * 			a Code object containing its hex, format and length
 *
 ********************************************************************************/
class OpCode{
	private:
		static std::unordered_map<std::string, Code> mOpTable;
	public:
		static void PopulateTable();                  // loads mOpTable from data/opcodes.txt
        static bool ValidateOperation(std::string);   // returns true if mnemonic exists in mOpTable
        static Code* GetCode(std::string);            // returns pointer to Code object for given mnemonic
};

#endif
