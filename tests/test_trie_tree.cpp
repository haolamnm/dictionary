#include <gtest/gtest.h>

#include "trie_tree.hpp"

static std::vector<std::string> to_words(const std::vector<std::shared_ptr<Word>>& input) {
    std::vector<std::string> out;
    for (auto& w : input) out.push_back(w->get_text());
    return out;
}

TEST(TrieTreeTest, InsertAndSearchWords) {
    Trie::Tree tree;
    auto word1 = std::make_shared<Word>("apple");
    auto word2 = std::make_shared<Word>("app");
    tree.insert(word1);
    tree.insert(word2);

    auto found = tree.search("apple");
    EXPECT_NE(found, nullptr);
    EXPECT_EQ(found->get_text(), "apple");

    auto not_found = tree.search("appl");
    EXPECT_EQ(not_found, nullptr);

    found = tree.search("app");
    EXPECT_NE(found, nullptr);
    EXPECT_EQ(found->get_text(), "app");

    EXPECT_GE(tree.get_memory_usage(), 0);
}

TEST(TrieTreeTest, SuggestWords) {
    Trie::Tree tree;
    tree.insert(std::make_shared<Word>("apple"));
    tree.insert(std::make_shared<Word>("app"));
    tree.insert(std::make_shared<Word>("application"));
    tree.insert(std::make_shared<Word>("banana"));

    auto suggestions = tree.suggest("app", 10);
    std::vector<std::string> expected = {"app", "apple", "application"};
    std::vector<std::string> found;
    for (const auto& word : suggestions) {
        found.push_back(word->get_text());
    }

    for (const auto& exp : expected) {
        EXPECT_NE(std::find(found.begin(), found.end(), exp), found.end());
    }
    EXPECT_GE(tree.get_memory_usage(), 0);
}

TEST(TrieTreeTest, MatchSingleCharacter) {
    Trie::Tree tree;
    tree.insert(std::make_shared<Word>("cat"));
    tree.insert(std::make_shared<Word>("cut"));
    tree.insert(std::make_shared<Word>("can"));
    tree.insert(std::make_shared<Word>("cot"));

    auto results = tree.match("c?t", 10);
    auto words = to_words(results);

    // Expect exactly those with one char between c and t
    EXPECT_NE(std::find(words.begin(), words.end(), "cat"), words.end());
    EXPECT_NE(std::find(words.begin(), words.end(), "cut"), words.end());
    EXPECT_NE(std::find(words.begin(), words.end(), "cot"), words.end());
    EXPECT_EQ(words.size(), 3);

    EXPECT_GE(tree.get_memory_usage(), 0);
}

TEST(TrieTreeTest, MatchZeroOrMoreCharacters) {
    Trie::Tree tree;
    tree.insert(std::make_shared<Word>("cat"));
    tree.insert(std::make_shared<Word>("cut"));
    tree.insert(std::make_shared<Word>("covert"));
    tree.insert(std::make_shared<Word>("caveat"));
    tree.insert(std::make_shared<Word>("cta"));  // does not match c*t

    auto results = tree.match("c*t", 10);
    auto words = to_words(results);

    // Should match any word that starts with 'c' and ends with 't'
    std::vector<std::string> expected = {"cat", "cut", "covert", "caveat"};
    for (auto& exp : expected) EXPECT_NE(std::find(words.begin(), words.end(), exp), words.end());
    EXPECT_EQ(words.size(), expected.size());

    EXPECT_GE(tree.get_memory_usage(), 0);
}

TEST(TrieTreeTest, MatchOneOrMoreCharacters) {
    Trie::Tree tree;
    tree.insert(std::make_shared<Word>("caught"));
    tree.insert(std::make_shared<Word>("cut"));
    tree.insert(std::make_shared<Word>("cart"));
    tree.insert(std::make_shared<Word>("cat"));

    auto results = tree.match("ca+t", 10);
    auto words = to_words(results);

    // 'ca+t' = 'ca' + at least one char + 't'
    EXPECT_NE(std::find(words.begin(), words.end(), "caught"), words.end());
    EXPECT_NE(std::find(words.begin(), words.end(), "cart"), words.end());
    EXPECT_EQ(std::find(words.begin(), words.end(), "cut"), words.end());
    EXPECT_EQ(std::find(words.begin(), words.end(), "cat"), words.end());
    EXPECT_EQ(words.size(), 2);

    EXPECT_GE(tree.get_memory_usage(), 0);
}

TEST(TrieTreeTest, MatchMultiplePatterns) {
    Trie::Tree tree;
    tree.insert(std::make_shared<Word>("tv"));
    tree.insert(std::make_shared<Word>("to"));
    tree.insert(std::make_shared<Word>("cute"));
    tree.insert(std::make_shared<Word>("t"));
    tree.insert(std::make_shared<Word>("cat"));
    tree.insert(std::make_shared<Word>("text"));

    auto results = tree.match("*t?", 10);
    auto words = to_words(results);

    // Expected matches
    EXPECT_NE(std::find(words.begin(), words.end(), "tv"), words.end());
    EXPECT_NE(std::find(words.begin(), words.end(), "to"), words.end());
    EXPECT_NE(std::find(words.begin(), words.end(), "cute"), words.end());

    // Ensure non-matches are not present
    EXPECT_EQ(std::find(words.begin(), words.end(), "t"), words.end());
    EXPECT_EQ(std::find(words.begin(), words.end(), "cat"), words.end());
    EXPECT_EQ(std::find(words.begin(), words.end(), "text"), words.end());

    EXPECT_EQ(words.size(), 3);

    EXPECT_GE(tree.get_memory_usage(), 0);
}
