%{#include<stdio.h>
int countA = 0;
int countB = 0;
%}

%%
A   {countA++;}
B   {countB++;}
\n   {
    printf("A: %d\nB: %d\n", countA, countB);
    if(countA == countB)printf("Valid!!\n");
    else{
        printf("Invalid!!\n");
        return 0;
    }
}
[a-z0-9C-Z] {
    printf("Invalid!!");
}

%%

int yywrap(void){
    return 1;
}
int main(void){
    yylex();
    return 0;
}