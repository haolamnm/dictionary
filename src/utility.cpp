#include "utility.hpp"

#include <algorithm>
#include <format>
#include <iomanip>
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

void print_progress_bar(int progress, int total, int bar_width) {
    float percent = 100.0f * progress / total;
    int filled = static_cast<int>(percent * bar_width / 100.0f);
    std::string bar(filled, '#');
    bar += std::string(bar_width - filled, '-');
    std::cout << "\r[" << bar << "] " << std::fixed << std::setprecision(1) << percent << "%"
              << std::flush;
}
