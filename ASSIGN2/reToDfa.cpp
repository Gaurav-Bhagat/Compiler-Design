#include<bits/stdc++.h>
#include "DFApackage.cpp"
using namespace std;

void showFollowPos() {
    for (auto x : followPos) {
        cout << "Node: " << x.first << endl;
        if (x.second.empty()) {
            cout << "No follow positions\n";
        } 
        else {
            for (auto p : x.second) {
                cout << p << " ";
            }
        }
        cout << endl;
    }
}


void mapObjToNode(string s,vector<Node*>& arr){
    int n = 1;
    for(int i = 0;i<s.length();i++){
        if((s[i]>='a' && s[i]<='z') || (s[i] == '#')){
            arr[i] = new Node(s[i],n,mp[s[i]],{n},{n},NULL,NULL);
            terminalsMapping[n] = s[i];
            if(s[i] != '#') ip.insert(s[i]);
            n++;
        }
        else if(s[i] == 'E'){
            arr[i] = new Node(s[i],n,mp[s[i]],{},{},NULL,NULL);
            n++;
        }
        else {
            arr[i] = new Node(s[i],-1,mp[s[i]],{},{},NULL,NULL);
        }
    }
}

void makeSyntaxTree(string s, vector<Node*>& arr) {
    stack<Node*> st;

    for (int i = 0; i < arr.size(); i++) {
        if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] == '#') || s[i] == 'E') {
            st.push(arr[i]);
        } 
        else if (s[i] == '|' || s[i] == '.') {
            if (st.size() < 2) {
                cerr << "Error: Insufficient operands for " << s[i] << endl;
                return;
            }

            Node* ch2 = st.top(); st.pop();
            Node* ch1 = st.top(); st.pop();

            arr[i]->c1 = ch1;
            arr[i]->c2 = ch2;

            arr[i]->nullable = (s[i] == '|') ? (ch1->nullable || ch2->nullable) : (ch1->nullable && ch2->nullable);

            st.push(arr[i]);
        } 
        else if (s[i] == '*') {
            if (st.empty()) {
                cerr << "Error: Insufficient operands for *" << endl;
                return;
            }

            Node* ch1 = st.top(); st.pop();
            arr[i]->c1 = ch1;

            st.push(arr[i]);
        }
    }
}


void createFirstPos(Node* n) {
    if (!n) return;  

    if (n->c == '|') {
        if (n->c1 && n->c2) {  
            n->first = mergeUnique(n->c1->first, n->c2->first);
        }
    } 
    else if (n->c == '.') {
        if (n->c1 && n->c2) {
            if (n->c1->nullable) {
                n->first = mergeUnique(n->c1->first, n->c2->first);
            } else {
                n->first = n->c1->first;
            }
        }
    } 
    else if (n->c == '*') {  
        if (n->c1) {
            n->first = n->c1->first;
        }
    }
}



void createLastPos(Node* n) {
    if (!n) return;

    if (n->c == '|') {
        if (n->c1 && n->c2) {  
            n->last = mergeUnique(n->c1->last, n->c2->last);
        }
    } 
    else if (n->c == '.') {
        if (n->c1 && n->c2) {
            if (n->c2->nullable) {
                n->last = mergeUnique(n->c1->last, n->c2->last);
            } else {
                n->last = n->c2->last;
            }
        }
    } 
    else if (n->c == '*') {  
        if (n->c1) {
            n->last = n->c1->last;
        }
    }
}


void makeFollowPos(Node* n){
    if(n->c == '.'){
        vector<int> firstc2 = n->c2->first;
        vector<int> lastc1 = n->c1->last;

        for(int i = 0;i<firstc2.size();i++){    
            for(int j = 0;j<lastc1.size();j++){
                followPos[lastc1[j]].push_back(firstc2[i]);
            }
        }
    }
    else if(n->c == '*'){  // *
        vector<int> firstN = n->first;
        vector<int> lastN = n->last;

        for(int i = 0;i<firstN.size();i++){
            for(int j = 0;j<lastN.size();j++){
                followPos[lastN[i]].push_back(firstN[j]);
            }
        }
    }
}

void makeTT(queue<vector<int>>& q, char& k) {
    if (q.empty()) return; 

    vector<int> curr = q.front();
    q.pop();

    for (auto i : ip) {
        vector<int> fop;

        for (auto x : curr) {
            if (terminalsMapping.count(x) && terminalsMapping[x] == i) {
                fop.push_back(x);
            }
        }

        if (fop.empty()) continue;  

        vector<int> state;
        if (followPos.count(fop[0])) {
            state = followPos[fop[0]];
        }

        for (size_t j = 1; j < fop.size(); j++) {
            if (followPos.count(fop[j])) {
                state = mergeUnique(state, followPos[fop[j]]);
            }
        }

        if (state.empty()) continue;

        if (help.find(state) == help.end()) {  
            help[state] = k;
            k++;
            q.push(state);
        }

        transTable.push_back(new TT(help[curr], i, help[state]));
    }
}


int main(){
    ifstream file("input2.txt");
    string line;

    //create a map for char -> nullable mapping....
    for(char c = 'a';c<='z';c++){
        mp[c] = false;
    }
    mp['*'] = true;
    mp['E'] = true;

    while(getline(file,line)){                 
        terminalsMapping.clear();    
        ip.clear();                      
        followPos.clear();    
        help.clear();        
        transTable.clear();            

        string s = generateString(line);
        cout<<"Augmented String: "<<s<<endl;
        string postS = infixToPostfix(s);
        cout<<"Augmented String(Postfix): "<<postS<<endl;
        vector<Node*> obj(postS.size(),NULL);

        mapObjToNode(postS,obj);

        makeSyntaxTree(postS,obj);

        //creating firstpos
        for(int i = 0;i<obj.size();i++){
            if (!obj[i]) continue;

            if(obj[i]->c == 'E' ||(obj[i]->c >= 'a'&& obj[i]->c <= 'z')){
                continue;
            }

            createFirstPos(obj[i]);
        }

        //creating lastpos
        for(int i = 0;i<obj.size();i++){
            if (!obj[i]) continue;
            if(obj[i]->c == 'E' ||(obj[i]->c >= 'a'&& obj[i]->c <= 'z')){
                continue;
            }

            createLastPos(obj[i]);
        }
        for(int i = 0;i<obj.size();i++){
            makeFollowPos(obj[i]);
        }
        //last element of the  obj is the root node
        Node* root = obj.back();
        char k = 'A';
        help[root->first] = k;
        k++;
        queue<vector<int>> q;
        q.push(root->first);

        while(!q.empty()){
            makeTT(q,k);
        }
        //showFollowPos();
        //debug(postS,obj);

        printTransitionTable(transTable);
        
    }
    return 0;
}