#include "dictionary.hpp"

#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "bk_tree.hpp"
#include "trie_tree.hpp"
#include "utility.hpp"

Dictionary::Dictionary() {
    trie = std::make_unique<Trie::Tree>();
    bktree = std::make_unique<BK::Tree>();
    config = std::make_unique<Config>();
    word_count = 0;
}

Dictionary::Dictionary(const std::string &filepath) : Dictionary() {
    load(filepath);
}

void Dictionary::insert(std::shared_ptr<Word> word) {
    trie->insert(word);
    bktree->insert(word);
}

bool Dictionary::load(const std::string &filepath) {
    std::ifstream fin(filepath.c_str(), std::ios::binary);
    if (fin.is_open() == false) {
        return false;
    }
    // Get total file size
    fin.seekg(0, std::ios::end);
    std::streampos total_size = fin.tellg();
    fin.seekg(0);

    // Only show progress for file bigger than 4 megabytes
    bool show_progress = (total_size > 4 * 1024 * 1024);

    std::string line;
    std::getline(fin, line);  // Skip header

    std::shared_ptr<Word> current_word = nullptr;
    std::string prev_text;

    int line_processed = 0;

    while (std::getline(fin, line)) {
        std::string text, pos_string, definition;

        std::istringstream ss(line);
        std::getline(ss, text, ',');
        std::getline(ss, pos_string, ',');
        POS pos = parse_string(pos_string);

        if (ss.peek() == '"') {
            ss.get();  // Remove opening quote
            std::getline(ss, definition, '"');
        } else {
            std::getline(ss, definition);  // Fallback
        }

        // If current text differ from previous text
        if (current_word == nullptr || text != prev_text) {
            // Insert previous word
            if (current_word) {
                insert(current_word);
                word_count += 1;
            }
            // Start new word
            current_word = std::make_shared<Word>(text);
            prev_text = text;
        }
        current_word->add_pos(pos);
        current_word->add_definition(definition);

        if (show_progress && ++line_processed % 500 == 0) {
            std::streampos current_pos = fin.tellg();
            print_progress_bar(static_cast<int>(current_pos), static_cast<int>(total_size));
        }
    }
    // Last word
    if (current_word) {
        insert(current_word);
        word_count += 1;
    }
    // Final 100% bar
    if (show_progress) {
        print_progress_bar(1, 1);
        std::cout << std::endl;
    }

    // Set as stable after load a file
    trie->set_stable(true);
    bktree->set_stable(true);

    // Load member parameters
    memory_usage = trie->get_memory_usage() + bktree->get_memory_usage();
    trie_height = trie->get_height();
    bktree_height = bktree->get_height();

    fin.close();
    return true;
}

std::vector<std::shared_ptr<Word>> Dictionary::search(const std::string &query) const {
    Mode mode = recognize(query);

    switch (mode) {
        case Mode::Search: {
            std::shared_ptr<Word> word = trie->search(query);
            if (word == nullptr) {
                return bktree->search(query, config->max_distance, config->max_suggestions);
            }
            return {word};
        }
        case Mode::Suggest: {
            std::string prefix = query;
            prefix.pop_back();
            return trie->suggest(prefix, config->max_suggestions);
        }
        case Mode::Match:
            return trie->match(query, config->max_matches);
        case Mode::None:
            // NOTE: Disable log for performance
            // log(Status::Warning, "invalid query");
            return {};
        default:
            return {};
    }
}

void Dictionary::set_stable(bool stable) {
    this->stable = stable;
}

void Dictionary::set_config(const Config &cfg) {
    config->max_distance = cfg.max_distance;
    config->max_suggestions = cfg.max_suggestions;
    config->max_matches = cfg.max_matches;
}

int Dictionary::get_stable() const {
    return stable;
}

int Dictionary::get_word_count() const {
    return word_count;
}

size_t Dictionary::get_memory_usage() {
    if (stable == false || memory_usage == 0) {
        memory_usage = trie->get_memory_usage() + bktree->get_memory_usage();
        stable = true;
    }
    return memory_usage;
}

int Dictionary::get_trie_height() {
    if (stable == false || trie_height == 0) {
        trie_height = trie->get_height();
        stable = true;
    }
    return trie_height;
}

int Dictionary::get_bktree_height() {
    if (stable == false || bktree_height == 0) {
        bktree_height = bktree->get_height();
        stable = true;
    }
    return bktree_height;
}

Mode Dictionary::recognize(const std::string &query) const {
    Query q = validate(query);

    if (q.valid == false) return Mode::None;                     // Invalid query
    if (q.has_underscore && q.has_wildcards) return Mode::None;  // Ambiguous query
    if (q.has_underscore) return Mode::Suggest;
    if (q.has_wildcards) return Mode::Match;
    return Mode::Search;
}

Query Dictionary::validate(const std::string &query) const {
    Query q{true, false, false};

    if (query.empty() || query.size() > 100) {
        q.valid = false;
        return q;  // Empty query
    }
    bool seen_wildcards = false;
    char prev_c = '\0';

    for (size_t i = 0; i < query.size(); ++i) {
        char c = query[i];

        if (std::isalpha(c) == false && c != '*' && c != '+' && c != '?' && c != '_') {
            q.valid = false;
            return q;  // Unsupported characters
        }
        if (c == '*' || c == '+' || c == '?') {
            if (seen_wildcards && !(c == '?' && prev_c == '?')) {
                q.valid = false;
                return q;  // Consecutive wildcards except consecutive '?'
            }
            q.has_wildcards = true;
            seen_wildcards = true;
        } else {
            seen_wildcards = false;
        }
        if (c == '_') {
            if (q.has_underscore || i != query.size() - 1) {
                q.valid = false;
                return q;  // Underscore is not at the end
            }
            q.has_underscore = true;
        }
        prev_c = c;
    }
    return q;
}
