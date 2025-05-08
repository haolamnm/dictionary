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
        log(Status::Error, "Cannot load file " + filepath);
        return false;
    }
    if (loaded == false) {
        log(Status::Info, "Loaded file " + filepath);
    } else {
        log(Status::Info, "Reloaded file " + filepath);
    }
    loaded = true;
    return true;
}

void App::run() {
    if (loaded == false) {
        log(Status::Info, "Load a file before run the app");
        return;
    }
    std::string input;
    while (true) {
        if (running == false) {
            welcome("> help()");
            help();
            running = true;
        }
        std::cout << "> ";
        if (!std::getline(std::cin, input) || terminate(input)) break;
        if (input == "help()") {
            help();
            return;
        }

        auto results = dict->search(lower(input));
        print(results);
    }
}

bool App::terminate(const std::string &input) const {
    return input == "exit()" || input == "quit()";
}

void App::help() const {
    std::cout << "quit() or exit() to exit the app.\n";
}

void App::welcome(const std::string &text, int delay_ms) const {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
    std::cout << std::endl;
}
