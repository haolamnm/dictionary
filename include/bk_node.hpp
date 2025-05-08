#ifndef BK_NODE_HPP
#define BK_NODE_HPP

#include <memory>
#include <unordered_map>

#include "word.hpp"

namespace BK {

class Node {
   private:
    std::unordered_map<int, std::unique_ptr<Node>> children;
    std::shared_ptr<Word> word;

   public:
    Node(std::shared_ptr<Word> word);
    ~Node() = default;

    std::shared_ptr<Word> get_word() const;

    Node *set_child(int distance, std::shared_ptr<Word> new_word);
    Node *get_child(int distance) const;
    bool has_child(int distance) const;

    const std::unordered_map<int, std::unique_ptr<Node>> &get_children() const;

   private:
};

};  // namespace BK

#endif
