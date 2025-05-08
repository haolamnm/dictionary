#include "word.hpp"

#include <format>
#include <iostream>
#include <memory>
#include <string>

#include "utility.hpp"

Word::Word(const std::string &text, const std::string &definition, POS pos)
    : text(text), definition(definition), pos(pos) {}

const std::string &Word::get_text() const {
    return text;
}

const std::string &Word::get_definition() const {
    return definition;
}

POS Word::get_pos() const {
    return pos;
}

void Word::set_text(const std::string &text) {
    this->text = text;
}

void Word::set_definition(const std::string &definition) {
    this->definition = definition;
}

void Word::set_pos(POS pos) {
    this->pos = pos;
}

std::string parse_pos(POS pos) {
    switch (pos) {
        case POS::Noun:
            return "noun";
        case POS::Verb:
            return "verb";
        case POS::Adjective:
            return "adjective";
        case POS::Adverb:
            return "adverb";
        case POS::Pronoun:
            return "pronoun";
        case POS::Preposition:
            return "preposition";
        case POS::Conjunction:
            return "conjunction";
        case POS::Interjection:
            return "interjection";
        case POS::Undefined:
            return "undefined";
        default:
            return "undefined";
    }
}

POS parse_string(const std::string &str) {
    std::string pos = lower(str);

    if (pos == "noun") return POS::Noun;
    if (pos == "verb") return POS::Verb;
    if (pos == "adjective") return POS::Adjective;
    if (pos == "adverb") return POS::Adverb;
    if (pos == "pronoun") return POS::Pronoun;
    if (pos == "preposition") return POS::Preposition;
    if (pos == "conjunction") return POS::Conjunction;
    if (pos == "interjection") return POS::Interjection;
    return POS::Undefined;
}

void print(std::shared_ptr<Word> word) {
    if (word.get() == nullptr) {
        log(Status::Warning, "Word cannot be nullptr");
        return;
    }
    std::string text = word->get_text();
    std::string definition = word->get_definition();
    std::string pos = parse_pos(word->get_pos());
    std::cout << "\t" << text << " (" << pos << ")" << ": " << definition << ".\n";
}

void print(std::vector<std::shared_ptr<Word>> words) {
    std::string message = std::format("found {} result(s)", words.size());
    log(Status::Info, message);

    for (std::shared_ptr<Word> word : words) {
        print(word);
    }
}
