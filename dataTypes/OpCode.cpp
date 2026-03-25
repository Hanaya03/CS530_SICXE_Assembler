#include <fstream>
#include <sstream>
#include <iostream>
#include <typeinfo>
#include <cstdio>
#include "OpCode.h"

std::unordered_map<std::string, Code> OpCode::mOpTable;

void OpCode::PopulateTable(){

	std::ifstream file("data/opcodes.txt");
	std::string name;
	int hex, format, length;

	while (file >> name >> std::hex >> hex >> format >> length) {		
		//printf("these should all be ints %d, %d, %d\n", hex, format, length);
		//std::cout << typeid(name).name() << std::endl;
		mOpTable["ADD"] = Code(hex, format, length);
	}
}

bool OpCode::IsCodeInTable(std::string c){
	if(mOpTable.find(c) != mOpTable.end())
		return true;
	return false;
}
