#include <cstdio>
#include "Code.h"
#include "OpCode.h"
#include "Label.h"
#include "Tokens.h"

int main(){
	Code a;
	OpCode::PopulateTable(); 
	Label *newLabel = new Label(0, 'n');
	printf("new Code object a has values hex: %d, length: %d, and format: %d\n", a.GetHex(), a.GetLength(), a.GetFormat());
	printf("is ADD in the opcode table? %d\n", OpCode::ValidateOperation("ADD"));
	printf("is MUL in the opcode table? %d\n", OpCode::ValidateOperation("MUL"));
	Token newToken(0, 1, "Sample source statement", OpCode::GetCode("Add"), newLabel);
	printf("newToken has values %d, %d, %s\n", newToken.GetAddress(), newToken.GetErrId(), newToken.GetSrcStmt().c_str());
}
