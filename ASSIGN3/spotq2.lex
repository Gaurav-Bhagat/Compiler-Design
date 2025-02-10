%{
    #include<bits/stdc++.h>
    using namespace std;

    extern "C" {
        int yylex();
        int yywrap();
    }

    int countA = 0;
    int countB = 0;

%}

%%
A   {countA++;}
B   {countB++;}
\n {
    cout<<"A: "<<countA<<"B: "<<countB<<endl;
    if(countA == countB){
        printf("Valid!!\n");
    }
    else{
        printf("Invalid!!\n");
        return 0;
    }
    countA = countB = 0;
}
[^AB\n] {
    printf("Invalid!!");
    return 0;
}

%%

int yywrap(void){
    return 1;
}
int main(void){
    yylex();
    return 0;
}