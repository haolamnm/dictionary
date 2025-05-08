#include "app.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "dictionary.hpp"
#include "utility.hpp"
#include "word.hpp"

App::App() : loaded(false), running(false) {
    dict = std::make_unique<Dictionary>();
}

App::App(const std::string &filepath) : App() {
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
        welcome();  // Run welcome effects

        std::cout << "> ";
        if (!std::getline(std::cin, input) || terminate(input)) {
            break;
        }
        bool is_command = parse_command(input);
        if (is_command) {
            continue;
        }
        auto results = dict->search(lower(input));
        print(results);
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

void App::show_stats() const {}

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
