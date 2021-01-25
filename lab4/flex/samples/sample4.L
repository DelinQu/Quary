%{
/*  Lex program to convert uppercase to
lowercase except inside comments
*/
#include <stdio.h>
#ifndef FALSE
#define FALSE 0
# endif
#ifndef TRUE
#define TRUE 1
# endif
%}
%%
[A-Z]  {putchar(tolower(yytext[0]));
/* yytext[0] is the single
uppercase char found */
}
"/*"   { char c ;
int done = FALSE;
ECHO ;
do
{ while ((c=input())!='*')
putchar(c);
putchar(c); 
while((c=input())=='*') 
putchar(c);
putchar(c);
if (c == '/') done = TRUE;
} while (!done);
}
%%
int yywrap(){return 1;}
void main(void)
{ yylex();}
