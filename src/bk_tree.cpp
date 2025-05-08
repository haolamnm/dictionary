#include "bk_tree.hpp"

#include <algorithm>
#include <memory>
#include <vector>

#include "word.hpp"

namespace BK {

void Tree::insert(std::shared_ptr<Word> word) {
    if (root.get() == nullptr) {
        root = std::make_unique<Node>(word);
        return;
    }
    Node *node = root.get();
    while (true) {
        int distance = calculate_distance(word->get_text(), node->get_word()->get_text());

        Node *child = node->get_child(distance);
        if (child == nullptr) {
            node->set_child(distance, word);
            return;
        }
        node = child;
    }
}

std::vector<std::shared_ptr<Word>> Tree::search(const std::string &query, int max_distance) const {
    if (root.get() == nullptr) {
        return {};
    }
    std::vector<std::shared_ptr<Word>> results;
    if (root != nullptr) {
        search_core(root.get(), query, max_distance, results);
    }
    return results;
}

void Tree::search_core(const Node *node, const std::string &query, int max_distance,
                       std::vector<std::shared_ptr<Word>> &results) const {
    int distance = calculate_distance(query, node->get_word()->get_text());

    if (distance <= max_distance) {
        results.push_back(node->get_word());
    }

    const auto &children = node->get_children();
    for (int d = distance - max_distance; d <= distance + max_distance; ++d) {
        auto it = children.find(d);
        if (it != children.end()) {
            Node *child = it->second.get();
            search_core(child, query, max_distance, results);
        }
    }
}

int Tree::calculate_distance(const std::string &s1, const std::string &s2) const {
    int sz1 = s1.size();
    int sz2 = s2.size();

    // Ensure s2 is longer than s1
    if (sz1 > sz2) {
        return calculate_distance(s2, s1);
    }
    std::vector<int> prev(sz2 + 1), curr(sz2 + 1);

    for (int j = 0; j <= sz2; ++j) {
        prev[j] = j;
    }
    // Fill the DP table row by row
    for (int i = 1; i <= sz1; ++i) {
        curr[0] = i;  // Initialize the first column of the current row
        for (int j = 1; j <= sz2; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                curr[j] = prev[j - 1];  // No cost if characters match
            } else {
                curr[j] = 1 + std::min({prev[j - 1], prev[j], curr[j - 1]});
            }
        }
        // Swap prev and curr for the next iteration
        swap(prev, curr);
    }
    // The result is in prev[n] after the final swap
    return prev[sz2];
}

};  // namespace BK
