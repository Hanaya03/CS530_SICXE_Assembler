#include <iostream>
#include "Pass1.h"
#include "Pass2.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: No input file provided.\n";
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::string inputFile = argv[1];

    std::cout << "Input file: " << inputFile << "\n";

	Pass1::ReadFile(inputFile);
        Pass2::GenerateOutput(inputFile);
}
