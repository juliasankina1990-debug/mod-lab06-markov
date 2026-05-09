// Copyright 2026 GHA Test Team
#include "textgen.h"

#include <sstream>
#include <string>
#include <algorithm>

#include <gtest/gtest.h>

// 1. Создание префикса из двух слов
TEST(TextGenTest, PrefixCreation) {
    prefix p;
    p.push_back("hello");
    p.push_back("world");
    ASSERT_EQ(p.size(), 2u);
    ASSERT_EQ(p[0], "hello");
    ASSERT_EQ(p[1], "world");
}

// 2. Формирование записи "префикс-суффикс"
TEST(TextGenTest, StatetabEntry) {
    std::istringstream input("a b c");
    TextGen gen;
    gen.learn(input);
    const auto& tab = gen.getStatetab();
    prefix key = {"a", "b"};
    ASSERT_GT(tab.count(key), 0u);
    ASSERT_EQ(tab.at(key).size(), 1u);
    ASSERT_EQ(tab.at(key)[0], "c");
}

// 3. Выбор единственного суффикса (использование ПСЧ)
TEST(TextGenTest, SingleSuffixChoice) {
    std::istringstream input("x y z");
    TextGen gen;
    gen.setSeed(42);
    gen.learn(input);
    std::string text = gen.generate(10);
    ASSERT_EQ(text, "x y z");
}

// 4. Несколько вариантов суффиксов
TEST(TextGenTest, MultipleSuffixes) {
    std::istringstream input("a b c a b d");
    TextGen gen;
    gen.learn(input);
    const auto& tab = gen.getStatetab();
    prefix key = {"a", "b"};
    ASSERT_EQ(tab.at(key).size(), 2u);
    bool hasC = false, hasD = false;
    for (const auto& s : tab.at(key)) {
        if (s == "c") hasC = true;
        if (s == "d") hasD = true;
    }
    ASSERT_TRUE(hasC && hasD);
}

// 5. Генерация текста заданной длины (ручное заполнение таблицы)
TEST(TextGenTest, GenerationLength) {
    std::string data = "x y z x y z x y z x y z";
    std::istringstream input(data);
    TextGen gen;
    gen.setSeed(1);
    gen.learn(input);
    std::string text = gen.generate(10);
    std::istringstream iss(text);
    int count = 0;
    std::string w;
    while (iss >> w) ++count;
    ASSERT_EQ(count, 10);
}

// 6. Остановка при отсутствии продолжения
TEST(TextGenTest, GenerationStopsAtEnd) {
    std::istringstream input("a b c");
    TextGen gen;
    gen.setSeed(123);
    gen.learn(input);
    std::string text = gen.generate(100);
    std::istringstream iss(text);
    int count = 0;
    std::string w;
    while (iss >> w) ++count;
    ASSERT_EQ(count, 3);
}

// 7. Пустой входной поток
TEST(TextGenTest, EmptyInput) {
    std::istringstream input("");
    TextGen gen;
    gen.learn(input);
    ASSERT_TRUE(gen.getStatetab().empty());
    ASSERT_EQ(gen.generate(10), "");
}

// 8. Слишком короткий текст (меньше NPREF слов)
TEST(TextGenTest, TooShortInput) {
    std::istringstream input("single");
    TextGen gen;
    gen.learn(input);
    ASSERT_TRUE(gen.getStatetab().empty());
}

// 9. Детерминированность генерации при фиксированном seed
TEST(TextGenTest, DeterministicGeneration) {
    std::istringstream input("m n o m n p");
    TextGen gen;
    gen.setSeed(77);
    gen.learn(input);
    std::string firstRun = gen.generate(5);
    TextGen gen2;
    gen2.setSeed(77);
    std::istringstream input2("m n o m n p");
    gen2.learn(input2);
    std::string secondRun = gen2.generate(5);
    ASSERT_EQ(firstRun, secondRun);
}

// 10. Проверка структуры таблицы после обучения вручную
TEST(TextGenTest, ManualTableStructure) {
    std::istringstream input("cat dog fish cat dog bird");
    TextGen gen;
    gen.learn(input);
    const auto& tab = gen.getStatetab();
    prefix cd = {"cat", "dog"};
    ASSERT_EQ(tab.count(cd), 1u);
    const auto& v = tab.at(cd);
    ASSERT_EQ(v.size(), 2u);
    ASSERT_NE(std::find(v.begin(), v.end(), std::string("fish")), v.end());
    ASSERT_NE(std::find(v.begin(), v.end(), std::string("bird")), v.end());
}
