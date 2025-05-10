#include <gtest/gtest.h>

#include "trie_node.hpp"
#include "word.hpp"

TEST(TrieNodeTest, SetAndGetChild) {
    Trie::Node node;

    Trie::Node* child = node.set_child('c');
    EXPECT_NE(child, nullptr);
    EXPECT_EQ(node.get_child('c'), child);

    EXPECT_TRUE(node.get_children().find('c') != node.get_children().end());
}

TEST(TrieNodeTest, SetAndGetWord) {
    Trie::Node node;
    auto word = std::make_shared<Word>("cat");
    node.set_word(word);

    EXPECT_TRUE(node.is_word());
    EXPECT_EQ(node.get_word()->get_text(), "cat");
}
