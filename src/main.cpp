#include <string>

#include "app.hpp"
#include "utility.hpp"

int main(int argc, char *argv[]) {
    if (argc > 2) {
        log(Status::Info, "dictionary.exe [filepath]");
        return -1;
    }
    std::string default_filepath = "../data/words.csv";
    std::string filepath = argv[1] ? argv[1] : default_filepath;

    App app(filepath);
    app.run();

    return 0;
}
