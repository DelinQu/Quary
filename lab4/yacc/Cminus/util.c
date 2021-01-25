#include "globals.h"
#include "util.h"

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
void printToken( TokenType token, const char* tokenString )
{
  switch (token) {
    case IF:
      fprintf(listing, "  %s\n", "IF");
      break;
    case ELSE:
      fprintf(listing, "  %s\n", "ELSE");
      break;
    case INT:
      fprintf(listing,"  %s\n", "INT");
      break;
    case RETURN:
      fprintf(listing, "  %s\n", "RETURN");
      break;
    case VOID:
      fprintf(listing, "  %s\n", "VOID");
      break;
    case WHILE:
      fprintf(listing, "  %s\n", "WHILE");
      break;
    case ID:
      fprintf(listing, "  %s %s\n", "ID", tokenString);
      break;
    case NUM:
      fprintf(listing, "  %s %s\n", "NUM", tokenString);
      break;
    case PLUS:
      fprintf(listing, "  %s\n", "+");
      break;
    case MINUS:
      fprintf(listing, "  %s\n", "-");
      break;
    case MUL:
      fprintf(listing, "  %s\n", "*");
      break;
    case DIV:
      fprintf(listing, "  %s\n", "/");
      break;
    case LEQ:
      fprintf(listing, "  %s\n", "<=");
      break;
    case LES:
      fprintf(listing, "  %s\n", "<");
      break;
    case BEQ:
      fprintf(listing, "  %s\n", ">=");
      break;
    case BIG:
      fprintf(listing, "  %s\n", ">");
      break;
    case EQ:
      fprintf(listing, "  %s\n", "==");
      break;
    case NEQ:
      fprintf(listing, "  %s\n", "!=");
      break;
    case ASSIGN:
      fprintf(listing, "  %s\n", "=");
      break;
    case SEMI:
      fprintf(listing, "  %s\n", ";");
      break;
    case COMMA:
      fprintf(listing, "  %s\n", ",");
      break;
    case SOPEN:
      fprintf(listing, "  %s\n", "(");
      break;
    case SCLOSE:
      fprintf(listing, "  %s\n", ")");
      break;
    case MOPEN:
      fprintf(listing, "  %s\n", "{");
      break;
    case MCLOSE:
      fprintf(listing, "  %s\n", "{");
      break;
    case BOPEN:
      fprintf(listing, "  %s\n", "[");
      break;
    case BCLOSE:
      fprintf(listing, "  %s\n", "]");
      break;
    case ERROR:
      fprintf(listing, "  %s %s\n", "ERROR", "Comment Error");
      break;
    case ENDFILE:
      fprintf(listing, "  %s\n", "ENDFILE", "EOF");
      break;
    default: /* should never happen */
      fprintf(listing,"Unknown token: %d\n",token);
      break;
  }
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
		t->paramnum = -1;
  }
  return t;
}

/* Function newExpNode creates a new expression 
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
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
		t->paramnum = -1;
  }
  return t;
}

/* Function newDeclNode creates a new declaration
 * node for syntax tree construction
 */
TreeNode * newDeclNode(DeclKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = DeclK;
    t->kind.decl = kind;
    t->lineno = lineno;
		t->paramnum = -1;
  }
  return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
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

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{ int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}

/* procedure printTree prints a syntax tree to the 
 * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode * tree )
{ int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
    if (tree->nodekind==StmtK)
    { switch (tree->kind.stmt) {
      case IfK:
        fprintf(listing,"If\n");
        break;
      case WhileK:
        fprintf(listing, "While\n");
        break;
      case AssignK:
        fprintf(listing, "Assign\n");
        break;
      case CompoundK:
        fprintf(listing, "Compound statement\n");
        break;
      case ReturnK:
        fprintf(listing, "Return\n");
        break;
      case CallK:
        fprintf(listing, "Call Function : %s\n", tree->attr.name);
        break;
      default:
        fprintf(listing,"Unknown ExpNode kind\n");
        break;
      }
    }
    else if (tree->nodekind==ExpK)
    { switch (tree->kind.exp) {
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
      case TypeK:
        if (tree->type == Integer)
          fprintf(listing,"Integer Type\n");
        else
          fprintf(listing,"Void Type\n");
        break;
      case CalcK:
        fprintf(listing, "Expression\n");
        break;
      default:
        fprintf(listing,"Unknown ExpNode kind\n");
        break;
      }
    }
    else if (tree->nodekind==DeclK)
      { switch (tree->kind.decl) {
        case varK :
          if (tree->array_size)
            fprintf(listing, "A Variable Declared: %s[%d]\n", tree->attr.name, tree->array_size);
          else {
            fprintf(listing, "A Variable Declared: %s\n", tree->attr.name);
          }
          break;
        case funK :
          fprintf(listing, "Function Declared: %s\n", tree->attr.name);
          break;
        case paramK :
          if (tree->array_size == -1)
            fprintf(listing, "Void Parameter\n");
          else if (tree->array_size == 0)
            fprintf(listing, "Parameter : %s\n", tree->attr.name);
          else
            fprintf(listing, "Parameter : %s[]\n", tree->attr.name);
          break;
        default:
          fprintf(listing, "Unknown Declaration\n");
          break;
        }
      }
    else fprintf(listing,"Unknown node kind\n");
    for (i=0;i<MAXCHILDREN;i++)
         printTree(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
}

void st_push(char *str) {
  strcpy(stack[++top], str);
}

char *st_pop() {
  return stack[top--];
}
