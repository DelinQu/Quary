/*工具类*/

#ifndef _UTIL_H_
#define _UTIL_H_


//打印识别串
void printToken( TokenType, const char* );


//创建一个新的语句状态节点用于语法树的构建
TreeNode * newStmtNode(StmtKind);


//创建一个表达式节点用于语法树的构建
TreeNode * newExpNode(ExpKind);


// 复制字符串
char * copyString( char * );

//打印语法树
void printTree( TreeNode * );

#endif
