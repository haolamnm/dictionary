#include <gtest/gtest.h>

#include "word.hpp"

TEST(WordTest, Initialization) {
    Word word("cat");
    EXPECT_EQ(word.get_text(), "cat");
    EXPECT_TRUE(word.get_definition().empty());
    EXPECT_TRUE(word.get_pos().empty());
}

TEST(WordTest, SetText) {
    Word word("cat");
    word.set_text("dog");
    EXPECT_EQ(word.get_text(), "dog");
}

TEST(WordTest, AddDefinition) {
    Word word("cat");
    word.add_definition("definition 1");
    word.add_definition("definition 2");

    const auto& definitions = word.get_definition();
    ASSERT_EQ(definitions.size(), 2);
    EXPECT_EQ(definitions[0], "definition 1");
    EXPECT_EQ(definitions[1], "definition 2");
}

TEST(WordTest, AddPOS) {
    Word word("cat");
    word.add_pos(POS::Adjective);
    word.add_pos(POS::Noun);

    const auto& pos_list = word.get_pos();
    ASSERT_EQ(pos_list.size(), 2);
    EXPECT_EQ(pos_list[0], POS::Adjective);
    EXPECT_EQ(pos_list[1], POS::Noun);
}
