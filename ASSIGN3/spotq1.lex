%{#include<stdio.h>
%}

%%
[A]+[B]+   {printf("valid string");}
.*      {printf("invalid string");}
%%

int yywrap(void){
    return 1;
}
int main(void){
    yylex();
    return 0;
}