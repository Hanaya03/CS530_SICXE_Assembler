#include <cstdio>
#include "Code.h"
#include "OpCode.h"

int main(){
	Code a;
	OpCode::PopulateTable(); 
	printf("new Code object a has values hex: %d, length: %d, and format: %d\n", a.GetHex(), a.GetLength(), a.GetFormat());
	printf("is ADD in the opcode table? %b", OpCode::IsCodeInTable("ADD"));
}
