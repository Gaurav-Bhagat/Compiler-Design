#include <iostream>
#include <regex>
#include <string>
#include <fstream> // For file purposes
using namespace std;
vector<pair<string,pair<int,int>>> operators;
vector<pair<string,pair<int,int>>> keyword;
vector<pair<string,pair<int,int>>> variables;
vector<pair<string,pair<int,int>>> nums;
vector<pair<string,pair<int,int>>> punct;

void extractToken(string s, int row, int col) {
    regex var("^[a-zA-Z_][a-zA-Z0-9_]*$");
    regex num("^[+-]?\\d+(\\.\\d+)?([eE][+-]?\\d+)?$");
    regex key(R"(\b(int|char|void|if|else|for|float|while|return|break|printf|continue|switch|case|default|struct|union|typedef|namespace|public|private|protected|class|new|delete|try|catch|throw|inline|virtual|static|const|volatile|friend|explicit|auto|long|short|signed|unsigned|true|false|nullptr)\b)");
    regex op(R"([+\-*/%=!&|<>^~])");

    if (regex_match(s, op)) {
        operators.push_back(make_pair(s, make_pair(row, col)));
    } else if (regex_match(s, key)) {
        keyword.push_back(make_pair(s, make_pair(row, col)));
    } else if (regex_match(s, var)) {
        variables.push_back(make_pair(s, make_pair(row, col)));
    } else if (regex_match(s, num)) {
        nums.push_back(make_pair(s, make_pair(row, col)));
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
                    punct.push_back(make_pair(string(1, line[i]), make_pair(row, col)));
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

    cout<<"---------------------------------------------------"<<endl;
    if(keyword.size() > 0){
        cout<<"------------------Keywords-------------"<<endl;
        for(auto p : keyword){
            string str = p.first;
            int row = p.second.first;
            int col = p.second.second;
            cout <<  str << " at Row: " << row << "  Col: " << col << endl;
        }
    }
    cout<<"---------------------------------------------------"<<endl;
    if(variables.size() > 0){  
        cout<<"------------------Variables-------------"<<endl;
        for(auto p : variables){
            string str = p.first;
            int row = p.second.first;
            int col = p.second.second;
            cout <<  str << " at Row: " << row << "  Col: " << col << endl;
        }
    }
    cout<<"---------------------------------------------------"<<endl;
    if(operators.size() > 0){
        cout<<"------------------Operators-------------"<<endl;
        for(auto p : operators){
            string str = p.first;
            int row = p.second.first;
            int col = p.second.second;
            cout <<  str << " at Row: " << row << "  Col: " << col << endl;
        }
    }
    cout<<"---------------------------------------------------"<<endl;
    if(nums.size() > 0){
        cout<<"------------------Numbers-------------"<<endl;
        for(auto p : nums){
            string str = p.first;
            int row = p.second.first;
            int col = p.second.second;
            cout <<  str << " at Row: " << row << "  Col: " << col << endl;
        }
    }
    cout<<"---------------------------------------------------"<<endl;
    if(punct.size() > 0){
        cout<<"------------------Punctuations-------------"<<endl;
        for(auto p : punct){
            string str = p.first;
            int row = p.second.first;
            int col = p.second.second;
            cout <<  str << " at Row: " << row << "  Col: " << col << endl;
        }
    }
    file.close(); 
    cout << "Done" << endl;
    return 0;
}
