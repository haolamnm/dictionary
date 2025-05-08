#include "bk_node.hpp"

#include <memory>
#include <unordered_map>

#include "word.hpp"

namespace BK {

Node::Node(std::shared_ptr<Word> word) : word(std::move(word)) {}

std::shared_ptr<Word> Node::get_word() const {
    return word;
}

Node *Node::set_child(int distance, std::shared_ptr<Word> new_word) {
    if (children.find(distance) == children.end()) {
        children[distance] = std::make_unique<Node>(new_word);
    }
    return children[distance].get();
}

Node *Node::get_child(int distance) const {
    auto it = children.find(distance);
    return (it != children.end()) ? it->second.get() : nullptr;
}

bool Node::has_child(int distance) const {
    return children.find(distance) != children.end();
}

const std::unordered_map<int, std::unique_ptr<Node>> &Node::get_children() const {
    return children;
}

};  // namespace BK
