/*工具类*/
#include "globals.h"
#include "util.h"



//打印字符串
void printToken( TokenType token, const char* tokenString )
{ switch (token)
  { 
    case IF:
    case ELSE:
    case INT:
    case CHAR:
    case RETURN:
    case VOID:
    case WHILE:
    case FOREACH:
    case AND:
    case OR:
    case NOT:
    case INPUT:
    case PRINT:
    case IN:
    case END:
      fprintf(listing,
         "reserved word: %s\n",tokenString);
      break;
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

    case NUM:
      fprintf(listing,
          "NUM, val= %s\n",tokenString);
      break;
    case ID:
      fprintf(listing,
          "ID, name= %s\n",tokenString);
      break;

    case ERROR:
      fprintf(listing,
          "ERROR: %s\n",tokenString);
      break;
    
    default: /* should never happen */
      fprintf(listing,"Unknown token: %d\n",token);
  }
}

//为语法树创建一个新的语句类型节点
TreeNode * newStmtNode(StmtKind kind)
{ 
  TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
  }
  return t;
}


//为语法树创建一个新的表达式类型节点
TreeNode * newExpNode(ExpKind kind)
{ 
  TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
    t->type = Void;
  }
  return t;
}

//复制字符串
char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else strcpy(t,s);
  return t;
}

//被printTree用来存储当前要缩进的空格数
static indentno = 0;

//用于增加或减少缩进
#define INDENT indentno+=2
#define UNINDENT indentno-=2

//通过打印空格来打印空格缩进
static void printSpaces(void)
{ 
  int i;
  for (i=0;i<indentno;i++)
    fprintf(listing,"  ");
}

//打印语法树
//深度优先
void printTree( TreeNode * tree )
{ int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
    if (tree->nodekind==StmtK)            //如果是语句类型
    { 
      switch (tree->kind.stmt) {
        case IfK:
          fprintf(listing,"If\n");
          break;
        case WhileK:
          fprintf(listing,"While\n");
          break;
        case ForEach:
          fprintf(listing,"ForEach\n");
          break;
        case AssignK:
          fprintf(listing,"Assign to: %s\n",tree->attr.name);
          break;
        case InputK:
          fprintf(listing,"Input: %s\n",tree->attr.name);
          break;
        case PrintK:
          fprintf(listing,"Print\n");
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==ExpK)        //如果是表达式类型
    { 
      switch (tree->kind.exp) {
        case OpK:
          fprintf(listing,"Op: ");
          printToken(tree->attr.op,"\0");
          break;
        case ConstK:
          fprintf(listing,"Const: %d\n",tree->attr.val);
          break;
        case IdK:
          fprintf(listing,"Id: %s\n",tree->attr.name);
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else fprintf(listing,"Unknown node kind\n");

    for (i=0;i<MAXCHILDREN;i++)       //兄弟节点
         printTree(tree->child[i]);
    
    tree = tree->sibling;
  }
  UNINDENT;
}
