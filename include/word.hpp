#ifndef WORD_HPP
#define WORD_HPP

#include <chrono>
#include <memory>
#include <string>
#include <vector>

enum class POS {
    Noun,
    Verb,
    Adjective,
    Adverb,
    Pronoun,
    Preposition,
    Conjunction,
    Interjection,
    Undefined
};

class Word {
   private:
    std::string text;
    std::vector<std::string> definition;
    std::vector<POS> pos;

   public:
    Word(const std::string &text);
    ~Word() = default;

    const std::string &get_text() const;
    const std::vector<std::string> &get_definition() const;
    const std::vector<POS> &get_pos() const;

    void set_text(const std::string &text);
    void add_definition(const std::string &definition);
    void add_pos(POS pos);

   private:
};

// Parse
POS parse_string(const std::string &str);
std::string parse_pos(POS pos);

// Print
void print(std::shared_ptr<Word> word, bool show_definition = false);
void print(std::vector<std::shared_ptr<Word>> words, std::chrono::steady_clock::duration duration);

#endif
