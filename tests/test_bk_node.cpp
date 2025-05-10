#include <gtest/gtest.h>

#include "bk_node.hpp"

TEST(BKNodeTest, Initialization) {
    auto word = std::make_shared<Word>("cat");
    BK::Node node(word);

    EXPECT_EQ(node.get_word()->get_text(), "cat");
    EXPECT_TRUE(node.get_children().empty());
}

TEST(BKNodeTest, SetAndGetChild) {
    auto parent_word = std::make_shared<Word>("cat");
    BK::Node node(parent_word);

    auto child_word = std::make_shared<Word>("mouse");
    BK::Node* child = node.set_child(2, child_word);

    EXPECT_TRUE(node.has_child(2));
    EXPECT_EQ(node.get_child(2)->get_word()->get_text(), "mouse");
    EXPECT_EQ(child, node.get_child(2));
}
