
#include "globals.h"
#include "symtab.h"
#include "analyze.h"

/* counter for variable memory locations */
// 可变内存位置计数器
static int location = 0;

// 删除一个语法树节点
static void deleteProc(TreeNode *t) {
  if (t==NULL) return;
  else {
    if (t->scope < depth) {
      st_delete(t->scope);
      depth = t->scope;
    }
    return;
  }
}

/* Procedure traverse is a generic recursive 
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc 
 * in postorder to tree pointed to by t
 */

/*
traverse遍历被buildSymtab和typeCheck调用
它接受两个作为参数的过程(和语法树)，一个完成每个节点的前序处理，一个进行后序处理
如果传递的是一个preProc前序过程
如果传递的是一个preProc后序过程

*/
static void traverse( TreeNode * t,void (* preProc) (TreeNode *),void (* postProc) (TreeNode *) )
{
  if (t != NULL)
    { 
      if (t->scope > depth) depth = t->scope;		//记录最深的节点
      if (t->nodekind == StmtK) {					//对于一个语句节点，我们要判断这些节点类型是不是内置的关键字，对于，IfK，WhileK，CompoundK，他们的localtion设置为0
		if (t->kind.stmt == IfK || t->kind.stmt == WhileK || t->kind.stmt == CompoundK)
		location = 0;
      }
      preProc(t);									//前序过程
      { 
		int i;
		for (i=0; i < MAXCHILDREN; i++) {			//遍历语法树的子节点
			if (t->child[i] == NULL) continue;		//跳过NULL
			//如果是语法树的比较类型的语句节点，那么scope+1
			if ( t->nodekind == StmtK && t->kind.stmt == CompoundK) {
				t->child[i]->scope = t->scope + 1;
			}
			// 如果是DeclK和funK类型，更新scope和返回类型type
			else if(t->nodekind == DeclK && t->kind.decl == funK){
				t->child[1]->scope = t->scope+1;
				return_type = t->child[0]->type;
			}
			else {									//否则不存在嵌套关系，直接赋值scope						
				t->child[i]->scope = t->scope;
			}
			traverse(t->child[i],preProc,postProc);	//前序遍历
		}
      }
      deleteProc(t);								//删除t语法节点
      postProc(t);									//后序过程
      if (t->sibling != NULL)						//兄弟节点不为空，则scop属性来自兄弟节点	 
	  	t->sibling->scope = t->scope;
      traverse(t->sibling, preProc, postProc);		//后序遍历
    }
}

/* Procedure insertNode inserts 
 * identifiers stored in t into 
 * the symbol table 
 */
/*
通过参数 (指向语法树节点的指针)接受的语法树节点的种类，确定何时把一个标识符 (与行号和地址一起 )插入到符号表中。
1.对于语句节点：
包含变量引用的节点是赋值节点和读节点，被赋值或读出的变量名
包含在节点的attr.name字段中。

2.对表达式节点：
关注标识符节点，名字也存储在attr.name中。
(1) 如果还没有看见变量insertNode过程调用st_insert (t->attr.name, t->lineno, location++)，与行号一起存储和增加地址计数器
(2) 如果变量已经在符号表中，则st_insert(t->attr.name,t->lineno,0);(存储行号但没有地址)
*/
static void insertNode( TreeNode * t)
{ 
	switch (t->nodekind)						//检查不同的类型			
    {
    case ExpK:									//表达式节点		
    switch (t->kind.exp)						
	{ case IdK:									//检查IdK
	    if (st_lookup(t->attr.name) == -1)		
	      printf("Id wasn't declared.\n");		//未定义异常
	    else									//将它插入符号表	
          //还不在表中，因此视为新定义
	      st_insert(t, 0, 0);
	    break;
	default:
	  break;
	}
      break;
    case DeclK:									//数据类型
      if (t->array_size >= 0) { // if variable is not void
		if (st_advanced_lookup(t->attr.name, t->scope) == -1) {
			//还不在表中，因此视为新定义
			st_insert(t, location++, 1);
		} else {
		printf("Declation Error %s\n",t->attr.name); location--;
		}
    }
      break;
    default:
      break;
    }
}

//类型错误异常，在类型错误的过程中将会被用到

static void typeError(TreeNode * t, char * message)
{ fprintf(listing,"Type error at line %d: %s\n",t->lineno,message);
  Error = TRUE;
}

/* Procedure checkNode performs
 * type checking at a single tree node
*/
// 过程checkNode在单个树节点上执行类型检查
//完成两个任务，首先，基于子节点的类型，它必须确定是否出现了类型错误。
//其次，它必须为当前节点推断一个类型(如果它有一个类型)并且在树节点中为这个类型分配一个新的字段
static void checkNode(TreeNode * t)
{
  BucketList l,r;
  int i,j;
  TreeNode * s;
  TreeNode * p;

  switch (t->nodekind)			
    {
    case DeclK:												//数据类型
      switch (t->kind.decl){
      case varK:											//变量节点
	if(t->child[0] != NULL){
	  if(t->child[0]->type == Integer){						//Integer
	    t->type = t->child[0]->type;						//分配一个新的字段，确定类型
	  }
	  else{													//检查错误
	    typeError(t,"Variable should not be void type");
	  }
	}
	break;
      case funK:											//同上，函数类型
	if(strcmp(t->attr.name, "main")==0){
	  if(t->sibling != NULL){								//没有兄弟节点，对于函数类型节点，错误
	    typeError(t,"main is not the last function");
	  }
	  else{						
	    if(t->child[1]->type != Void){						//void
	      typeError(t,"main has parameter");
	    }
	    if(t->child[0]->type != Void){						//void	
	      typeError(t,"main is not void type");
	    }
	  }
	}
	i=0;
	if(t->child[1]->type != Void){							//如果字节点不为void，扫描参数列表
	  s = t->child[1];
	  while(s != NULL){
	    if(i > 0 && s->type == Void){	
	      typeError(s,"Void parameter is allowed when it is the first parameter");
	    }
	    i++;												//记录列表长度
	    s = s->sibling;										//迭代
	  }
	}
	break;
      case paramK:											//检查参数		
	if(t->array_size != -1){
	  t->type = t->child[0]->type;							
	}
	break;
      }
      break;
    case ExpK:												//表达式节点
      switch (t->kind.exp)
	{
	case IdK:												//Id类型，查表
	  l = st_type_lookup (t->attr.name);
	  if (l == NULL) break;
	  if ( l->tnode_p->array_size > 0 ) { 					// should be array
		// 注意这个地方不能直接比较array_size == 0, 因为t有可能是一个数组指针，NULL == 0
	    if (t->array_size > 0) {
			//满足数组定义，arr[const]
	      if(t->child[0]->nodekind == ExpK && t->child[0]->kind.exp == ConstK){
				if(t->child[0]->attr.val < 0){				//如果val < 0说明出现了错误
					typeError(t,"Negative Subscript Error");
				}
	    	}
	      else if(t->child[0]->type != Integer){			//如果index部位Integer类型，说明出现错误
			typeError(t,"Array Index Type Error");
	      }
	    }
	  } 
	  else if (l->tnode_p->array_size == 0)					//数组大小被定义为0
	  { // should be var
	    if (t->array_size > 0) {
	      typeError(t,"Wrong type!");
	    }
	  }
	  break;
	case CalcK:												//计算类型，要求0,2两个子节点为Integer
	  if ((t->child[0]->type != Integer) ||
	      (t->child[2]->type != Integer)){
	    typeError(t,"Op applied to non-integer");
	  }
	  else{
	    t->type = Integer;									//分配一个新的字段，确定类型
	  }
	  break;
	}
      break;
    case StmtK:												//语句类型
      switch (t->kind.stmt)
	{
	case IfK:												//If语句，要求child0的属性为0,并且数值为1
	  if(t->child[0]->attr.val != 0 && t->child[0]->attr.val != 1){
	    typeError(t->child[0],"if test is not Boolean");	   
	  }
	  break;
	case AssignK:											//赋值语句X = Y
	  l = r = NULL;
	  l = st_type_lookup(t->child[0]->attr.name);			//检查字节点是否在符号表中
	  if(t->child[1]->kind.exp == IdK){
	    r = st_type_lookup(t->child[1]->attr.name);			//检查字节点是否在符号表中
	  }
	  if(l == NULL){										//如果l为NULL那么左操作数缺失
	    typeError(t->child[0], "invalid assignment : left operand error");
	  } else {										
	    if (l->tnode_p->type != Integer) {					//如果不是整型，类型异常
	      typeError(t->child[0],"not integer");
	    } else if (l->tnode_p->array_size > 0 && t->child[0]->array_size == 0) {
			// 如果检测到左操作数是一个数组，但是
	      typeError(t,"L is array but using without []");
	    }
	  }
	  if(r == NULL){//expr									//右侧操作数为NULL，说明不是一个简单的操作数，而可能是一个表达式
	    if(t->child[1]->type != Integer){					//右操作数缺失	
	      typeError(t->child[1],"invalid assignment : right operand error");
	    }
	  } else {
	    if (r->tnode_p->type != Integer) {
	      typeError(t->child[1],"not integer");
	    }
	    if(t->child[1]->nodekind == ExpK && t->child[1]->kind.exp == IdK){//var = var
	      if(r->tnode_p->array_size > 0 && t->child[1]->array_size == 0){	//如果是简单赋值，但是没有[]
		typeError(t,"R is array but using without []");		//抛出异常
	      }
	    }
	    t->type = Integer;
	  }
	  break;
	case WhileK:											//while状态
	  if (t->child[0]->type != Integer){					//如果字节点不是Integer类型，抛出异常
	    typeError(t->child[1],"expression should be integer type");
	  }
	  else{
	    t->type = Integer;
	  }
	  break;
	case ReturnK:											//返回类型
	  /* l = st_type_lookup(t->attr.name); */
	  if(t->child[0] == NULL){//void return					//返回为空，但是类型不是Void
	    if (return_type != Void)							
	      typeError(t,"Function has no return, but the function is not void type");
	  }
	  else{//data return									//返回类型为Void，但是返回val	
	    if (return_type == Void)		
	      typeError(t,"Function has return value, but the function is void type");
	    if(t->child[0]->kind.stmt == CallK) {				//如果字节点为CalK类型的，我们要查找符号表
	      l = st_type_lookup(t->child[0]->attr.name);		//查找符号表
	      if(l != NULL && l->tnode_p->type != Integer){		//如果call函数不存在或者返回类型不是Integer，出现错误
	    	typeError(t,"return type error");				//返回类型错误
	      }
	    } else if (t->child[0]->kind.exp == IdK) {			//如果为Id类型
	      l = st_type_lookup(t->child[0]->attr.name);		//查找符号表，Id是否存在
	      if (l->tnode_p->array_size > 0 && t->child[0]->array_size == 0) {	//如果前驱节点arrsize 大于0并且数组的大小==0，返回类型异常
	    	typeError(t,"return type error");
	      } // case : return array
	    } else{												
	      if(t->child[0]->type == Void){					//子节点为Void，直接返回异常
	    	typeError(t->child[0],"return type error");
	      }
	    }
	    t->type = Integer;									
	  }
	  break;
	case CallK:												//函数调用
	  l = st_type_lookup(t->attr.name);						//查找调用函数名
	  if(l == NULL){
	    typeError(t,"unknown function name");				//未找到
	  }
	  else{													
	    t->type = l->tnode_p->type;							//	函数名未查找到
	    if(l->tnode_p->paramnum == -1){						//is not function name
	      typeError(t,"is not function name");
	    }
	    else{
	      i=0;
	      if(t->child[0] == NULL){//no argument				//如果child0 ==NULL说明没有参数
		if(l->tnode_p->paramnum != 0){						//如果
		  typeError(t,"arguments not match");
		}
	      }
	      else{//some argument								// 有参数	
			s = t->child[0];
			while(s != NULL){								// 扫描得到参数个数
				i++;
				s = s->sibling;
			}
			if(l->tnode_p->paramnum == i){					// 如果参数数量匹配
				s = t->child[0];							// 子节点
				p = l->tnode_p->child[1];					// 后续节点

				while(s != NULL && p != NULL){				
					if(s->type != p->type){					// 如果类型不匹配
						typeError(s,"argument type is not matched");	
					}
					// 如果是表达式节点，那么我们需要查找符号表
					if (s->nodekind == ExpK && s->kind.exp == IdK) {

						BucketList tmp = st_type_lookup(s->attr.name);
						if (p->array_size == 0) { 			// 先看arr_size, 如果为0,那么是一个var
							if (tmp->tnode_p->array_size > 0 && s->array_size == 0) {
								// 但是p是一个arr，不匹配
								typeError(s,"argument type is not matched(array to var)");
							}
						} else if (p->array_size > 0) { 	// p->arr_size >0 说明是一个数组指针
						//should be array pointer
						if ( tmp->tnode_p->array_size == 0 ||(tmp->tnode_p->array_size > 0 && s->array_size > 0))
							// 但是p是一个var
							typeError(s,"argument type is not matched(var to array)");
						}
					}
					s = s->sibling;
					p = p->sibling;
				}
			} else{//number of arguments not match to number of parameters
				typeError(t,"arguments not match2");
			}
	      }
	    }
	  }
	  break;
	default:
	  break;
	}
      break;
    default:
      break;

    }
}


/* Function buildSymtab constructs the symbol 
 * table by preorder traversal of the syntax tree
 */
// 构建符号表
void buildSymtab(TreeNode * syntaxTree)
{ 
  syntaxTree->scope = 0;
  fprintf(listing,"Scope  Variable Name Location Type isArr ArrSize isFunc isParam Line Numbers\n");
  fprintf(listing,"-----  ------------- -------- ---- ----- ------- ------ ------- ------------\n");
  traverse(syntaxTree,insertNode,checkNode);
  if (TraceAnalyze)
    { //fprintf(listing,"\nSymbol table:\n\n");
      //printSymTab(listing);
      st_delete(-1);
    }
}
