#include <cstdio>
#include "Code.h"
#include "OpCode.h"
#include "Label.h"
#include "Tokens.h"
#include "Pass1.h"

int main(){
	Pass1::ReadFile("examples/P2sample.sic");
}
