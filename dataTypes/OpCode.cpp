#include <fstream>
#include <sstream>
#include <iostream>
#include <typeinfo>
#include <cstdio>
#include "OpCode.h"

std::unordered_map<std::string, Code> OpCode::mOpTable;

/***************************************************************************
* Did NOT want to hard code the OpCode table in the code so I decided to 
* make an algorithm to read them from a file and populate the table 
* automatically. Easier to maintain, imo.
**************************************************************************/
void OpCode::PopulateTable(){

	std::ifstream file("data/opcodes.txt");
	std::string name;
	int hex, format, length;

	while (file >> name >> std::hex >> hex >> format >> length) {		
		mOpTable[name] = Code(hex, format, length);
	}
}

/***************************************************************************
* kinda, sorta, not really a getter 
***************************************************************************/
bool OpCode::ValidateOperation(std::string c){
	
	if(mOpTable.find(c) != mOpTable.end())
		return true;
	return false;
}

/*****************************************************************************
 * actually is a getter
 *************************************************************************/
Code* OpCode::GetCode(std::string c){
	return &mOpTable[c];
}
