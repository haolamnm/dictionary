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
    size_t memory_usage = 0;
    bool stable = true;
    int height = 0;

   public:
    Tree();
    ~Tree() = default;

    void insert(std::shared_ptr<Word> word);

    std::shared_ptr<Word> search(const std::string &word) const;
    std::vector<std::shared_ptr<Word>> suggest(const std::string &prefix,
                                               int max_suggestions) const;
    std::vector<std::shared_ptr<Word>> match(const std::string &pattern, int max_matches) const;

    void set_stable(bool stable);
    size_t get_memory_usage();
    int get_height();

   private:
    void suggest_core(const Node *node, std::string prefix,
                      std::vector<std::shared_ptr<Word>> &suggestions, int max_suggestions) const;
    void match_core(const Node *node, size_t pattern_index, const std::string &current,
                    const std::string &pattern, std::vector<std::shared_ptr<Word>> &matches,
                    int max_matches) const;
    size_t calculate_memory_usage(const Node *node) const;
    int calculate_height(const Node *node) const;
};

};  // namespace Trie

#endif
