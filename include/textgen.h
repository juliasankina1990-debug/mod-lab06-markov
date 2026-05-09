// Copyright 2026 GHA Test Team
#pragma once

#include <string>
#include <deque>
#include <map>
#include <vector>
#include <random>

const int NPREF = 2;
const int MAXGEN = 1000;

typedef std::deque<std::string> prefix;

class TextGen {
 public:
    TextGen();

    void setSeed(unsigned int seed);
    void learn(std::istream& in);
    std::string generate(int maxWords = MAXGEN);
    const std::map<prefix, std::vector<std::string>>& getStatetab() const;

 private:
    std::map<prefix, std::vector<std::string>> statetab;
    std::mt19937 rng;
};
