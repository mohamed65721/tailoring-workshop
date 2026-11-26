// InputUtils.cpp
// CS212 Project 94 - Tailoring Order Workshop (simplified course-style version)

#include <iostream>
#include <string>
#include "InputUtils.h"

using namespace std;

// Removes spaces, tabs and line-end characters from both ends of a string.
string trim(string s) {
    int b = 0;
    int e = (int)s.size() - 1;
    while (b <= e && (s[b] == ' ' || s[b] == '\t' || s[b] == '\r' || s[b] == '\n')) b++;
    while (e >= b && (s[e] == ' ' || s[e] == '\t' || s[e] == '\r' || s[e] == '\n')) e--;
    return s.substr(b, e - b + 1);
}

// Shows the prompt and reads one whole line. At end of input the program stops cleanly.
bool ended = false;     // becomes true when the input runs out

bool inputHasEnded() { return ended; }

string readLine(string prompt) {
    if (ended) return "";
    cout << prompt;
    string line;
    if (!getline(cin, line)) {
        cout << "\n(Input ended.)\n";
        ended = true;
        return "";
    }
    // Windows editors/shells may put a UTF-8 byte-order mark at the start of redirected input.
    if (line.size() >= 3 && line.compare(0, 3, "\xEF\xBB\xBF") == 0) line.erase(0, 3);
    return trim(line);
}

string lowerCopy(string s) {
    string r = s;
    for (int i = 0; i < (int)r.size(); i++)
        if (r[i] >= 'A' && r[i] <= 'Z') r[i] = r[i] + ('a' - 'A');
    return r;
}

// Converts text such as "42", "-7" or "+5" to a number. Returns false if the text
// is not a whole number. Very long numbers are clamped (they fail the range check).
bool parseInt(string text, int& value) {
    int i = 0;
    bool negative = false;
    if (i < (int)text.size() && (text[i] == '+' || text[i] == '-')) {
        negative = (text[i] == '-');
        i++;
    }
    if (i >= (int)text.size()) return false;           // no digits
    int v = 0;
    int digits = 0;
    for (; i < (int)text.size(); i++) {
        if (text[i] < '0' || text[i] > '9') return false;
        if (digits < 9) v = v * 10 + (text[i] - '0');
        digits++;
    }
    if (digits > 9) v = 1000000000;           // too big: will fail the range check
    if (negative) v = -v;
    value = v;
    return true;
}

// Converts text such as "42", "42.5" or ".5" to a number. Returns false otherwise.
bool parseDouble(string text, double& value) {
    int i = 0;
    bool negative = false;
    if (i < (int)text.size() && (text[i] == '+' || text[i] == '-')) {
        negative = (text[i] == '-');
        i++;
    }
    double whole = 0.0;
    int digits = 0;
    while (i < (int)text.size() && text[i] >= '0' && text[i] <= '9') {
        if (digits < 15) whole = whole * 10 + (text[i] - '0');
        else whole = 1e15;
        digits++;
        i++;
    }
    double fraction = 0.0;
    double divisor = 1.0;
    int fracDigits = 0;
    if (i < (int)text.size() && text[i] == '.') {
        i++;
        while (i < (int)text.size() && text[i] >= '0' && text[i] <= '9') {
            if (fracDigits < 12) {
                fraction = fraction * 10 + (text[i] - '0');
                divisor = divisor * 10;
            }
            fracDigits++;
            i++;
        }
    }
    if (i != (int)text.size()) return false;           // extra characters
    if (digits + fracDigits == 0) return false;        // no digits at all
    double v = whole + fraction / divisor;
    if (negative) v = -v;
    value = v;
    return true;
}

int readInt(string prompt, int minVal, int maxVal) {
    while (true) {
        string line = readLine(prompt);
        if (ended) return 0;
        int value = 0;
        if (line.empty() || !parseInt(line, value)) {
            cout << "  Error: please enter a whole number.\n";
            continue;
        }
        if (value < minVal || value > maxVal) {
            cout << "  Error: value must be between " << minVal << " and " << maxVal << ".\n";
            continue;
        }
        return value;
    }
}

double readDouble(string prompt, double minVal, double maxVal) {
    while (true) {
        string line = readLine(prompt);
        if (ended) return 0.0;
        double value = 0.0;
        if (line.empty() || !parseDouble(line, value)) {
            cout << "  Error: please enter a number (e.g. 42 or 42.5).\n";
            continue;
        }
        if (value < minVal || value > maxVal) {
            cout << "  Error: value must be between " << minVal << " and " << maxVal << ".\n";
            continue;
        }
        return value;
    }
}

string readNonEmptyLine(string prompt, int maxLen) {
    while (true) {
        string line = readLine(prompt);
        if (ended) return "";
        if (line.empty()) {
            cout << "  Error: this field cannot be empty.\n";
            continue;
        }
        if ((int)line.size() > maxLen) {
            cout << "  Error: maximum length is " << maxLen << " characters.\n";
            continue;
        }
        return line;
    }
}

bool readOptionalLine(string prompt, string& out, int maxLen) {
    while (true) {
        string line = readLine(prompt);
        if (ended) return false;
        if (line.empty()) return false;
        if ((int)line.size() > maxLen) {
            cout << "  Error: maximum length is " << maxLen << " characters.\n";
            continue;
        }
        out = line;
        return true;
    }
}

bool readYesNo(string prompt) {
    while (true) {
        string line = lowerCopy(readLine(prompt));
        if (ended) return false;
        if (line == "y" || line == "yes") return true;
        if (line == "n" || line == "no") return false;
        cout << "  Error: please answer y or n.\n";
    }
}

string intToText(int n) {
    if (n == 0) return "0";
    bool neg = n < 0;
    if (neg) n = -n;
    string digits = "";
    while (n > 0) {
        digits = char('0' + n % 10) + digits;
        n = n / 10;
    }
    if (neg) digits = "-" + digits;
    return digits;
}
