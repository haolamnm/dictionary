#ifndef UTILITY_HPP
#define UTILITY_HPP

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"

#include <string>

// String manipulation
std::string lower(const std::string &str);

// Log message with status
enum class Status { Debug, Info, Success, Warning, Error, Critical };
void log(Status status, const std::string &msg);

// Safely get integer input
bool get_int(int &input, const std::string &prompt, int min_value = 0, int max_value = 100);

// Progress bar
void print_progress_bar(int progress, int total, int bar_width = 50);

#endif
