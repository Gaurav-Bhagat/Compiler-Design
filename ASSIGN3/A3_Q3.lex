%{
    #include<bits/stdc++.h>
    using namespace std;

    extern "C" {
        int yylex();
        int yywrap();
    }
%}

%%

[0-9]+[\+\-\*/\^\%][0-9]+  {
    string input(yytext);   
    char op;
    string num1, num2;
    int ans = 0;

    for (int i = 0; i < input.length(); i++) {
        if (input[i] == '*' || input[i] == '/' || input[i] == '^' || input[i] == '+' || input[i] == '-' || input[i] == '%') {
            op = input[i];
            num1 = input.substr(0, i);
            num2 = input.substr(i + 1);
            break;
        }
    }

    int n1 = stoi(num1);
    int n2 = stoi(num2);

    switch (op) {
        case '+': 
			ans = n1 + n2; 
			break;
        case '-': 
			ans = n1 - n2; 
			break;
        case '*': 
			ans = n1 * n2; 
			break;
        case '/': 
			ans = (n2 != 0) ? (n1 / n2) : 0; 
			break;
        case '^': 
			ans = pow(n1, n2); 
			break;
        case '%': 
			ans = n1 % n2; 
			break;
    }

    cout << "The result is: " << ans << endl;
}

.*  { cout << "Invalid input" << endl; }

%%

int yywrap() { return 1; }

int main() {
    cout << "Enter an expression: ";
    yylex();
    return 0;
}
