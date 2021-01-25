/*语义分析*/
#include "globals.h"
#include "symtab.h"
#include "analyze.h"

/* counter for variable memory locations */
/*可变内存位置计数器*/
static int location = 0;

/* Procedure traverse is a generic recursive 
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc 
 * in postorder to tree pointed to by t
 */

/*
*过程遍历是一种通用的递归方法
*语法树遍历例程：它使用在前序中的preProc和后序中的postProc进行遍历
*/
static void traverse( TreeNode * t,void (* preProc) (TreeNode *),void (* postProc) (TreeNode *) )
{ if (t != NULL)
  { 
    preProc(t);
    { int i;
      for (i=0; i < MAXCHILDREN; i++) 
        traverse(t->child[i],preProc,postProc); //先序遍历
    }
    postProc(t);                                
    traverse(t->sibling,preProc,postProc);      //后续遍历
  }
}

/* nullProc is a do-nothing procedure to 
 * generate preorder-only or postorder-only
 * traversals from traverse
 */
/*
*nullProc是一个不执行任何操作的过程
*仅生成预订单或仅生成后订单
*导线测量中的导线测量
*/
static void nullProc(TreeNode * t)
{ if (t==NULL) return;
  else return;
}


// 程序将存储在t中的标识符节点插入符号表
static void insertNode( TreeNode * t)
{ switch (t->nodekind)
  { case StmtK:
      switch (t->kind.stmt)
      { case AssignK:
        case ReadK:
          if (st_lookup(t->attr.name) == -1)
          /* not yet in table, so treat as new definition */
          /*还不在表中，因此视为新定义*/
            st_insert(t->attr.name,t->lineno,location++);
          else
          /* already in table, so ignore location, 
             add line number of use only */ 
             /*已经在表中，所以忽略位置，仅添加使用的行号*/
            st_insert(t->attr.name,t->lineno,0);
          break;
        default:
          break;
      }
      break;
    case ExpK:
      switch (t->kind.exp)
      { case IdK:
          if (st_lookup(t->attr.name) == -1)
          /* not yet in table, so treat as new definition */
          /*还不在表中，因此视为新定义*/
            st_insert(t->attr.name,t->lineno,location++);
          else
          /* already in table, so ignore location, 
             add line number of use only */ 
             /*已经在表中，所以忽略位置，仅添加使用的行号*/
            st_insert(t->attr.name,t->lineno,0);
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}


//函数buildSymtab按语法树的前序遍历构造符号表

void buildSymtab(TreeNode * syntaxTree)
{ traverse(syntaxTree,insertNode,nullProc);
  if (TraceAnalyze)
  { fprintf(listing,"\nSymbol table:\n\n");
    printSymTab(listing);
  }
}

static void typeError(TreeNode * t, char * message)
{ fprintf(listing,"Type error at line %d: %s\n",t->lineno,message);
  Error = TRUE;
}


//在单个树节点上进行类型检查
//完成两个任务，首先，基于子节点的类型，它必须确定是否出现了类型错误。
//其次，它必须为当前节点推断一个类型(如果它有一个类型)并且在树节点中为这个类型分配一个新的字段
static void checkNode(TreeNode * t)
{ switch (t->nodekind)
  { case ExpK:                                            //表达式节点
      switch (t->kind.exp)
      { case OpK:
          if ((t->child[0]->type != Integer) ||
              (t->child[1]->type != Integer))
            typeError(t,"Op applied to non-integer");     //确定是否出现了类型错误
          if ((t->attr.op == EQ) || (t->attr.op == LT))   //分配一个新的字段，确定类型
            t->type = Boolean;                            //boolean
          else
            t->type = Integer;                            //Integer
          break;
        case ConstK:                                      
        case IdK:
          t->type = Integer;                              //ConstK和IdK类型默认是Integer
          break;
        default:
          break;
      }
      break;
    case StmtK:                                           //语句节点
      switch (t->kind.stmt)
      { case IfK:                                         //确定是否出现了类型错误
          if (t->child[0]->type == Integer)
            typeError(t->child[0],"if test is not Boolean");
          break;
        case AssignK:                                     
          if (t->child[0]->type != Integer)
            typeError(t->child[0],"assignment of non-integer value");
          break;
        case WriteK:
          if (t->child[0]->type != Integer)
            typeError(t->child[0],"write of non-integer value");
          break;
        case RepeatK:
          if (t->child[1]->type == Integer)
            typeError(t->child[1],"repeat test is not Boolean");
          break;
        default:
          break;
      }
      break;
    default:
      break;

  }
}


// typeCheck通过后序语法树遍历进行类型检查
void typeCheck(TreeNode * syntaxTree)
{ 
  traverse(syntaxTree,nullProc,checkNode);
}
