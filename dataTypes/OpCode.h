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
		
		/***************************************************************************
		 * Read the file ./data/opcodes.txt and populate the mOpTable hash table 
		 * with the read values
		 *
		 * Input: 
		 * 	None
		 *
		 * Output:
		 * 	None
		 **************************************************************************/
		static void PopulateTable();
		
		/***************************************************************************
		 * Given some input string, check if that key exists in the mOpTable hash table
		 *      
		 * Input:    
		 *     std::string c - The operation that the parser is currently parsing
		 *
		 * Output:
		 *	bool - True means the inputted operation exists in the hashtable
		 *		and is therefore a valid operation, else it's an invalid
		 *		operation and parser should flag accordingly
		 ***************************************************************************/
		static bool ValidateOperation(std::string);

		static Code* GetCode(std::string);
};

#endif
