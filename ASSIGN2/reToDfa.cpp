#include<bits/stdc++.h>
using namespace std;

//not using any capital letters for the RE so E as epsilon

class Node{
    public:
        int no;
        bool nullable;
        vector<int> first;
        vector<int> last;
        int c1;  //only for *  also for . , /
        int c2;  //for / and .

        Node() : no(-1), nullable(false), c1(-1), c2(-1) {} //default

        Node(int n,bool nul,vector<int> f,vector<int> l,int c11,int c22){
            no = n;
            nullable = nul;
            first = f;
            last = l;
            c1 = c11;
            c2 = c22;
        }
};


//find the firstpos and lastpos for every symbol....

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

int findC1(string s,int i){
    if(s[i-1] == ')'){
        i-=2;
        while(i>=0){
            if(s[i] == '|' || s[i] == '.' || s[i] == '*'){
                return i;
            }
            i--;
        }
    }

    return i-1;
}
int findC2(string s,int i){
    if(s[i+1] == '('){
        i+=2;
        while(s[i] != ')'){
            if(s[i] == '|' || s[i] == '.' || s[i] == '*'){
                return i;
            }
            i++;
        }
    }

    return i+1;
}

int dotC1(const string &augString, int dotIndex) {
    stack<char> st; 
    for (int i = dotIndex - 1; i >= 0; --i) {
        char c = augString[i];

        if (c == ')') {
            st.push(c); 
        } else if (c == '(') {
            if (!st.empty() && st.top() == ')') {
                st.pop(); 
            } else {
                return i; 
            }
        } else if (st.empty()) {
            if (c == '|' || c == '.') {
                return i;
            } else if (isalnum(c) || c == '*' || c == '#') {
                return i; 
            }
        } else {
            if (c == '|' || c == '*' || c == '.'){
                return i;
            }
        }
    }
    return -1;
}



vector<int> createFirst(char c ,int c1, int c2,vector<int> a,vector<int> b,vector<Node> tree){
    if(c == '.'){
        if(!tree[c1].nullable){
            return tree[c1].first;
        }

        vector<int> f = a;
        set<int> st(a.begin(),a.end());
        for(auto x : b){
            if(!st.count(x)){
                f.push_back(x);
            }
        }
        return f;
    }
    else{
        vector<int> f = a;
        set<int> st(a.begin(),a.end());
        for(auto x : b){
            if(!st.count(x)){
                f.push_back(x);
            }
        }
        return f;
    }
}

vector<int> createLast(char c,int c1, int c2,vector<int> a,vector<int> b,vector<Node> tree){
    if(c == '.'){
        if(!tree[c2].nullable){
            return tree[c2].last;
        }

        vector<int> f = a;
        set<int> st(a.begin(),a.end());
        for(auto x : b){
            if(!st.count(x)){
                f.push_back(x);
            }
        }
        return f;
    }
    else{
        vector<int> f = a;
        set<int> st(a.begin(),a.end());
        for(auto x : b){
            if(!st.count(x)){
                f.push_back(x);
            }
        }
        return f;
    }
}

void debug(string s,vector<Node> synt){
    cout<<"\n----------------------------------------------------------------"<<endl;
    cout<<"FOR--->  "<<s<<endl;
    int n =s.length();
    for(int i = 0;i<n;i++){
        if(s[i] == '(' || s[i] == ')'){
            continue;
        }
        cout<<"\nChar is -> "<<s[i]<<endl;
        auto v = synt[i];
        cout<<"Number: "<<v.no<<endl;
        cout<<"Nullable: "<<v.nullable<<endl;
        cout<<"FirstPos: ";
        for(int x : v.first){
            cout<<x<<" ";
        }
        cout<<endl;

        cout<<"LastPos: ";
        for(int x : v.last){
            cout<<x<<" ";
        }
        cout<<endl;

        cout<<"c1: "<<v.c1;
        cout<<"c2: "<<v.c2;
    }
    cout<<"------------------------------\n"<<endl;
}



int main(){
    ifstream file("input2.txt");
    string line;

    //create a map for char -> nullable mapping....
    map<char,bool> mp;
    for(char c = 'a';c<='z';c++){
        mp[c] = false;
    }
    mp['*'] = true;
    mp['E'] = true;

    while(getline(file,line)){
        string augString = generateString(line);
        cout<<augString<<endl;
        vector<Node> syntaxTree(augString.size());

        int number = 0;
        for(int i = 0;i<augString.size();i++){   //fill the shoes for terminals
            if(augString[i] == ')' || augString[i] == '('){
                syntaxTree[i] = Node(-1,false,{},{},-1,-1);
                continue;
            }
            if(augString[i]>='a' && augString[i]<='z' || (augString[i] == '#')){
                number++;
                syntaxTree[i] = Node(number,false,{number},{number},-1,-1);
            }
        }


        for(int i = 0;i<augString.size();i++){
            if(augString[i] == '*'){
                //find the c1 and then create the node
                int c1 = findC1(augString,i); //index of c1;
                syntaxTree[i] = Node(-1,true,syntaxTree[c1].first,syntaxTree[c1].last,c1,-1);
            }
            else if(augString[i] == '.'){
                int c1 = i-1;
                int c2 = i+1;
                vector<int> first1 = createFirst('.',c1,c2,syntaxTree[c1].first,syntaxTree[c2].first,syntaxTree);
                vector<int> last1 = createLast('.',c1,c2,syntaxTree[c1].last,syntaxTree[c2].last,syntaxTree);

                syntaxTree[i] = Node(-1,(syntaxTree[c1].nullable & syntaxTree[c2].nullable),first1,last1,c1,c2);
            }
            else if(augString[i] == '|'){
                //finding c1 and c2
                int c1 = findC1(augString,i);
                int c2 = findC2(augString,i);
                vector<int> first1 = createFirst('|',c1,c2,syntaxTree[c1].first,syntaxTree[c2].first,syntaxTree);
                vector<int> last1 = createLast('|',c1,c2,syntaxTree[c1].last,syntaxTree[c2].last,syntaxTree);

                syntaxTree[i] = Node(-1,(syntaxTree[c1].nullable & syntaxTree[c2].nullable),first1,last1,c1,c2);
            }
        }
        debug(augString,syntaxTree);
    }
    return 0;
}