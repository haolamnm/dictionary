#include "trie_tree.hpp"

#include <memory>
#include <string>
#include <vector>

#include "word.hpp"

namespace Trie {

Tree::Tree() {
    root = std::make_unique<Node>();
    memory_usage = 0;
    stable = true;
    height = 0;
}

void Tree::insert(std::shared_ptr<Word> word) {
    Node *node = root.get();
    for (char c : word->get_text()) {
        Node *child = node->set_child(c);
        node = child;
    }
    node->set_word(word);
    stable = false;
}

std::shared_ptr<Word> Tree::search(const std::string &word) const {
    Node *node = root.get();
    for (char c : word) {
        Node *child = node->get_child(c);
        if (child == nullptr) return nullptr;
        node = child;
    }
    return node->get_word();
}

std::vector<std::shared_ptr<Word>> Tree::suggest(const std::string &prefix,
                                                 int max_suggestions) const {
    std::vector<std::shared_ptr<Word>> suggestions;
    Node *node = root.get();

    for (char c : prefix) {
        Node *child = node->get_child(c);
        if (child == nullptr) {
            return suggestions;
        }
        node = child;
    }
    suggest_core(node, prefix, suggestions, max_suggestions);
    return suggestions;
}

std::vector<std::shared_ptr<Word>> Tree::match(const std::string &pattern, int max_matches) const {
    std::vector<std::shared_ptr<Word>> matches;
    Node *node = root.get();
    match_core(node, 0, "", pattern, matches, max_matches);
    return matches;
}

void Tree::set_stable(bool stable) {
    this->stable = stable;
}

size_t Tree::get_memory_usage() {
    if (stable == false || memory_usage == 0) {
        memory_usage = calculate_memory_usage(root.get());
        stable = true;
    }
    return memory_usage;
}

int Tree::get_height() {
    if (stable == false || height == 0) {
        height = calculate_height(root.get());
        stable = true;
    }
    return height;
}

void Tree::suggest_core(const Node *node, std::string prefix,
                        std::vector<std::shared_ptr<Word>> &suggestions,
                        int max_suggestions) const {
    // Early exit
    if (node == nullptr || suggestions.size() >= max_suggestions) return;

    if (node->is_word()) {
        suggestions.push_back(node->get_word());

        // Hot exit after adding a word
        if (suggestions.size() >= max_suggestions) return;
    }
    for (const auto &[c, child] : node->get_children()) {
        suggest_core(child.get(), prefix + c, suggestions, max_suggestions);

        // After visiting a child
        if (suggestions.size() >= max_suggestions) return;
    }
}

void Tree::match_core(const Node *node, size_t pattern_index, const std::string &current,
                      const std::string &pattern, std::vector<std::shared_ptr<Word>> &matches,
                      int max_matches) const {
    // End of finding path
    if (node == nullptr || matches.size() >= max_matches) return;

    // End of pattern, collect word if it is valid
    if (pattern_index == pattern.size()) {
        if (node->is_word()) {
            matches.push_back(node->get_word());
        }
        return;
    }

    char pattern_char = pattern[pattern_index];

    // '+': Match zero or more characters
    if (pattern_char == '*') {
        // Case 1: Match zero characters and advance pattern index
        match_core(node, pattern_index + 1, current, pattern, matches, max_matches);

        // Case 2. Match one or more characters (recurse into each child and stay on '*')
        for (const auto &[c, child] : node->get_children()) {
            match_core(child.get(), pattern_index, current + c, pattern, matches, max_matches);
        }
    }
    // '+': Must match at least one or more characters
    else if (pattern_char == '+') {
        // NOTE: Must consume at least one character before staying on '+'
        for (const auto &[c, child] : node->get_children()) {
            // First, must advance into children (consume one char), stay on '+'
            match_core(child.get(), pattern_index, current + c, pattern, matches, max_matches);

            // After consuming one or more, now move to next pattern index
            match_core(child.get(), pattern_index + 1, current + c, pattern, matches, max_matches);
        }
    }
    // '?': Match exactly one character
    else if (pattern_char == '?') {
        for (const auto &[c, child] : node->get_children()) {
            match_core(child.get(), pattern_index + 1, current + c, pattern, matches, max_matches);
        }
    }
    // Normal match
    else {
        auto it = node->get_children().find(pattern_char);
        if (it != node->get_children().end()) {
            const Node *child = it->second.get();
            match_core(child, pattern_index + 1, current + pattern_char, pattern, matches,
                       max_matches);
        }
    }
}

size_t Tree::calculate_memory_usage(const Node *node) const {
    if (node == nullptr) return 0;

    size_t size = sizeof(Node);

    for (const auto &[_, child] : node->get_children()) {
        size += calculate_memory_usage(child.get());
    }
    return size;
}

int Tree::calculate_height(const Node *node) const {
    if (node == nullptr) return 0;

    int max_depth = 0;
    for (const auto &[_, child] : node->get_children()) {
        max_depth = std::max(max_depth, calculate_height(child.get()));
    }
    return 1 + max_depth;
}

};  // namespace Trie
