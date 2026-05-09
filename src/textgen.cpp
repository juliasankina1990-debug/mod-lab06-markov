#include "textgen.h"
#include <sstream>
#include <algorithm>

TextGen::TextGen() : rng(std::random_device{}()) {}

void TextGen::setSeed(unsigned int seed) {
    rng.seed(seed);
}

void TextGen::learn(std::istream& in) {
    statetab.clear();
    allPrefixes.clear();

    prefix current;
    std::string word;

    // читаем первые npref слов для стартового префикса
    for (int i = 0; i < NPREF && in >> word; ++i) {
        current.push_back(word);
    }
    if (static_cast<int>(current.size()) < NPREF) return; // мало слов

    allPrefixes.push_back(current);

    // обрабатываем оставшийся текст
    while (in >> word) {
        statetab[current].push_back(word);
        current.pop_front();
        current.push_back(word);
        allPrefixes.push_back(current);
    }
}

std::string TextGen::generate(int maxWords) {
    if (statetab.empty()) return "";

    // Случайный выбор первого префикса
    std::uniform_int_distribution<size_t> prefixDist(0, allPrefixes.size() - 1);
    prefix current = allPrefixes[prefixDist(rng)];

    std::ostringstream out;
    out << current[0];
    for (size_t i = 1; i < current.size(); ++i) {
        out << " " << current[i];
    }

    int generated = NPREF;
    while (generated < maxWords) {
        auto it = statetab.find(current);
        if (it == statetab.end()) break; // нет продолжения

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
