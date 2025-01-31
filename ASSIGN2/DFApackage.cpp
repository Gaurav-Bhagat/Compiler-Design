#include <bits/stdc++.h>
using namespace std;

class Node{
    public:
        char c;
        int no;
        bool nullable;
        vector<int> first;
        vector<int> last;
        Node* c1;  
        Node* c2;  

        Node() : c('-'),no(-1), nullable(false), c1(NULL), c2(NULL) {} //default

        Node(char ch,int n,bool nul,vector<int> f,vector<int> l,Node* c11,Node* c22){
            c = ch;
            no = n;
            nullable = nul;
            first = f;
            last = l;
            c1 = c11;
            c2 = c22;
        }
};

class TT{
    public:
        char state;
        char input;
        char nxt;

        TT(){};
        TT(char s,char i,char n){
            state = s;
            input = i;
            nxt = n;
        }
};

//not using any capital letters in the RE because taking E as epsilon

map<char,bool> mp;                  //for mapping of nullable
map<int,char> terminalsMapping;    //a -> 1 , b->2 numbers assigning to the terminals
set<char> ip;                      //input symbols (a,b)
map<int,vector<int>> followPos;    //<unique id,followpos(vector)>
map<vector<int>,char> help;        //this is a helping map for giving the vector a state name like ({1,2,3} -> A) , ({1,2,3,4} -> B)
vector<TT*> transTable;            //this the transaction table 


int precedence(char op) {
    if(op == '|') return 1;
    if (op == '.' || op == '-') return 2;
    if (op == '*' || op == '/') return 3;
    return 0;
}

string infixToPostfix(const string& infix) {
    stack<char> operators;
    string postfix;

    for (char ch : infix) {
        if (isalnum(ch) || ch == '#') {
            postfix += ch;
        }
        else if (ch == '(') {
            operators.push(ch);
        }
        else if (ch == ')') {
            while (!operators.empty() && operators.top() != '(') {
                postfix += operators.top();
                operators.pop();
            }
            if (!operators.empty()) operators.pop();  
        }
        else {
            while (!operators.empty() && precedence(operators.top()) >= precedence(ch)) {
                postfix += operators.top();
                operators.pop();
            }
            operators.push(ch);
        }
    }

    while (!operators.empty()) {
        postfix += operators.top();
        operators.pop();
    }

    return postfix;
}

string generateString(string ip){
    stack<char> st; // Stack to store characters
    string result;  // Processed regex

    for (int i = 0; i < ip.size(); ++i) {
        char x = ip[i];

        if (x == '(' || x == ')') {
            result += x;
            continue;
        }

        if (result.empty()) {
            result.push_back(x);
        } else {
            char prev = result.back();

            if ((prev >= 'a' && prev <= 'z' && x >= 'a' && x <= 'z') || // ab
                (prev == '*' && x >= 'a' && x <= 'z') ||               // a*b
                (prev >= 'a' && prev <= 'z' && x == '(') ||            // a(
                (prev == ')' && x >= 'a' && x <= 'z') ||               // )a
                (prev == '*' && x == '(') ||                          // a*(
                (prev == ')' && x == '(')) {                          // )(
                result.push_back('.'); // Add concatenation
            }
            result.push_back(x); // Add the current character
        }
    }

    result += ".#";
    return result;
}

void debug(string s,vector<Node*> synt){
    cout<<"\n----------------------------------------------------------------"<<endl;
    cout<<"FOR--->  "<<s<<endl;
    int n =s.length();
    for(int i = 0;i<n;i++){
        cout<<"\nChar is -> "<<s[i]<<endl;
        auto v = synt[i];
        cout<<"Number: "<<v->no<<endl;
        cout<<"Nullable: "<<v->nullable<<endl;
        cout<<"FirstPos: ";
        for(int x : v->first){
            cout<<x<<" ";
        }
        cout<<endl;

        cout<<"LastPos: ";
        for(int x : v->last){
            cout<<x<<" ";
        }
        cout<<endl;

        cout << "c1: " << (v->c1 ? v->c1->c : 'N') << " ";
        cout << "c2: " << (v->c2 ? v->c2->c : 'N') << endl;
    }
    cout<<"------------------------------\n"<<endl;
}


vector<int> mergeUnique(vector<int>& v1, vector<int>& v2) {
    set<int> s(v1.begin(), v1.end());
    s.insert(v2.begin(), v2.end()); 

    return vector<int>(s.begin(), s.end());
}

void printTransitionTable(vector<TT*>& transTable) {
    if (transTable.empty()) {
        cout << "No transitions available!" << endl;
        return;
    }

    cout << "\n================= Transition Table =================\n";
    cout << setw(10) << "State" << setw(10) << "Input" << setw(15) << "Next State" << endl;
    cout << "---------------------------------------------------\n";

    for (TT* t : transTable) {
        cout << setw(10) << t->state << setw(10) << t->input << setw(15) << t->nxt << endl;
    }

    cout << "===================================================\n";
}
