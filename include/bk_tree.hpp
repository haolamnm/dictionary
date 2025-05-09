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
    size_t memory_usage = 0;
    bool stable = true;
    int height = 0;

   public:
    Tree() : root(nullptr) {}
    ~Tree() = default;

    void insert(std::shared_ptr<Word> word);

    std::vector<std::shared_ptr<Word>> search(const std::string &query, int max_distance,
                                              int max_searches) const;

    void set_stable(bool stable);
    size_t get_memory_usage();
    int get_height();

   private:
    void search_core(const Node *node, const std::string &query, int max_distance,
                     std::vector<std::pair<std::shared_ptr<Word>, int>> &results,
                     int max_searches) const;

    int calculate_distance(const std::string &s1, const std::string &s2) const;
    size_t calculate_memory_usage(const Node *node) const;
    int calculate_height(const Node *node) const;
};

};  // namespace BK

#endif
