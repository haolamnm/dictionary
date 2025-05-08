#ifndef APP_HPP
#define APP_HPP

#include <memory>
#include <string>

#include "dictionary.hpp"

class App {
   private:
    std::unique_ptr<Dictionary> dict;
    bool loaded = false;
    bool running = false;

   public:
    App();
    App(const std::string &filepath);
    ~App() = default;

    bool load(const std::string &filepath);
    void run();

   private:
    bool terminate(const std::string &input) const;
    void help() const;
    void welcome(const std::string &text, int delay_ms = 100) const;
    ;
};

#endif
