
/*
*Quary编译器的全局类型和变量
*/

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

/* 保留字个数*/
#define MAXRESERVED 15
typedef enum {
   //文件状态
   ENDFILE,ERROR,
   //保留字
   IF,ELSE,INT,CHAR,RETURN,VOID,WHILE,FOREACH,AND,OR,NOT,INPUT,PRINT,IN,END,
   //数字和标识符
   ID,NUM,
   // 特殊字 + - * / < <= > >= == != = ; , ( ) [ ] { } # :
   ADD,SUB,MUL,DIV,LS,LE,GT,GE,EQ,NE,
   ASSIGN,SEMI,COMMA,                                       //赋值= 分号;  逗号, 
   LPAREN,RPAREN,LFPAREN,RFPAREN,LDPAREN,RDPAREN,
   COMMENT,COLON                                            //注释# 冒号:   
} TokenType;

extern FILE* source;    //文件
extern FILE* listing;   //文件输出
extern FILE* code;      //为TM模拟器的文本

extern int lineno;      //当前读取到第几行


/*用于分析的语法树*/
typedef enum {StmtK,ExpK} NodeKind;                         //节点类型
typedef enum {IfK,WhileK,ForEach,AssignK,InputK,PrintK} StmtKind;   //语句类型（statment）
typedef enum {OpK,ConstK,IdK} ExpKind;                      //表达式类型（expression）算符表达式、常量表达式和标识符表达式
/*ExpType用于类型检查*/
typedef enum {Void,Integer,Boolean} ExpType;                //表达式种类   

#define MAXCHILDREN 3

//语法树的节点
typedef struct treeNode                   //语法树的节点
{ 
   struct treeNode * child[MAXCHILDREN];  //子节点, 三元
   struct treeNode * sibling;             //兄弟节点
   int lineno;                            //当前读取到的行号
   NodeKind nodekind;                     //节点类型
   //类型
   union {                                
      StmtKind stmt;                      //语句类型
      ExpKind exp;                        //表达式类型
   } kind;
   //属性
   union {                                
      TokenType op;                       //操作符
      int val;                            //值
      char * name;                        //字段名
   } attr;
   //表达式类型
   ExpType type;                          //表达式类型
} TreeNode;

/*跟踪标志: 在语法分析的过程中打印行号*/
extern int EchoSource;


//打印被scanner识别的串
extern int TraceScan;


//线性化打印与法树
extern int TraceParse;

//打印符号表插入和查找
extern int TraceAnalyze;

//输出注释
extern int TraceCode;

//防止发生错误时编译通过
extern int Error; 
#endif
