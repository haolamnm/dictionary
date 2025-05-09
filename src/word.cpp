#include "word.hpp"

#include <chrono>
#include <format>
#include <iostream>
#include <memory>
#include <string>

#include "utility.hpp"

Word::Word(const std::string &text) : text(text) {}

const std::string &Word::get_text() const {
    return text;
}

const std::vector<std::string> &Word::get_definition() const {
    return definition;
}

const std::vector<POS> &Word::get_pos() const {
    return pos;
}

void Word::set_text(const std::string &text) {
    this->text = text;
}

void Word::add_definition(const std::string &definition) {
    this->definition.push_back(definition);
}

void Word::add_pos(POS pos) {
    this->pos.push_back(pos);
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

void print(std::shared_ptr<Word> word, bool show_definition) {
    if (word.get() == nullptr) {
        log(Status::Warning, "Word cannot be nullptr");
        return;
    }
    std::string text = word->get_text();
    const std::vector<std::string> &definition = word->get_definition();
    const std::vector<POS> &pos = word->get_pos();

    if (show_definition && definition.empty() == false) {
        // Header
        std::cout << "\t== " << BOLD << text << RESET << " ==" << '\n';

        // Each definition in numberring
        for (int i = 0; i < definition.size(); ++i) {
            std::string pos_str = "(" + parse_pos(pos[i]) + ")";
            // Left-align POS with fixed width
            std::cout << std::format("\t{}. {}({}){} : {}\n", i + 1, BOLD, parse_pos(pos[i]), RESET,
                                     definition[i]);
        }
    } else {
        std::cout << "\t * " << text << '\n';
    }
}

void print(std::vector<std::shared_ptr<Word>> words, std::chrono::steady_clock::duration duration) {
    // Dynamic time display
    auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    std::string unit = "microseconds";
    if (1'000 <= elapsed_time && elapsed_time < 1'000'000) {
        elapsed_time /= 1'000.0;
        unit = "milliseconds";
    } else if (elapsed_time >= 1'000'000) {
        elapsed_time /= 1'000'000.0;
        unit = "seconds";
    }
    std::string result = (words.size() == 1) ? "result" : "results";
    std::string message =
        std::format("found {} {} in {} {}", words.size(), result, elapsed_time, unit);
    log(Status::Info, message);

    bool show_definition = words.size() == 1;
    for (std::shared_ptr<Word> word : words) {
        print(word, show_definition);
    }
}
