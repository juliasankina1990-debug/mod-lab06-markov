// Copyright 2026 GHA Test Team

#include <iostream>
#include <fstream>
#include <string>
#include "textgen.h"

int main() {
    std::ifstream infile("input.txt");
    if (!infile) {
        std::cerr << "Error: cannot open input.txt\n";
        return 1;
    }

    TextGen gen;
    gen.learn(infile);

    if (gen.getStatetab().empty()) {
        std::cerr << "Error: not enough text to learn\n";
        return 1;
    }

    std::string result = gen.generate(MAXGEN);

    std::ofstream outfile("result/gen.txt");
    if (!outfile) {
        std::cerr << "Error: cannot write result/gen.txt\n";
        return 1;
    }
    outfile << result;

    std::cout << "Generated " << MAXGEN
              << "-word text saved to result/gen.txt" << std::endl;
    return 0;
}
