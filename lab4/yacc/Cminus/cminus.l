%{
#include "globals.h"
#include "util.h"
#include "scan.h"
#include "cminus.tab.h"

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];
extern YYSTYPE yylval;
%}

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t]+

%%

"if"		{return IF;}
"else"		{return ELSE;}
"int"		{return INT;}
"return"	{return RETURN;}
"void"		{return VOID;}
"while"		{return WHILE;}

"+"		{return PLUS;}
"-"		{return MINUS;}
"*"		{return MUL;}
"/"		{return DIV;}
"<="		{return LEQ;}
"<"		{return LES;}
">="		{return BEQ;}
">"		{return BIG;}
"=="		{return EQ;}
"!="		{return NEQ;}
"="		{return ASSIGN;}
";"		{return SEMI;}
","		{return COMMA;}
"("		{return SOPEN;}
")"		{return SCLOSE;}
"{"		{return MOPEN;}
"}"		{return MCLOSE;}
"["		{return BOPEN;}
"]"		{return BCLOSE;}
"/*"		{/* Handle comment */
		  register int c;
		  for ( ; ; ) {
		    while ( (c = input()) != '*' && c != EOF ) {
                      if (c == '\n') lineno++;
                    }
		    if ( c == '*' ) {
		      while ( (c = input()) == '*' );
		      if ( c == '/' ) break;
                      if ( c == '\n') lineno++;
		    }
		    if ( c == EOF ) {
		      return ERROR;
		    }
		  }
		}

{number}        {st_push(yytext); return NUM;}
{identifier}    {st_push(yytext); return ID;}
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}
.               {return ERROR;}
%%
void initParser() {
  yyin = source;
  yyout = listing;
}

TokenType getToken(void)
{ static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
  { firstTime = FALSE;
    lineno++;
    yyin = source;
    yyout = listing;
  }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  if (TraceScan) {
    fprintf(listing,"\t%d",lineno);
    printToken(currentToken,tokenString);
  }
  return currentToken;
}

