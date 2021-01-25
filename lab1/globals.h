/****************************************************/
/*Global types and vars for TINY compiler            /
/*Quary编译器的全局类型和变量                             /
/****************************************************/
   
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* reserved words 关键字个数*/
#define MAXRESERVED 14
typedef enum {
   /* book-keeping tokens 文件状态*/
   ENDFILE,ERROR,
   /* reserved words 关键字*/
   IF,ELSE,INT,CHAR,RETURN,VOID,WHILE,FOREACH,AND,OR,NOT,INPUT,PRINT,IN,
   /* multicharacter tokens */
   ID,NUM,
   /* special symbols 特殊字*/
   /*+ - * / < <= > >= == != = ; , ( ) [ ] { } # :*/
   ADD,SUB,MUL,DIV,LS,LE,GT,GE,EQ,NE,ASSIGN,SEMI,COMMA,LPAREN,RPAREN,LFPAREN,RFPAREN,LDPAREN,RDPAREN,COMMENT,COLON
} TokenType;

extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */
extern FILE* code; /* code text file for TM simulator */

extern int lineno; /* source line number for listing */


/*用于分析的语法树*/


typedef enum {StmtK,ExpK} NodeKind;
typedef enum {IfK,RepeatK,AssignK,ReadK,WriteK} StmtKind;
typedef enum {OpK,ConstK,IdK} ExpKind;

/* ExpType is used for type checking */
typedef enum {Void,Integer,Boolean} ExpType;

#define MAXCHILDREN 3

typedef struct treeNode
   { struct treeNode * child[MAXCHILDREN];
     struct treeNode * sibling;
     int lineno;
     NodeKind nodekind;
     union { StmtKind stmt; ExpKind exp;} kind;
     union { TokenType op;
             int val;
             char * name; } attr;
     ExpType type; /* for type checking of exps */
   } TreeNode;
   
/*Flags for tracing 跟踪标志*/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
 * and lookups to be reported to the listing file
 */
extern int TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
 * to the TM code file as code is generated
 */
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern int Error; 
#endif
