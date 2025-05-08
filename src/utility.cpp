#include "utility.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>

std::string lower(const std::string &str) {
    std::string result;
    std::transform(str.begin(), str.end(), std::back_inserter(result), ::tolower);
    return result;
}

void log(Status status, const std::string &msg) {
    std::string color;
    std::string label;

    switch (status) {
        case Status::Debug:
            color = CYAN;
            label = "Debug:";
            break;
        case Status::Info:
            color = BLUE;
            label = "Info:";
            break;
        case Status::Success:
            color = GREEN;
            label = "Success:";
            break;
        case Status::Warning:
            color = YELLOW;
            label = "Warning:";
            break;
        case Status::Error:
            color = RED;
            label = "Error:";
            break;
        case Status::Critical:
            color = BOLD RED;
            label = "Critical:";
            break;
    }

    std::cout << color << label << " " << msg << RESET << std::endl;
}
