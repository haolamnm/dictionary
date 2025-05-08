#ifndef WORD_HPP
#define WORD_HPP

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
    std::string definition;
    POS pos;

   public:
    Word(const std::string &text, const std::string &definition, POS pos);
    ~Word() = default;

    const std::string &get_text() const;
    const std::string &get_definition() const;
    POS get_pos() const;

    void set_text(const std::string &text);
    void set_definition(const std::string &definition);
    void set_pos(POS pos);

   private:
};

// Parsing
POS parse_string(const std::string &str);
std::string parse_pos(POS pos);

// Printing
void print(std::shared_ptr<Word> word);
void print(std::vector<std::shared_ptr<Word>> words);

#endif
