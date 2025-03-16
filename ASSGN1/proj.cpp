#include <bits/stdc++.h>
using namespace std;

// Regular expressions for different data types
regex intRegex(R"(^-?\d+$)");               // Matches integers
regex floatRegex(R"(^-?\d*\.\d+$)");        // Matches floating-point numbers
regex boolRegex(R"(^\b(True|False)\b$)");   // Matches boolean values
regex strRegex(R"(^(['\"])(.*?)\1$)");      // Matches strings in '...' or "..."
regex listRegex(R"(^\[(.*)\]$)");           // Matches lists

// Function to check data types
bool isInteger(string s) { return regex_match(s, intRegex); }
bool isFloat(string s) { return regex_match(s, floatRegex); }
bool isBoolean(string s) { return regex_match(s, boolRegex); }
bool isString(string s) { return regex_match(s, strRegex); }
bool isList(string s) { return regex_match(s, listRegex); }

int main() {
    ifstream file("python.txt");
    string line;
    int row = 0;

    while (getline(file, line)) {
        string s;
        for (int i = 0; i < line.length(); i++) {
            char x = line[i];

            // Separators (space, operators, punctuations)
            if (isspace(x) || x == '=' || x == ',' || x == '(' || x == ')' || x == ':') {
                if (!s.empty()) { // Check the token before separator
                    if (isInteger(s)) {
                        cout << "Integer: " << s << endl;
                    } else if (isFloat(s)) {
                        cout << "Float: " << s << endl;
                    } else if (isBoolean(s)) {
                        cout << "Boolean: " << s << endl;
                    } else if (isString(s)) {
                        cout << "String: " << s << endl;
                    } else if (isList(s)) {
                        cout << "List: " << s << endl;
                    } else {
                        cout << "Identifier: " << s << endl; // Variable names
                    }
                    s.clear();
                }
            } else {
                s.push_back(x); // Build token
            }
        }

        // Last token in the line
        if (!s.empty()) {
            if (isInteger(s)) {
                cout << "Integer: " << s << endl;
            } else if (isFloat(s)) {
                cout << "Float: " << s << endl;
            } else if (isBoolean(s)) {
                cout << "Boolean: " << s << endl;
            } else if (isString(s)) {
                cout << "String: " << s << endl;
            } else if (isList(s)) {
                cout << "List: " << s << endl;
            } else {
                cout << "Identifier: " << s << endl;
            }
        }

        row++;
    }

    return 0;
}
