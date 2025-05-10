#ifndef TRIE_NODE_HPP
#define TRIE_NODE_HPP

#include <memory>
#include <unordered_map>

#include "word.hpp"

namespace Trie {

class Node {
   private:
    std::unordered_map<char, std::unique_ptr<Node>> children;
    std::shared_ptr<Word> word;

   public:
    Node() = default;
    ~Node() = default;

    bool is_word() const;
    std::shared_ptr<Word> get_word() const;
    void set_word(std::shared_ptr<Word> word);

    Node *get_child(char c) const;
    Node *set_child(char c);

    const std::unordered_map<char, std::unique_ptr<Node>> &get_children() const;

   private:
};

};  // namespace Trie

#endif
