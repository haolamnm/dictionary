#include "dictionary.hpp"

#include <cctype>
#include <fstream>
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
    std::ifstream fin(filepath.c_str());
    if (fin.is_open() == false) {
        return false;
    }
    std::string line;
    std::getline(fin, line);  // Skip header

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

        std::shared_ptr<Word> word = std::make_shared<Word>(text, definition, pos);
        insert(word);
        word_count += 1;
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
            log(Status::Warning, "invalid query");
            return {};
        default:
            log(Status::Critical, "unexpected fallback");
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

    for (size_t i = 0; i < query.size(); ++i) {
        char c = query[i];

        if (std::isalpha(c) == false && c != '*' && c != '+' && c != '?' && c != '_') {
            q.valid = false;
            return q;  // Unsupported characters
        }
        if (c == '*' || c == '+' || c == '?') {
            if (seen_wildcards) {
                q.valid = false;
                return q;  // Consecutive wildcards
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
    }
    return q;
}
