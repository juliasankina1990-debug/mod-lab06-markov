// Copyright 2026 GHA Test Team

#include "textgen.h"
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

TextGen::TextGen() : rng(std::random_device { } ())  { }

void TextGen::setSeed(unsigned int seed) {
    rng.seed(seed);
}

void TextGen::learn(std::istream& in) {
    statetab.clear();
    prefix current;
    std::string word;
    for (int i = 0; i < NPREF && in >> word; ++i) {
        current.push_back(word);
    }
    if (static_cast<int>(current.size()) < NPREF) return;

    while (in >> word) {
        statetab[current].push_back(word);
        current.pop_front();
        current.push_back(word);
    }
}

std::string TextGen::generate(int maxWords) {
    if (statetab.empty()) return "";

    std::vector<prefix> keys;
    for (const auto& entry : statetab) {
        keys.push_back(entry.first);
    }

    std::ostringstream out;
    int generated = 0;

    std::uniform_int_distribution<size_t> prefixDist(0, keys.size() - 1);
    prefix current = keys[prefixDist(rng)];
    for (const auto& w : current) {
        if (generated >= maxWords) break;
        if (generated > 0) out << " ";
        out << w;
        ++generated;
    }

    while (generated < maxWords) {
        auto it = statetab.find(current);
        if (it == statetab.end()) {
            current = keys[prefixDist(rng)];
            for (const auto& w : current) {
                if (generated >= maxWords) break;
                out << " " << w;
                ++generated;
            }
            continue;
        }
        const auto& suffixes = it->second;
        std::uniform_int_distribution<size_t> suffixDist(0, suffixes.size() - 1);
        const std::string& next = suffixes[suffixDist(rng)];
        out << " " << next;
        current.pop_front();
        current.push_back(next);
        ++generated;
    }
    return out.str();
}

const std::map<prefix, std::vector<std::string>>& TextGen::getStatetab() const {
    return statetab;
}
