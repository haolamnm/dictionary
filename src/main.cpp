#include <string>

#include "app.hpp"
#include "utility.hpp"

int main(int argc, char *argv[]) {
    std::string filepath = "../data/dictionary.csv";
    bool silent = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg.rfind("--file=", 0) == 0) {
            filepath = arg.substr(7);  // after "--file="
        } else if (arg == "--silent") {
            silent = true;
        } else {
            log(Status::Error, "unknown argument " + arg);
            log(Status::Info, "usage: dictionary.exe [--file=path] [--silent]");
            return -1;
        }
    }

    App app(filepath, silent);
    app.run();

    return 0;
}
