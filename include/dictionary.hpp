#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <memory>
#include <string>
#include <vector>

#include "bk_tree.hpp"
#include "trie_tree.hpp"
#include "word.hpp"

enum class Mode { Search, Suggest, Match, None };

struct Query {
    bool valid;
    bool has_underscore;
    bool has_wildcards;
};

struct Config {
    int max_distance = 2;
    int max_suggestions = 5;
    int max_matches = 5;
};

class Dictionary {
   private:
    std::unique_ptr<Trie::Tree> trie;
    std::unique_ptr<BK::Tree> bktree;
    std::unique_ptr<Config> config;
    size_t memory_usage = 0;
    bool stable = true;
    int word_count = 0;
    int trie_height = 0;
    int bktree_height = 0;

   public:
    Dictionary();
    Dictionary(const std::string &filepath);
    ~Dictionary() = default;

    void insert(std::shared_ptr<Word> word);
    bool load(const std::string &filepath);

    std::vector<std::shared_ptr<Word>> search(const std::string &query) const;

    void set_stable(bool stable);
    void set_config(const Config &cfg);

    int get_stable() const;
    int get_word_count() const;
    size_t get_memory_usage();
    int get_trie_height();
    int get_bktree_height();

   private:
    Mode recognize(const std::string &query) const;
    Query validate(const std::string &query) const;
};

#endif
