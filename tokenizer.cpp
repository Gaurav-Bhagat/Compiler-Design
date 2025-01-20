#include <iostream>
#include <regex>
#include <string>
#include <fstream> //file purposes...
using namespace std;
void extractToken(string s,int row,int col){
	regex var("^[a-zA-Z_][a-zA-Z0-9_]*$");
	regex num("^[+-]?\\d+(\\.\\d+)?([eE][+-]?\\d+)?$");
	regex key(R"(\b(int|char|void|if|else|for|float|while|return|break|continue|switch|case|default|struct|union|typedef|namespace|public|private|protected|class|new|delete|try|catch|throw|inline|virtual|static|const|volatile|friend|explicit|auto|long|short|signed|unsigned|true|false|nullptr)\b)");
	regex op(R"([+\-*/%=!&|<>^~])");
	// regex punct("([;,\?]{1,2})");

	if(regex_match(s, op)){
		cout << "Operator: " << s << " at (" << row << "," << col<< ")" << endl;
	}
	else if(regex_match(s, key)){
		cout << "Keyword: " << s << " at (" << row << "," << col<< ")" << endl;
	}
	else if(regex_match(s, var)){
		cout << "Variable: " << s << " at (" << row << "," << col<< ")" << endl;
	}
	else if(regex_match(s, num)){
		cout << "Number: " << s << " at (" << row << "," << col<< ")" << endl;
	}
	// else if(regex_match(s, punct)){
	// 	cout << "Punctuations: " << s << " at (" << row << "," << col<< ")" << endl;
	// }
	else{
		cout << "Invalid Token: " << s << " at (" << row << "," << col<< ")" << endl;
	}
}
string trim(const string &str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v"); 
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r\f\v"); 
    return str.substr(first, (last - first + 1));
}
int main(){
	ifstream file("input.c");
	string line;
	int row = 0;
	while(getline(file,line)){
		string s ;
		// cout<<line<<endl;
		for(int i = 0;i<line.length();i++){
			// if(line[i] == '{' || line[i] == '}' || line[i] == '(' || line[i] == ')' || line[i] == ',' || line[i] == ';'){
			// 	cout << "Punctuations: " << line[i] << " at (" << row << "," << i << ")" << endl;
			// }
			if(line[i] == ' ' && s.length() == 0){
				continue;
			}
			if(line[i] == ' '){
				if(s.length() != 0){
					int len = s.length();
					int l = trim(s).length();
					//extractToken(trim(s),row,i-s.length()/*len-l*/);
					cout<<"->"<<s<<"<-"<<endl;
					s.clear();
				}
				//continue;
			}
			else if(line[i] != ' '){
				s += line[i];
			}
		}
		row++;
	}
	cout<<"Done"<<endl;
	return 0;
}

