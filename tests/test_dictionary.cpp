#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

#include "dictionary.hpp"

TEST(DictionaryTest, SearchWord) {
    Dictionary dict;
    auto word = std::make_shared<Word>("apple");
    word->add_definition("definition");
    dict.insert(word);

    auto results = dict.search("apple");

    ASSERT_EQ(results.size(), 1);
    EXPECT_EQ(results[0]->get_text(), "apple");
    EXPECT_EQ(results[0]->get_definition().at(0), "definition");

    EXPECT_GE(dict.get_memory_usage(), 0);
}

TEST(DictionaryTest, MatchWords) {
    Dictionary dict;
    dict.insert(std::make_shared<Word>("cat"));
    dict.insert(std::make_shared<Word>("cut"));
    dict.insert(std::make_shared<Word>("coat"));

    auto results = dict.search("c?t");
    std::vector<std::string> words;
    for (const auto &word : results) {
        words.push_back(word->get_text());
    }

    EXPECT_NE(std::find(words.begin(), words.end(), "cat"), words.end());
    EXPECT_NE(std::find(words.begin(), words.end(), "cut"), words.end());
    EXPECT_EQ(std::find(words.begin(), words.end(), "coat"), words.end());

    EXPECT_EQ(words.size(), 2);

    EXPECT_GE(dict.get_memory_usage(), 0);
}

TEST(DictionaryTest, SuggestWords) {
    Dictionary dict;
    dict.insert(std::make_shared<Word>("app"));
    dict.insert(std::make_shared<Word>("apple"));
    dict.insert(std::make_shared<Word>("application"));
    dict.insert(std::make_shared<Word>("banana"));

    auto results = dict.search("appl_");  // Suggests words starting with "appl"
    std::vector<std::string> words;
    for (const auto &word : results) {
        words.push_back(word->get_text());
    }

    EXPECT_NE(std::find(words.begin(), words.end(), "apple"), words.end());
    EXPECT_NE(std::find(words.begin(), words.end(), "application"), words.end());
    EXPECT_EQ(std::find(words.begin(), words.end(), "app"), words.end());
    EXPECT_EQ(std::find(words.begin(), words.end(), "banana"), words.end());

    EXPECT_EQ(words.size(), 2);

    EXPECT_GE(dict.get_memory_usage(), 0);
}

TEST(DictionaryTest, InvalidQueries) {
    Dictionary dict;
    dict.insert(std::make_shared<Word>("cat"));
    dict.insert(std::make_shared<Word>("coat"));

    std::vector<std::string> invalid_queries = {
        "c++t",                 // consecutive wildcards
        "c+?t",                 // consecutive wildcards
        "c*+t",                 // consecutive wildcards
        "c**t",                 // consecutive wildcards
        "c*?t",                 // consecutive wildcards
        "c?t_",                 // mixing ? and _
        "cat__",                // multiple underscores
        "_cat",                 // underscore not at the end
        "ca@t",                 // invalid character
        "",                     // empty string
        std::string(101, 'a'),  // too long
    };

    for (const auto &query : invalid_queries) {
        auto result = dict.search(query);
        EXPECT_TRUE(result.empty()) << "Expected empty for query: " << query;
    }
}
