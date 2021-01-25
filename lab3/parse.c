/*
*语法分析程序
*/
#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static TokenType token;                 //当前读取的串

//用于递归调用的函数原型
static TreeNode * stmt_sequence(void);  //在循环中匹配分号，再匹配新行（statement）
static TreeNode * statement(void);      //expression-stmt | compound-stmt | selection-stmt,产生新的语句类型,判断当前开头的字符,决定不同的语法
static TreeNode * if_stmt(void);        //IF语句规则
static TreeNode * while_stmt(void);     //循环语句规则: WHILE
static TreeNode * forEach_stmt(void);   //循环语句规则: forEach
static TreeNode * assign_stmt(void);    //赋值语句规则: assign
static TreeNode * input_stmt(void);     //输入语法规则INPUT
static TreeNode * print_stmt(void);     //输出语法规则PRINT
static TreeNode * express(void);        //逻辑表达式
static TreeNode * compareExpress(void); //大小比较表达式
static TreeNode * simple_exp(void);     //加减法表达式
static TreeNode * term(void);           //乘除运算
static TreeNode * factor(void);         //因子数


// 异常打印, 设置Error=TRUE
static void syntaxError(char * message)
{ 
  fprintf(listing,"\n>>> ");
  fprintf(listing,"Syntax error at line %d: %s",lineno,message);
  Error = TRUE;
}


//匹配串
static void match(TokenType expected)
{ 
  if (token == expected)  //匹配则读取下一个字符串
    token = getToken();
  else                    //否则抛异常
  {
    syntaxError("unexpected token -> ");
    printToken(token,tokenString);
    fprintf(listing,"      ");
  }
}


//stmt_sequence->stmt_sequence;statement | statement
TreeNode * stmt_sequence(void)        //在循环中匹配分号，再匹配新行（statement）
{
  TreeNode * t = statement();
  TreeNode * p = t;
  // if(token==SEMI) match(SEMI);
  // if(token==ENDFILE) match(ENDFILE);
  // if(token==SEMI) match(SEMI);
  //当遇到文件结束, END保留字的时候单独一行
  while ((token!=ENDFILE) && (token!=END) &&(token!=ELSE))
  {
    TreeNode * q;
    //匹配当前是不是; 或 :号, 这个时候表示新的一行结束
    // if(token==SEMI) 
    match(SEMI);
    // else
    //   match(COLON);
    q = statement();  //增加兄弟节点  
    if (q!=NULL) {
      if (t==NULL) t = p = q;
      else /* now p cannot be NULL either */
      { 
        p->sibling = q;
        p = q;
      }
    }
  }
  return t;
}

/*
statement → expression-stmt |  statement
*/
//产生新的语句类型,判断当前开头的字符,决定不同的语法
TreeNode * statement(void)
{ 
  TreeNode * t = NULL;
  switch (token) {
    case IF : t = if_stmt(); break;                   //IF
    case WHILE : t = while_stmt(); break;             //WHILE
    case FOREACH: t=forEach_stmt(); break;            //FOREACH
    case ID : t = assign_stmt(); break;               //赋值
    case INPUT: t = input_stmt(); break;              //输入  
    case PRINT: t = print_stmt(); break;              //输出
    // case SEMI: match(SEMI);break;
    default : 
      syntaxError("unexpected token -> ");            //其他状态认为是错误状态
      printToken(token,tokenString);
      token = getToken();
      break;
  }
  return t;
}

//IF语句规则
TreeNode * if_stmt(void)
{ 
  TreeNode * t = newStmtNode(IfK);                      //新的语句IF节点
  match(IF);                                            //匹配IF
  if (t!=NULL) t->child[0] = express();                 //匹配表达式
  match(COLON);                                         //匹配:
  if (t!=NULL){
    t->child[1] = stmt_sequence();                      //第一个语句节点
  }
  if (token==ELSE) {                                    //else分支
    //printfT(token);
    match(ELSE);                                        //匹配else
    match(COLON);                                       //匹配:
    if (t!=NULL) t->child[2] = stmt_sequence();         //匹配第二个分支
  }
  // if(token==SEMI) match(SEMI);                          //消除开始就读取分号的sequence
  match(END);                                           //匹配结束end保留字
  return t;
}

//循环语句规则: WHILE
TreeNode * while_stmt(void)
{ 
  TreeNode * t = newStmtNode(WhileK);                   //产生一个新的语句WHILE节点
  match(WHILE);                                         //匹配WHILE
  if (t!=NULL) t->child[0] = express();                 //匹配紧跟着的条件表达式
  match(COLON);                                         //匹配:
  if (t!=NULL) t->child[1] = stmt_sequence();           //匹配执行语句
  match(END);                                           //匹配一个END
  return t;
}


//循环语句规则: forEach
TreeNode * forEach_stmt(void)
{ 
  TreeNode * t = newStmtNode(ForEach);                  //产生一个新的语句FOREACH节点
  match(FOREACH);                                       //匹配FOREACH
  if (t!=NULL) t->child[0] = express();                 //匹配紧跟着的条件表达式
  match(COLON);                                         //匹配:
  if (t!=NULL) t->child[1] = stmt_sequence();           //匹配执行语句
  match(END);                                           //匹配一个END
  return t;
}


//赋值语句规则: assign
TreeNode * assign_stmt(void)
{ 
  TreeNode * t = newStmtNode(AssignK);
  if ((t!=NULL) && (token==ID))                         //name属性
    t->attr.name = copyString(tokenString);
  match(ID);                                            //匹配一个ID
  match(ASSIGN);                                        //匹配一个=
  if (t!=NULL) t->child[0] = express();                     //匹配表达式
  return t;
}


//输入语法规则INPUT
TreeNode * input_stmt(void)
{ 
  TreeNode * t = newStmtNode(InputK);
  match(INPUT);                                         //匹配INPUT保留字
  if ((t!=NULL) && (token==ID))           
    t->attr.name = copyString(tokenString);             //添加属性    
  match(ID);
  return t;
}

//输出语法规则PRINT
TreeNode * print_stmt(void)
{ 
  TreeNode * t = newStmtNode(PrintK);           
  match(PRINT);                                         //匹配输出PRINT
  if (t!=NULL) t->child[0] = express();                 //添加子节点为表达式
  return t;
}

//表达式类型识别
//var = expression | compareExpress
//注意优先级表

//逻辑表达式
TreeNode * express(void)
{ 
  TreeNode * t = compareExpress();
  if ((token==AND)||(token==OR)||(token==NOT)){
    TreeNode * p = newExpNode(OpK);                     //新建一个表达式节点, 类型为算符表达式
    if (p!=NULL) {
      p->child[0] = t;                                  //
      p->attr.op = token;                               //操作符为token
      t = p;                                            //
    }
    match(token);                                       //匹配当前字符防止检测发生
    if (t!=NULL)                                
      t->child[1] = compareExpress();                     //子表达式
  }
  return t;
}


//大小比较表达式
//var = compareExpress | simple_exp
TreeNode * compareExpress(void)
{ 
  TreeNode * t = simple_exp();
  if ((token==LS)||(token==LE)||(token==GT)||(token==GE)||(token==EQ)||(token==NE)){
    TreeNode * p = newExpNode(OpK);                     //新建一个表达式节点, 类型为算符表达式
    if (p!=NULL) {
      p->child[0] = t;                                  //
      p->attr.op = token;                               //操作符为token
      t = p;                                            //
    }
    match(token);                                       //匹配当前字符防止检测发生
    if (t!=NULL)                                
      t->child[1] = simple_exp();                       //子表达式
  }
  return t;
}


//additive_expression relop additive_expression
//加减法表达式
TreeNode * simple_exp(void)                         //
{ 
  TreeNode * t = term();  
  while ((token==ADD)||(token==SUB))                //加减       
  { 
    TreeNode * p = newExpNode(OpK);                 //新建一个表达式节点
    if (p!=NULL) {
      p->child[0] = t;                              //左子节点为t
      p->attr.op = token;                           //操作符属性为token  
      t = p;                                        //将t指向p
      match(token);
      t->child[1] = term();                         //t的子节点为term
    }
  }
  return t;
}


//term mulop factor | factor
//乘除运算
TreeNode * term(void)
{ 
  TreeNode * t = factor();
  while ((token==MUL)||(token==DIV))                  //乘除
  { 
    TreeNode * p = newExpNode(OpK);                   //迭代表达式节点p
    if (p!=NULL) {
      p->child[0] = t;                                //表达式的第一个子节点
      p->attr.op = token;                             //操作符
      t = p;                                          //迭代逆向生成    
      match(token);                                   //继续读取下一个token
      p->child[1] = factor();                         //第二个子节点是一个factor因式
    }
  }
  return t;
}


// 因子数
// (expression) | var | call | NUM
// (expression) | NUM | ID 

TreeNode * factor(void)                             
{ 
  TreeNode * t = NULL;
  switch (token) {
    case NUM :                            //NUM类型表达式
      t = newExpNode(ConstK);
      if ((t!=NULL) && (token==NUM))
        t->attr.val = atoi(tokenString);
      match(NUM);
      break;
    case ID :                             //ID标识符类型表达式
      t = newExpNode(IdK);
      if ((t!=NULL) && (token==ID))
        t->attr.name = copyString(tokenString);
      match(ID);
      break;
    case LPAREN :                         //左括号表达式   
      match(LPAREN);
      t = express();
      match(RPAREN);                      //匹配右括号
      break;
    // case SEMI: match(SEMI);break;
    default:
      syntaxError("unexpected token -> ");
      printToken(token,tokenString);
      token = getToken();
      break;
    }
  return t;
}

//构建语法树,LL(1)
TreeNode * parse(void)
{ 
  TreeNode * t;
  token = getToken();
  t = stmt_sequence();
  if (token!=ENDFILE)
    syntaxError("Code ends before file\n");
  return t;
}

void printfT(TokenType token){
  printf("-----------------------------------------\n");
  printf("CurToken=");
 switch (token)
  { 
    case IF:fprintf(listing,"IF\n"); break;
    case ELSE:fprintf(listing,"ELSE\n"); break;
    case INT:fprintf(listing,"INT\n"); break;
    case CHAR:fprintf(listing,"CHAR\n"); break;
    case RETURN:fprintf(listing,"RETURN\n"); break;
    case VOID:fprintf(listing,"VOID\n"); break;
    case WHILE:fprintf(listing,"WHILE\n"); break;
    case FOREACH:fprintf(listing,"FOREACH\n"); break;
    case AND:fprintf(listing,"AND\n"); break;
    case OR:fprintf(listing,"OR\n"); break;
    case NOT:fprintf(listing,"NOT\n"); break;
    case INPUT:fprintf(listing,"INPUT\n"); break;
    case PRINT:fprintf(listing,"PRINT\n"); break;
    case IN:fprintf(listing,"IN\n"); break;
    case END:fprintf(listing,"END\n"); break;
    
    case ADD: fprintf(listing,"+\n"); break;
    case SUB: fprintf(listing,"-\n"); break;
    case MUL: fprintf(listing,"*\n"); break;
    case DIV: fprintf(listing,"/\n"); break;
    case LS: fprintf(listing,"<\n"); break;
    case LE: fprintf(listing,"<=\n"); break;
    case GT: fprintf(listing,">\n"); break;
    case GE: fprintf(listing,">=\n"); break;
    case EQ: fprintf(listing,"==\n"); break;
    case NE: fprintf(listing,"!=\n"); break;
    case ASSIGN: fprintf(listing,"=\n"); break;
    case SEMI: fprintf(listing,";\n"); break;
    case COMMA: fprintf(listing,",\n"); break;
    case LPAREN: fprintf(listing,"(\n"); break;
    case RPAREN: fprintf(listing,")\n"); break;
    case LFPAREN: fprintf(listing,"[\n"); break;
    case RFPAREN: fprintf(listing,"]\n"); break;
    case LDPAREN: fprintf(listing,"{\n"); break;
    case RDPAREN: fprintf(listing,"}\n"); break;
    case COLON: fprintf(listing,":\n"); break;
    case ENDFILE: fprintf(listing,"EOF\n"); break;
  }
  printf("\n");
}