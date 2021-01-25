%{
/*定义段*/
#define YYPARSER /* distinguishes Yacc output from other code files */

#include "globals.h"
#include "util.h"
#include "scan.h"

//语法树
int yyerror(char *message);
TreeNode * parse(void);

//语法树节点
#define YYSTYPE TreeNode *
static char * savedName;      /* for use in assignments 读取的位置*/
static int savedLineNo;       /* ditto 行号*/
static TreeNode * savedTree;  /* stores syntax tree for later return 保存语法树*/
int pn;
%}

//保留字
%token IF ELSE INT RETURN VOID WHILE
%token ID NUM
%token LEQ LES BEQ BIG EQ NEQ SEMI
%token SOPEN SCLOSE MOPEN MCLOSE BOPEN BCLOSE
%token ERROR ENDFILE
%left PLUS MINUS
%left MUL DIV COMMA
%right ASSIGN

%%
/*规则段*/
//仿照TINY,使用巴克斯范式(BNF)定义语法
program     : declar_list
{ savedTree = $1;}
            ;
declar_list : declar_list declar                    //定义语句, 循环中不断匹配新的兄弟节点
                 { YYSTYPE t = $1;
                   if (t != NULL)
                   { while (t->sibling != NULL)
                        t = t->sibling;
                     t->sibling = $2;
                     $$ = $1; }
                   else $$ = $2;
                 }
            | declar  { $$ = $1; }
            ;
declar      : var_declar { $$ = $1; }               //包括类型定义和函数定义
            | fun_declar { $$ = $1; }
            ;
var_declar  : type_spec ID SEMI                     //ID类型赋值, 对于每个var节点, 我们需要设置节点属性, 包括$1  name  array_size
                 { $$ = newDeclNode(varK);
                   $$->child[0] = $1;
                   $$->attr.name = copyString(st_pop());
                   $$->array_size = 0;
                 }
            | type_spec ID BOPEN NUM BCLOSE SEMI    //ID类型数组
                 { $$ = newDeclNode(varK);
                   $$->child[0] = $1;
                   $$->array_size = atoi(st_pop()); //设置数组大小为回退一个数据的整数值
                   $$->attr.name = copyString(st_pop());
                 }
            ;
type_spec   : INT                                   //int 类型定义
            { $$ = newExpNode(TypeK);
              $$->type = Integer;
            }
            | VOID                                  //void 类型定义
            { $$ = newExpNode(TypeK);
              $$->type = Void;
            }
            ;
fun_declar  : type_spec ID { savedLineNo = lineno;} //函数定义
		 SOPEN params SCLOSE compound_stmt
                 { $$ = newDeclNode(funK);
                   $$->child[0] = $1;               //函数名
                   $$->child[1] = $5;               //括号内的参数
                   $$->child[2] = $7;               //函数体
                   $$->attr.name = copyString(st_pop());
		   $$->lineno = savedLineNo;
                 }
            ;
params      : param_list { $$ = $1; }               //匹配参数
            | VOID
            { $$ = newDeclNode(paramK);
		$$->array_size = -1;
		$$->type = Void;
		$$->paramnum = 0;
            }
            ;
param_list  : param_list COMMA param                //参数列表, while 循环匹配
                 { YYSTYPE t = $1;
                   if (t != NULL)
                       { while (t->sibling != NULL)
                           t = t->sibling;
                         t->sibling = $3;
                         $$ = $1; }
                   else $$ = $3;
                 }
            | param { $$ = $1; }
            ;
param       : type_spec ID                          //参数ID
                 { $$ = newDeclNode(paramK);
                   $$->child[0] = $1;
                   $$->attr.name = copyString(st_pop());
                   $$->array_size = 0;
                 }
            | type_spec ID BOPEN BCLOSE
                 { $$ = newDeclNode(paramK);
                   $$->child[0] = $1;
                   $$->attr.name = copyString(st_pop());
		   $$->array_size = 1;
                 }
            ;
compound_stmt : MOPEN local_declar stmt_list MCLOSE //组合状态
                 { $$ = newStmtNode(CompoundK);
                   $$->child[0] = $2;
                   $$->child[1] = $3;
                 }
             ;
local_declar : local_declar var_declar              //本地变量
                 { YYSTYPE t = $1;
                   if (t != NULL)
                   { while (t->sibling != NULL)
                        t = t->sibling;
                     t->sibling = $2;
                     $$ = $1; }
                     else $$ = $2;
                 }
             | empty { $$ = $1; }
             ;
stmt_list    : stmt_list stmt                       //语句状态列表, 循环匹配
                 { YYSTYPE t = $1;
                   if (t != NULL)
                   { while (t->sibling != NULL)
                        t = t->sibling;
                     t->sibling = $2;
                     $$ = $1; }
                     else $$ = $2;
                 }
             | empty { $$ = $1; }
             ;
stmt         : expr_stmt { $$ = $1; }               //表达式状态
| compound_stmt { $$ = $1; }
| selection_stmt { $$ = $1; }
| iteration_stmt { $$ = $1; }
| return_stmt { $$ = $1; }
;
expr_stmt    : expr SEMI { $$ = $1; }               //表达式状态
             | SEMI { $$ = NULL; }
             ;
selection_stmt : IF SOPEN expr SCLOSE stmt          //选择状态
               { $$ = newStmtNode(IfK);
                 $$->child[0] = $3;
                 $$->child[1] = $5;
               }
               | IF SOPEN expr SCLOSE stmt ELSE stmt
               { $$ = newStmtNode(IfK);
                 $$->child[0] = $3;
                 $$->child[1] = $5;
                 $$->child[2] = $7;
               }
               ;
iteration_stmt : WHILE SOPEN expr SCLOSE stmt       //下标状态
                 { $$ = newStmtNode(WhileK);
                   $$->child[0] = $3;
                   $$->child[1] = $5;
                 }
               ;
return_stmt    : RETURN SEMI                        //返回状态
                 { $$ = newStmtNode(ReturnK);
		   $$->type = Void;
		 }
               | RETURN expr SEMI
                 { $$ = newStmtNode(ReturnK);
                   $$->child[0] = $2;
                 }
               ;
expr           : var ASSIGN expr                    //表达式节点
               { $$ = newStmtNode(AssignK);
                 $$->child[0] = $1;
                 $$->child[1] = $3;
               }
               | simple_expr
               { $$ = $1;
               }
               ;
var            : ID                                 //类型定义
               { $$ = newExpNode(IdK);
                 $$->attr.name = copyString(st_pop());
		 $$->array_size = 0;
		 $$->type = Integer;
               }
               | ID BOPEN expr BCLOSE
               { $$ = newExpNode(IdK);
                 $$->attr.name = copyString(st_pop());
                 $$->child[0] = $3;
		 $$->array_size = 1;
		 $$->type = Integer;
               }
               ;
simple_expr : additive_expr relop additive_expr     //简单表达式
                { $$ = newExpNode(CalcK);
                  $$->child[0] = $1;
                  $$->child[1] = $2;
                  $$->child[2] = $3;
                }
            | additive_expr { $$ = $1; }
            ;
relop       : LEQ                                   //比较表达式
                { $$ = newExpNode(OpK);
                  $$->attr.op = LEQ;
                }
            | LES
                { $$ = newExpNode(OpK);
                  $$->attr.op = LES;
                }
            | BIG
                { $$ = newExpNode(OpK);
                  $$->attr.op = BIG;
                }
            | BEQ
                { $$ = newExpNode(OpK);
                  $$->attr.op = BEQ;
                }
            | EQ
                { $$ = newExpNode(OpK);
                  $$->attr.op = EQ;
                }
            | NEQ
                { $$ = newExpNode(OpK);
                  $$->attr.op = NEQ;
                }
            ;
additive_expr : additive_expr addop term            //加法表达式
                { $$ = newExpNode(CalcK);
                  $$->child[0] = $1;
                  $$->child[1] = $2;
                  $$->child[2] = $3;
                }
              | term { $$ = $1; }
              ;
addop         : PLUS                              
                { $$ = newExpNode(OpK);
                  $$->attr.op = PLUS;
                }
              | MINUS
                { $$ = newExpNode(OpK);
                  $$->attr.op = MINUS;
                }
              ;
term          : term mulop factor                   //乘法表达式
                { $$ = newExpNode(CalcK);
                  $$->child[0] = $1;
                  $$->child[1] = $2;
                  $$->child[2] = $3;
                }
               | factor { $$ = $1; }
               ;
mulop          : MUL
                { $$ = newExpNode(OpK);
                  $$->attr.op = MUL; }
               | DIV { $$ = newExpNode(OpK);
                  $$->attr.op = DIV; }
               ;
factor    : SOPEN expr SCLOSE { $$ = $2; }          //单个项
          | var { $$ = $1; }
          | call { $$ = $1; }
          | NUM
            { $$ = newExpNode(ConstK);
	      $$->type = Integer;
              $$->attr.val = atoi(st_pop());
            };
call      : ID SOPEN args SCLOSE                    //函数调用
            { $$ = newStmtNode(CallK);
              $$->attr.name = copyString(st_pop());
              $$->child[0] = $3;
              /* $$->lineno = savedLineNo; */
            };
args       : arg_list { $$ = $1; } | empty { $$ = $1; };

arg_list : arg_list COMMA expr                      //参数列表
             { YYSTYPE t = $1;
               if (t != NULL)
                 { while (t->sibling != NULL)
                     t = t->sibling;
                   t->sibling = $3;
                   $$ = $1; }
               else $$ = $3;
             }
          | expr { $$ = $1; }
          ;
empty     : { $$ = NULL; };                         //绑定空
%%

/*用户子例程段*/
//错误处理
int yyerror(char * message)
{ fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
  fprintf(listing,"Current token: ");
  printToken(yychar,tokenString);
  Error = TRUE;
  return 0;
}

/* yylex calls getToken to make Yacc/Bison output
 * compatible with ealier versions of the TINY scanner
 */
/* static int yylex(void) */
/* { return getToken(); } */

//语法树打印
TreeNode * parse(void)
{
  yyparse();
  return savedTree;
}