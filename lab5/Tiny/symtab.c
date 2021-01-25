#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

/* Hash 表的size */
#define SIZE 211

/* SHIFT is the power of two used as multiplier
   in hash function  */
// hash函数中的移位
#define SHIFT 4

// hash函数
static int hash ( char * key )
{ int temp = 0;
  int i = 0;
  while (key[i] != '\0')
  { temp = ((temp << SHIFT) + key[i]) % SIZE;
    ++i;
  }
  return temp;
}


// 存储行序列的结构体
typedef struct LineListRec
   { int lineno;
     struct LineListRec * next;
   } * LineList;

/*
存储桶列表中的记录每个变量，包括名称，分配的内存位置
以及它出现在源代码中的行号列表
*/
typedef struct BucketListRec
   { char * name;                 //变量名
     LineList lines;              //行号
     int memloc ;                 //变量的内存位置
     struct BucketListRec * next; //下一个节点
   } * BucketList;

// hash表
static BucketList hashTable[SIZE];

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
/*
程序st_insert插入行号和符号表中的内存位置 = 仅插入位置第一次，否则忽略
*/
void st_insert( char * name, int lineno, int loc )
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL) /* variable not yet in table */
  { l = (BucketList) malloc(sizeof(struct BucketListRec));
    l->name = name;
    l->lines = (LineList) malloc(sizeof(struct LineListRec));
    l->lines->lineno = lineno;
    l->memloc = loc;
    l->lines->next = NULL;
    l->next = hashTable[h];
    hashTable[h] = l; }
  else /* found in table, so just add line number */
  { LineList t = l->lines;
    while (t->next != NULL) t = t->next;
    t->next = (LineList) malloc(sizeof(struct LineListRec));
    t->next->lineno = lineno;
    t->next->next = NULL;
  }
}

/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */
/*
函数st_lookup返回内存变量的位置，如果找不到则为-1
*/
int st_lookup ( char * name )
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL) return -1;
  else return l->memloc;
}

/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
/*
过程打印symtab打印格式化的符号表内容列表到列表文件
*/
void printSymTab(FILE * listing)
{ int i;
  fprintf(listing,"Variable Name  Location   Line Numbers\n");
  fprintf(listing,"-------------  --------   ------------\n");
  for (i=0;i<SIZE;++i)
  { if (hashTable[i] != NULL)
    { BucketList l = hashTable[i];
      while (l != NULL)
      { LineList t = l->lines;
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
}
