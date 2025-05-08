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

class Dictionary {
   private:
    std::unique_ptr<Trie::Tree> trie;
    std::unique_ptr<BK::Tree> bktree;

   public:
    Dictionary();
    Dictionary(const std::string &filepath);
    ~Dictionary() = default;

    void insert(std::shared_ptr<Word> word);
    bool load(const std::string &filepath);

    std::vector<std::shared_ptr<Word>> search(const std::string &query) const;

   private:
    Mode recognize(const std::string &query) const;
    Query validate(const std::string &query) const;
};

#endif
