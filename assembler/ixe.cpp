#include <iostream>
#include "OpCode.h"
#include "Pass1.h"
#include "Pass2.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: No input file provided.\n";
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

	OpCode::PopulateTable();

	for (int i = 1; i < argc; i++) {
		std::string inputFile = argv[i];
	
		Pass1::ReadFile(inputFile);

		if (Pass1::HadError()) {
			std::cerr << "Assembly stopped due to Pass1 errors.\n";
			Pass1::ClearTables();
			continue;
		}
		
		Pass2::GenerateOutput(inputFile);
		Pass1::ClearTables();
	}
}
