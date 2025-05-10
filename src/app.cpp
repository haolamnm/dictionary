#include "app.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "dictionary.hpp"
#include "utility.hpp"
#include "word.hpp"

App::App(bool silent) : loaded(false), running(false), silent(silent) {
    dict = std::make_unique<Dictionary>();
}

App::App(const std::string &filepath, bool silent) : App(silent) {
    load(filepath);
}

bool App::load(const std::string &filepath) {
    bool success = dict->load(filepath);
    if (success == false) {
        log(Status::Error, "cannot load file " + filepath);
        return false;
    }
    if (loaded == false) {
        log(Status::Info, "loaded file " + filepath);
    } else {
        log(Status::Info, "reloaded file " + filepath);
    }
    loaded = true;
    return true;
}

void App::run() {
    if (loaded == false) {
        log(Status::Warning, "load a file before run the app");
        return;
    }
    std::string input;
    while (true) {
        if (silent == false) {
            welcome();  // Run welcome effects
        }

        std::cout << "> ";
        if (!std::getline(std::cin, input) || terminate(input)) {
            break;
        }
        bool is_command = parse_command(input);
        if (is_command) {
            continue;
        }
        auto start = std::chrono::steady_clock::now();
        auto results = dict->search(lower(input));
        auto end = std::chrono::steady_clock::now();
        print(results, end - start);
    }
}

bool App::terminate(const std::string &input) const {
    return input == "exit()" || input == "quit()";
}

void App::typewritter(const std::string &text, int delay_ms) const {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
    std::cout << std::endl;
}

void App::welcome() {
    if (running == false) {
        typewritter("> clear()");
        clear();
        typewritter("> docs()");
        show_docs();
        running = true;
    }
}

bool App::parse_command(const std::string &command) {
    if (command == "docs()") {
        show_docs();
        return true;
    } else if (command == "clear()") {
        clear();
        return true;
    } else if (command == "config()") {
        config();
        return true;
    } else if (command == "stats()") {
        show_stats();
        return true;
    }
    return false;
}

void App::show_docs() const {
    std::cout << "\tdocs: https://github.com/haolamnm/dictionary/blob/main/README.md\n";
}

void App::show_stats() const {
    std::cout << std::left;
    int word_count = dict->get_word_count();
    std::string word_unit = (word_count == 1) ? "word" : "words";
    std::cout << std::setw(20) << "\tword-count" << ": " << word_count << " " << word_unit << '\n';

    size_t memory_usage = dict->get_memory_usage();
    std::string memory_unit = "bytes";
    double memory_display = static_cast<double>(memory_usage);
    if (memory_usage >= 1024 && memory_usage < 1024 * 1024) {
        memory_display /= 1024.0;
        memory_unit = "kilobytes";
    } else if (memory_usage >= 1024 * 1024 && memory_usage < 1024 * 1024 * 1024) {
        memory_display /= (1024.0 * 1024.0);
        memory_unit = "megabytes";
    } else if (memory_usage >= 1024 * 1024 * 1024) {
        memory_display /= (1024.0 * 1024.0 * 1024.0);
        memory_unit = "gigabytes";
    }
    std::cout << std::setw(20) << "\tmemory-usage" << ": " << std::fixed << std::setprecision(2)
              << memory_display << " " << memory_unit << '\n';

    // NOTE: May not be useful for users
    // std::cout << std::setw(20) << "\ttrie-height" << ": " << dict->get_trie_height() << '\n';
    // std::cout << std::setw(20) << "\tbktree-height" << ": " << dict->get_bktree_height() << '\n';
    std::cout << std::flush;
}

void App::clear() const {
    std::cout << "\033[2J\033[1;1H";
}

void App::config() {
    int max_distance, max_suggestions, max_matches;
    get_int(max_distance, "\tmax-distance = ", 0, 10);
    get_int(max_suggestions, "\tmax-suggestions = ", 1, 1000);
    get_int(max_matches, "\tmax-matches = ", 1, 1000);
    std::cin.ignore();

    Config config{max_distance, max_suggestions, max_matches};
    dict->set_config(config);
}
