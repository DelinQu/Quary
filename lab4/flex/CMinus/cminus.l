/* File: tiny.l               */
/* Lex specification for TINY */
/* CMinus 修改版词法LEx程序     */

%{
#include "globals.h"
#include "util.h"
#include "scan.h"
/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];
%}
 
digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t]+

%%
 
"if"            {return IF;}
"while"          {return WHILE;}
"else"          {return ELSE;}
"return"         {return RETURN;}
"int"          {return INT;}
"void"         {return VOID;}
"="            {return ASSIGN;}
"=="             {return EQ;}
"<"             {return LT;}
"<="             {return LTEQ;}
">"             {return RT;}
">="             {return RTEQ;}
"!="             {return UNEQ;}
"+"             {return PLUS;}
"-"             {return MINUS;}
"*"             {return TIMES;}
","             {return NOB;}
"/"             {return OVER;}
"("             {return LPAREN;}
")"             {return RPAREN;}
";"             {return SEMI;}
"["             {return MLPAREN;}
"]"             {return MRPAREN;}
"{"             {return LLPAREN;}
"}"             {return LRPAREN;}
{number}        {return NUM;}
{identifier}    {return ID;}
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}
"/*"            {     char c;
                      char d='f';
                      do
                      {     c = input();
                            if (c == EOF) break;
                            if (c == '\n') lineno++;
                            if (c == '*'){
                                c = input();
                                if (c == '/') d = 'a';
                            }
                      } while (d == 'f');
                }
.               {return ERROR;}
 
%%
TokenType getToken(void)
{    static int firstTime = TRUE;
     TokenType currentToken;
     if (firstTime)
     {     firstTime = FALSE;
           lineno++;
           yyin = source;
           yyout = listing;
     }
     currentToken = yylex();
     strncpy(tokenString,yytext,MAXTOKENLEN);
     if (TraceScan) {
        fprintf(listing,"\t%d: ",lineno);
        printToken(currentToken,tokenString);
     }
     return currentToken;
}