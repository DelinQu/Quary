%{
/*a Lex program that changes all numbers
from decimal to hexadecimal notation,
printing a summary statistic to stderr
*/
#include<stdlib.h>
#include<stdio.h>
int count=0;
int n;
%}
digit [0-9]
number {digit}+
%%
{number}
{
n=atoi(yytext);
printf("%x",n);
if(n>9)
count++;
}
%%
int yywrap() {return 1;}
main()
{ yylex();
fprintf(stderr,"number of replacements = %d",count);
return 0;
}
