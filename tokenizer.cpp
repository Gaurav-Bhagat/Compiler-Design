#include <iostream>
#include <regex>
#include <string>
#include <fstream> // For file purposes
using namespace std;

void extractToken(string s, int row, int col) {
    regex var("^[a-zA-Z_][a-zA-Z0-9_]*$");
    regex num("^[+-]?\\d+(\\.\\d+)?([eE][+-]?\\d+)?$");
    regex key(R"(\b(int|char|void|if|else|for|float|while|return|break|continue|switch|case|default|struct|union|typedef|namespace|public|private|protected|class|new|delete|try|catch|throw|inline|virtual|static|const|volatile|friend|explicit|auto|long|short|signed|unsigned|true|false|nullptr)\b)");
    regex op(R"([+\-*/%=!&|<>^~])");

    if (regex_match(s, op)) {
        cout << "Operator: " << s << " at (" << row << "," << col << ")" << endl;
    } else if (regex_match(s, key)) {
        cout << "Keyword: " << s << " at (" << row << "," << col << ")" << endl;
    } else if (regex_match(s, var)) {
        cout << "Variable: " << s << " at (" << row << "," << col << ")" << endl;
    } else if (regex_match(s, num)) {
        cout << "Number: " << s << " at (" << row << "," << col << ")" << endl;
    } else {
        cout << "Invalid Token: " << s << " at (" << row << "," << col << ")" << endl;
    }
}

int main() {
    ifstream file("input.c");
    string line;
    int row = 0;
    const int tabWidth = 4;

    while (getline(file, line)) {
        string s;
        int col = 0;
        bool insideToken = false;

        for (int i = 0; i < line.length(); i++) {
            if (line[i] == '\t') {
                col += (tabWidth - (col % tabWidth));
            } else if (isspace(line[i]) || line[i] == '{' || line[i] == '}' || line[i] == '(' || 
                       line[i] == ')' || line[i] == ',' || line[i] == ';') {
                if (insideToken) {
                    extractToken(s, row, col - s.length());
                    s.clear();
                    insideToken = false;
                }
                if (!isspace(line[i])) {
                    // string punct(1, line[i]);
                    // extractToken(punct, row, col);
                    cout<<"Punct: "<<line[i]<<" at "<<row<<" "<<col<<endl;
                }
                col++;
            } else {
                if (!insideToken && !isspace(line[i])) {
                    insideToken = true;
                }
                s += line[i];
                col++;
            }
        }
        if (!s.empty()) {
            extractToken(s, row, col - s.length());
        }
        row++;
    }

    cout << "Done" << endl;
    return 0;
}
