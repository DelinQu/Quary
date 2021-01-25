#ifndef _SYMTAB_H
#define _SYMTAB_H_

/* SIZE is the size of the hash table */
// hash表的大小
#define SIZE 211

/* SHIFT is the power of two used as multiplier
   in hash function  */
// 用于乘法计算的位移
#define SHIFT 4

/* the hash function */
// hash函数，返回key的大小*SHIFT
static int hash ( char * key )
{ int temp = 0;
  int i = 0;
  while (key[i] != '\0')
    { temp = ((temp << SHIFT) + key[i]) % SIZE;
      ++i;
    }
  return temp;
}

/* the list of line numbers of the source 
 * code in which a variable is referenced
 */
// 引用变量的源代码的行号列表
typedef struct LineListRec
{ int lineno;
  struct LineListRec * next;
} * LineList;

/* The record in the bucket lists for
 * each variable, including name, 
 * assigned memory location, and
 * the list of line numbers in which
 * it appears in the source code
 */
// 存储桶中的记录列出了每个变量，包括名称、分配的内存位置以及它在源代码中出现的行号列表
typedef struct BucketListRec
{ char * name;          //变量名
  LineList lines;       //行号
  int scope;            //非全局变量global
  TreeNode *tnode_p;    //语法树节点保存变量的信息
  int memloc ;          //分配的内存位置
  struct BucketListRec * next;
} * BucketList;

/* the hash table */
// hash表
BucketList hashTable[SIZE];


/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */

// 程序st_insert在符号表中插入行号和符号表中的内存位置
// loc=仅插入第一个位置内存位置，否则忽略
void st_insert( TreeNode *t, int loc, int addflag);

// 删除
void st_delete( int scope);

/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */
// lookup操作，从符号表中取出地址号(后面的代码产生器需要，符号表生成器也要检查是否已经看见了变量 )
// 函数st_lookup返回变量的内存位置，如果找不到，则返回-1
int st_lookup ( char * name );

int st_advanced_lookup (char *name, int scope);

BucketList st_type_lookup ( char *name );


/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
// 打印符号表
void printSymTab(FILE * listing);

#endif
