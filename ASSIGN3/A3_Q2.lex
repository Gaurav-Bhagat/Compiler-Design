%{#include<stdio.h>
%}

%%
[A-Z][a-z]+[0-9]+[!@#$%^&*] {printf("valid string");}
.*             {printf("invalid string");}
%%

int yywrap(void){
    return 1;
}
int main(void){
    printf("Enter the string:\n");
    yylex();
    return 0;
}