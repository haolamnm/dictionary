#ifndef BK_TREE_HPP
#define BK_TREE_HPP

#include <memory>
#include <string>
#include <vector>

#include "bk_node.hpp"

namespace BK {

class Tree {
   private:
    std::unique_ptr<Node> root;

   public:
    Tree() : root(nullptr) {}
    ~Tree() = default;

    void insert(std::shared_ptr<Word> word);

    std::vector<std::shared_ptr<Word>> search(const std::string &query, int max_distance,
                                              int max_searches) const;

   private:
    void search_core(const Node *node, const std::string &query, int max_distance,
                     std::vector<std::shared_ptr<Word>> &results, int max_searches) const;

    int calculate_distance(const std::string &s1, const std::string &s2) const;
};

};  // namespace BK

#endif
