// InputUtils.h
// CS212 Project 94 - Tailoring Order Workshop
// Line-based, validated console input. Every read consumes a whole line, so a
// bad entry (letters, empty line, out of range) never leaves junk in std::cin.
#ifndef INPUT_UTILS_H
#define INPUT_UTILS_H

#include <stdexcept>
#include <string>

// Thrown when standard input ends (Ctrl+Z / Ctrl+D or end of a redirected file),
// so the program can shut down cleanly instead of looping forever.
class InputEnded : public std::runtime_error {
public:
    InputEnded() : std::runtime_error("input ended") {}
};

// Re-prompts until the user types a whole integer in [minVal, maxVal].
int readInt(const std::string& prompt, int minVal, int maxVal);

// Re-prompts until the user types a number in [minVal, maxVal].
double readDouble(const std::string& prompt, double minVal, double maxVal);

// Re-prompts until a non-empty line (after trimming) of at most maxLen chars.
std::string readNonEmptyLine(const std::string& prompt, std::size_t maxLen = 60);

// Reads a line; an empty line means "keep current value" and returns false.
bool readOptionalLine(const std::string& prompt, std::string& out, std::size_t maxLen = 60);

// Reads y/n (case-insensitive). Re-prompts on anything else.
bool readYesNo(const std::string& prompt);

#endif // INPUT_UTILS_H
