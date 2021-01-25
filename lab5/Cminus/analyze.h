/****************************************************/
/* File: analyze.h                                  */
/* Semantic analyzer interface for TINY compiler    */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _ANALYZE_H_
#define _ANALYZE_H_

/* Function buildSymtab constructs the symbol 
 * table by preorder traversal of the syntax tree
 */
//函数buildSymtab通过前序遍历语法树来构造符号表
void buildSymtab(TreeNode *);

/* Procedure typeCheck performs type checking 
 * by a postorder syntax tree traversal
 */
//过程typeCheck执行类型检查通过后序语法树遍历
void typeCheck(TreeNode *);
#endif
