// InputUtils.h
// CS212 Project 94 - Tailoring Order Workshop (simplified course-style version)
// Line-based, validated console input. Every read takes a whole line, so a
// bad entry (letters, empty line, out of range) never leaves junk in cin.
// When the input ends (Ctrl+Z / Ctrl+D or end of a redirected file) the
// readers print a short message and set a flag; the menu code checks it and stops cleanly.
#ifndef INPUTUTILS_H
#define INPUTUTILS_H

#include <string>

using namespace std;

// True once the input has run out (the readers then return empty/zero values at once).
bool inputHasEnded();

// Re-prompts until the user types a whole integer in [minVal, maxVal].
int readInt(string prompt, int minVal, int maxVal);

// Re-prompts until the user types a number in [minVal, maxVal].
double readDouble(string prompt, double minVal, double maxVal);

// Re-prompts until a non-empty line (after trimming) of at most maxLen characters.
string readNonEmptyLine(string prompt, int maxLen);

// Reads a line; an empty line means "keep current value" and returns false.
bool readOptionalLine(string prompt, string& out, int maxLen);

// Reads y/n (any letter case, also yes/no). Re-prompts on anything else.
bool readYesNo(string prompt);

string intToText(int n);          // number -> text (no to_string needed)

#endif // INPUTUTILS_H
