%{#include<stdio.h>
%}

%%
1(.|1|0)+(101)  {printf("valid string");}
.*             {printf("invalid string");}
%%

int yywrap(void){
    return 1;
}
int main(void){
    yylex();
    return 0;
}
