#include "trie_node.hpp"

#include <memory>
#include <unordered_map>

namespace Trie {

bool Node::is_word() const {
    return word.get() != nullptr;
}

std::shared_ptr<Word> Node::get_word() const {
    return word;
}

void Node::set_word(std::shared_ptr<Word> word) {
    this->word = std::move(word);
}

Node *Node::get_child(char c) const {
    auto it = children.find(c);
    return (it != children.end()) ? it->second.get() : nullptr;
}

Node *Node::set_child(char c) {
    if (children.count(c) == 0) {
        children[c] = std::make_unique<Node>();
    }
    return children[c].get();
}

const std::unordered_map<char, std::unique_ptr<Node>> &Node::get_children() const {
    return children;
}

};  // namespace Trie
