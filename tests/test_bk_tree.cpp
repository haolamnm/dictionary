#include <gtest/gtest.h>

#include <memory>

#include "bk_tree.hpp"
#include "word.hpp"

TEST(BKTreeTest, InsertAndSearchWords) {
    BK::Tree tree;

    tree.insert(std::make_shared<Word>("book"));
    tree.insert(std::make_shared<Word>("back"));  // 2
    tree.insert(std::make_shared<Word>("boon"));  // 1
    tree.insert(std::make_shared<Word>("cook"));  // 1

    auto results = tree.search("book", 1, 10);

    std::vector<std::string> expected = {"book", "boon", "cook"};
    std::vector<std::string> result_texts;

    for (const auto& word : results) {
        result_texts.push_back(word->get_text());
    }

    for (const std::string& exp : expected) {
        EXPECT_NE(std::find(result_texts.begin(), result_texts.end(), exp), result_texts.end());
    }
}

TEST(BKTreeTest, MemoryUsageAndHeight) {
    BK::Tree tree;
    tree.insert(std::make_shared<Word>("book"));
    tree.insert(std::make_shared<Word>("back"));
    tree.insert(std::make_shared<Word>("boon"));

    EXPECT_GT(tree.get_memory_usage(), 0);
    EXPECT_GE(tree.get_height(), 1);
}
