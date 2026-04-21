#include <fstream>
#include <sstream>
#include <iostream>
#include <typeinfo>
#include <cstdio>
#include "OpCode.h"

std::unordered_map<std::string, Code> OpCode::mOpTable;

void OpCode::PopulateTable(){
	// Read from file instead of hardcoding — easier to maintain and extend
	std::ifstream file("data/opcodes.txt");
	std::string name;
	int hex, format, length;

	while (file >> name >> std::hex >> hex >> format >> length) {		
		mOpTable[name] = Code(hex, format, length);
	}
}

bool OpCode::ValidateOperation(std::string c){
	
	if(mOpTable.find(c) != mOpTable.end())
		return true;
	return false;
}

Code* OpCode::GetCode(std::string c){
	return &mOpTable[c];
}
