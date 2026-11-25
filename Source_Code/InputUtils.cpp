// InputUtils.cpp
// CS212 Project 94 - Tailoring Order Workshop
#include "InputUtils.h"

#include <cmath>
#include <iostream>
#include <sstream>

static std::string trim(const std::string& s) {
    std::size_t b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos) return "";
    std::size_t e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

static std::string getLineOrThrow(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) throw InputEnded();
    // Windows editors/shells may prefix redirected input with a UTF-8 byte-order mark.
    if (line.size() >= 3 && line.compare(0, 3, "\xEF\xBB\xBF") == 0) line.erase(0, 3);
    return trim(line);
}

int readInt(const std::string& prompt, int minVal, int maxVal) {
    while (true) {
        std::string line = getLineOrThrow(prompt);
        std::istringstream iss(line);
        long long value;
        char extra;
        if (line.empty() || !(iss >> value) || (iss >> extra)) {
            std::cout << "  Error: please enter a whole number.\n";
            continue;
        }
        if (value < minVal || value > maxVal) {
            std::cout << "  Error: value must be between " << minVal << " and " << maxVal << ".\n";
            continue;
        }
        return static_cast<int>(value);
    }
}

double readDouble(const std::string& prompt, double minVal, double maxVal) {
    while (true) {
        std::string line = getLineOrThrow(prompt);
        std::istringstream iss(line);
        double value;
        char extra;
        if (line.empty() || !(iss >> value) || (iss >> extra) || !std::isfinite(value)) {
            std::cout << "  Error: please enter a number (e.g. 42 or 42.5).\n";
            continue;
        }
        if (value < minVal || value > maxVal) {
            std::cout << "  Error: value must be between " << minVal << " and " << maxVal << ".\n";
            continue;
        }
        return value;
    }
}

std::string readNonEmptyLine(const std::string& prompt, std::size_t maxLen) {
    while (true) {
        std::string line = getLineOrThrow(prompt);
        if (line.empty()) {
            std::cout << "  Error: this field cannot be empty.\n";
            continue;
        }
        if (line.size() > maxLen) {
            std::cout << "  Error: maximum length is " << maxLen << " characters.\n";
            continue;
        }
        return line;
    }
}

bool readOptionalLine(const std::string& prompt, std::string& out, std::size_t maxLen) {
    while (true) {
        std::string line = getLineOrThrow(prompt);
        if (line.empty()) return false;
        if (line.size() > maxLen) {
            std::cout << "  Error: maximum length is " << maxLen << " characters.\n";
            continue;
        }
        out = line;
        return true;
    }
}

bool readYesNo(const std::string& prompt) {
    while (true) {
        std::string line = getLineOrThrow(prompt);
        if (line == "y" || line == "Y" || line == "yes" || line == "YES") return true;
        if (line == "n" || line == "N" || line == "no" || line == "NO") return false;
        std::cout << "  Error: please answer y or n.\n";
    }
}
