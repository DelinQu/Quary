%{
/* Selects only lines that end or
begin with the letter 'a'.
Deletes everything else.
*/
#include <stdio.h>
%}
ends_with_a .*a\n
begins_with_a a.*\n
%%
{ends_with_a} ECHO;
{begins_with_a} ECHO;
.*\n;
%%
int yywrap()
{return 1;}
main()
{ yylex(); return 0; }