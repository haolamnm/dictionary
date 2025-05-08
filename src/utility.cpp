#include "utility.hpp"

#include <algorithm>
#include <format>
#include <iostream>
#include <iterator>
#include <limits>

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
            label = "debug:";
            break;
        case Status::Info:
            color = BLUE;
            label = "info:";
            break;
        case Status::Success:
            color = GREEN;
            label = "success:";
            break;
        case Status::Warning:
            color = YELLOW;
            label = "warning:";
            break;
        case Status::Error:
            color = RED;
            label = "error:";
            break;
        case Status::Critical:
            color = BOLD RED;
            label = "critical:";
            break;
    }

    std::cout << color << label << " " << msg << RESET << std::endl;
}

bool get_int(int &input, const std::string &prompt, int min_value, int max_value) {
    while (true) {
        std::cout << prompt;
        std::cin >> input;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            log(Status::Warning, "invalid input");
        } else if (input < min_value || input > max_value) {
            std::string message =
                std::format("please enter a number between {} and {}", min_value, max_value);
            log(Status::Info, message);
        } else {
            return true;  // Valid input
        }
    }
}
