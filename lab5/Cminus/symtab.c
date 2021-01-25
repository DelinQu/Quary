#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "symtab.h"
#include "util.h"


/*
  符号表的操作无外乎3, insert，delete， lookup
*/
/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */


// 插入操作
/*过程：
通过参数 (指向语法树节点的指针)接受的语法树节点的种类，确定何时把一个标识符 (与行号和地址一起 )插入到符号表中。
1.对于语句节点：
包含变量引用的节点是赋值节点和读节点，被赋值或读出的变量名
包含在节点的attr.name字段中。

2.对表达式节点：
关注标识符节点，名字也存储在attr.name中。
(1) 如果还没有看见变量insertNode过程调用st_insert (t->attr.name, t->lineno, location++)，与行号一起存储和增加地址计数器
(2) 如果变量已经在符号表中，则st_insert(t->attr.name,t->lineno,0);(存储行号但没有地址)
*/
void st_insert( TreeNode *t, int loc, int addflag )                 
{ 
  int h = hash(t->attr.name);                                       //
  BucketList l =  hashTable[h];                                     //确定hash的位置
  TreeNode * s;                                                     
  int tmp;

  while ((l != NULL) && (strcmp(t->attr.name,l->name) != 0))        //找到一个空位置
    l = l->next;

  if (addflag) /* variable not yet in table */                      //如果变量还没有在符号表中
    { 
      l = (BucketList) malloc(sizeof(struct BucketListRec));        //分配，添加到符号表
      l->name = t->attr.name;                                       //name
      l->lines = (LineList) malloc(sizeof(struct LineListRec));     //分配，添加到索引列表
      l->lines->lineno = t->lineno;                                 //行号
      l->memloc = loc;                                              //内存位置
      l->scope = t->scope;                                          //非全局变量global 

      l->tnode_p = t;                                               //保存语法树节点
      l->lines->next = NULL;                                        //next指针
      l->next = hashTable[h];                                       //l节点的next指向起始位置，insert在head而不是tail

      if(t->kind.decl == funK){                                     //如果是一个funK类型，执行判断  
          if(t->child[0]->type == Void){                                    //如果是一void类型，更改类型        
            t->type = Void;
          }
          else{                                                             //否则返回值是Integer
            t->type = Integer;
          }

          //if(t->child[1]->type == Void)                                   //如果子节点的类型是void，并且size=0,则参数列表为空void(),赋值paramnum=0
          if(t->child[1]->array_size == -1){//void
            t->paramnum = 0;
          }
          else{                                                             //否则循环遍历兄弟节点  
            tmp = 0;
            s = t->child[1];
            while(s != NULL){
              tmp++;
              s = s->sibling;
            }
            t->paramnum = tmp;                                              //参数列表长度
          }
      }
      else{                                                                 //否则没有参数列表
	      t->paramnum = -1;
      }
      hashTable[h] = l;
    } 
    // found in table, so just add line number                              //我们之前讨论的是符号表中不存在的情况，现在name存在，我们只需要修改行号即可
    else 
    { LineList ll = l->lines;
      while (ll->next != NULL) ll = ll->next;                               //找到ll的尾部
      ll->next = (LineList) malloc(sizeof(struct LineListRec));             //新增一项
      ll->next->lineno = t->lineno;                                         //标记行号
      ll->next->next = NULL;
    }
} /* st_insert */


// 删除语句
void st_delete ( int scope ) {
  int i;

  for (i = 0; i < SIZE; i++) {                                              //遍历hashTable
    BucketList l = hashTable[i];                                                

    while ( (l != NULL) && (l->scope > scope) ){                            //当l不为空并且scope范围大于scop
      hashTable[i] = l->next;
      fprintf(listing,"%-5d  %-14s %-8d ",l->scope,l->name,l->memloc);      
      //if(l->tnode_p->paramnum != -1){//function
      if(l->tnode_p->kind.decl == funK){                                    //函数节点
        if(l->tnode_p->child[0]->type == Void){                             //void
          fprintf(listing,"%-5s ","void");
        }
        else{
          fprintf(listing,"%-5s ","int");                                   
        }
        fprintf(listing,"%-4s %-9d %-4s %-7s  ","no",0,"yes","no");
      }
      else if(l->tnode_p->kind.decl == paramK){                             //decl数据类型，对所有定义的枚举判断
        if(l->tnode_p->array_size > 0){//array                              //数组
          fprintf(listing,"%-5s %-4s %-9d %-4s %-7s  ","int","yes",l->tnode_p->array_size,"no","yes");
        }
        else{                                                               
          fprintf(listing,"%-5s %-4s %-9d %-4s %-7s  ","int","no",0,"no","yes");
        }
      }
      else if(l->tnode_p->array_size > 0){//array                           //数组
        fprintf(listing,"%-5s %-4s %-9d %-4s %-7s  ","int","yes",l->tnode_p->array_size,"no","no");
      }
      else{
        fprintf(listing,"%-5s %-4s %-9d %-4s %-7s  ","int","no",0,"no","no");
      }

      LineList t = l->lines;                                                //取出索引列表的索引
      while(t != NULL) {                                                    //遍历释放它
        LineList next = t->next;    
        fprintf(listing,"%4d ",t->lineno);
        free(t);
        t = next;
      }
      fprintf(listing,"\n");
      free(l);
      l = hashTable[i];                                                     //迭代l，循环  
    }
  }
  return ;
}

/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */
// 查找 函数st_lookup返回变量的内存位置，如果找不到，则返回-1，name
int st_lookup ( char * name )
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL) return -1;
  else return l->memloc;
}

/*
查找advance,除了比较name还要比较scope作用域范围
 */
int st_advanced_lookup ( char *name , int scope) {
  int h = hash(name);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0 || l->scope != scope))
    l = l->next;
  if (l == NULL) return -1;
  else return l->memloc;
}

// 查找type，存在则直接诶返回一个表项结构体BucketListRec
BucketList st_type_lookup ( char *name ){
  int h = hash(name);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;

  if (l == NULL){
    return NULL;
  }
  else{
    return l;
  }
}

/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
//打印符号表
void printSymTab(FILE * listing)
{ int i;
  fprintf(listing,"Scope  Variable Name  Location  Type isArr ArrSize isFunc  Line Numbers\n");
  fprintf(listing,"-----  -------------  --------  ---- ----- ------- ------  ------------\n");
  for (i=0;i<SIZE;++i)
  { 
    if (hashTable[i] != NULL)
    { BucketList l = hashTable[i];
      while (l != NULL)
        { LineList t = l->lines;
          fprintf(listing,"%-5d  ",l->scope);
          fprintf(listing,"%-14s ",l->name);
          fprintf(listing,"%-8d  ",l->memloc);
          while (t != NULL)
          { fprintf(listing,"%4d ",t->lineno);
            t = t->next;
          }
          fprintf(listing,"\n");
          l = l->next;
        }
    }
  }
} /* printSymTab */
