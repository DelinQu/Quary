/*带有二义性文法和算符的优先权及结合性规则的简单计算器*/
%{
#include <stdio.h>
#include <ctype.h>
%}

%token NUMBER

%%

command : exp { printf("%d\n", $1); }
	; /* allows printing of the result */
exp	:  exp '+' term {$$ = $1 + $3;}
	|  exp '-'  term {$$ = $1 - $3;}
	|  term  {$$ = $1;}
	;

term	:  term '*' factor {$$ = $1 = $1 * $3;}
	|  factor {$$ = $1;}
	;

factor	: NUMBER  {$$ = $1;}
	| '(' exp ')' {$$ = $2;}
	;
%%

main()
{ return yyparse();
}

int yylex(void)
{ int c;
  while((c = getchar()) == ' ' );
  /* eliminates blanks */
  if( isdigit(c) )
  {
    ungetc(c, stdin);
    scanf("%d", &yylval);
    return(NUMBER);
  }
  if (c == '\n') return 0;
  /* makes the parse stop */
  return(c);
}

void yyerror(char * s)
{
fprintf(stderr, "%s\n", s);
} /* allows for printing of an error message */
