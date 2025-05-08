#ifndef TRIE_TREE_HPP
#define TRIE_TREE_HPP

#include <memory>
#include <string>
#include <vector>

#include "trie_node.hpp"
#include "word.hpp"

namespace Trie {
class Tree {
   private:
    std::unique_ptr<Node> root;

   public:
    Tree();
    ~Tree() = default;

    void insert(std::shared_ptr<Word> word);

    std::shared_ptr<Word> search(const std::string &word) const;
    std::vector<std::shared_ptr<Word>> suggest(const std::string &prefix) const;
    std::vector<std::shared_ptr<Word>> match(const std::string &pattern) const;

   private:
    void suggest_core(const Node *node, std::string prefix,
                      std::vector<std::shared_ptr<Word>> &suggestions) const;
    void match_core(const Node *node, size_t pattern_index, const std::string &current,
                    const std::string &pattern, std::vector<std::shared_ptr<Word>> &matches) const;
};
};  // namespace Trie

#endif
