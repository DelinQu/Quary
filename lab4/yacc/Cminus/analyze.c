/****************************************************/
/* File: analyze.c                                  */
/* Semantic analyzer implementation                 */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "analyze.h"

/* counter for variable memory locations */
static int location = 0;
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
static void traverse( TreeNode * t,
		      void (* preProc) (TreeNode *),
		      void (* postProc) (TreeNode *) )
{
  if (t != NULL)
    { 
      if (t->scope > depth) depth = t->scope;
      if (t->nodekind == StmtK) {
	if (t->kind.stmt == IfK || t->kind.stmt == WhileK || t->kind.stmt == CompoundK)
	  location = 0;
      }
      preProc(t);
      { int i;
	for (i=0; i < MAXCHILDREN; i++) {
	  if (t->child[i] == NULL) continue;
	  if ( t->nodekind == StmtK && t->kind.stmt == CompoundK) {
	    t->child[i]->scope = t->scope + 1;
	  } 
	  else if(t->nodekind == DeclK && t->kind.decl == funK){
	    t->child[1]->scope = t->scope+1;
	    return_type = t->child[0]->type;
	  }
	  else {
	    t->child[i]->scope = t->scope;
	  }
	  traverse(t->child[i],preProc,postProc);
	}
      }
      deleteProc(t);
      postProc(t);
      if (t->sibling != NULL) t->sibling->scope = t->scope;
      traverse(t->sibling, preProc, postProc);
    }
}

/* Procedure insertNode inserts 
 * identifiers stored in t into 
 * the symbol table 
 */
static void insertNode( TreeNode * t)
{ switch (t->nodekind)
    {
    case ExpK:
      switch (t->kind.exp)
	{ case IdK:
	    if (st_lookup(t->attr.name) == -1)
	      printf("Id wasn't declared.\n");
	    else
	      st_insert(t, 0, 0);
	    break;
	default:
	  break;
	}
      break;
    case DeclK:
      if (t->array_size >= 0) { // if variable is not void
	if (st_advanced_lookup(t->attr.name, t->scope) == -1) {
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

static void typeError(TreeNode * t, char * message)
{ fprintf(listing,"Type error at line %d: %s\n",t->lineno,message);
  Error = TRUE;
}

/* Procedure checkNode performs
 * type checking at a single tree node
 */
static void checkNode(TreeNode * t)
{
  BucketList l,r;
  int i,j;
  TreeNode * s;
  TreeNode * p;

  switch (t->nodekind)
    {
    case DeclK:
      switch (t->kind.decl){
      case varK:
	if(t->child[0] != NULL){
	  if(t->child[0]->type == Integer){
	    t->type = t->child[0]->type;
	  }
	  else{
	    typeError(t,"Variable should not be void type");
	  }
	}
	break;
      case funK:
	if(strcmp(t->attr.name, "main")==0){
	  if(t->sibling != NULL){
	    typeError(t,"main is not the last function");
	  }
	  else{
	    if(t->child[1]->type != Void){
	      typeError(t,"main has parameter");
	    }
	    if(t->child[0]->type != Void){
	      typeError(t,"main is not void type");
	    }
	  }
	}
	i=0;
	if(t->child[1]->type != Void){
	  s = t->child[1];
	  while(s != NULL){
	    if(i > 0 && s->type == Void){
	      typeError(s,"Void parameter is allowed when it is the first parameter");
	    }
	    i++;
	    s = s->sibling;
	  }
	}
	break;
      case paramK:
	if(t->array_size != -1){
	  t->type = t->child[0]->type;
	}
	break;
      }
      break;
    case ExpK:
      switch (t->kind.exp)
	{
	case IdK:
	  l = st_type_lookup (t->attr.name);
	  if (l == NULL) break;
	  if ( l->tnode_p->array_size > 0 ) { // should be array
	    /* can't compare 't->array_size == 0' because t can be used for array pointer */
	    if (t->array_size > 0) {
	      if(t->child[0]->nodekind == ExpK && t->child[0]->kind.exp == ConstK){
		if(t->child[0]->attr.val < 0){
		  typeError(t,"Negative Subscript Error");
		}
	      }
	      else if(t->child[0]->type != Integer){
		typeError(t,"Array Index Type Error");
	      }
	    }
	  } else if ( l->tnode_p->array_size == 0) { // should be var
	    if (t->array_size > 0) {
	      typeError(t,"Wrong type!");
	    }
	  }
	  break;
	case CalcK:
	  if ((t->child[0]->type != Integer) ||
	      (t->child[2]->type != Integer)){
	    typeError(t,"Op applied to non-integer");
	  }
	  else{
	    t->type = Integer;
	  }
	  break;
	}
      break;
    case StmtK:
      switch (t->kind.stmt)
	{
	case IfK:
	  if(t->child[0]->attr.val != 0 && t->child[0]->attr.val != 1){
	    typeError(t->child[0],"if test is not Boolean");	   
	  }
	  break;
	case AssignK:
	  l = r = NULL;
	  l = st_type_lookup(t->child[0]->attr.name);
	  if(t->child[1]->kind.exp == IdK){
	    r = st_type_lookup(t->child[1]->attr.name);
	  }
	  if(l == NULL){
	    typeError(t->child[0], "invalid assignment : left operand error");
	  } else {
	    if (l->tnode_p->type != Integer) {
	      typeError(t->child[0],"not integer");
	    } else if (l->tnode_p->array_size > 0 && t->child[0]->array_size == 0) {
	      typeError(t,"L is array but using without []");
	    }
	  }
	  if(r == NULL){//expr
	    if(t->child[1]->type != Integer){
	      typeError(t->child[1],"invalid assignment : right operand error");
	    }
	  } else {
	    if (r->tnode_p->type != Integer) {
	      typeError(t->child[1],"not integer");
	    }
	    if(t->child[1]->nodekind == ExpK && t->child[1]->kind.exp == IdK){//var = var
	      if(r->tnode_p->array_size > 0 && t->child[1]->array_size == 0){
		typeError(t,"R is array but using without []");
	      }
	    }
	    t->type = Integer;
	  }
	  break;
	case WhileK:
	  if (t->child[0]->type != Integer){
	    typeError(t->child[1],"expression should be integer type");
	  }
	  else{
	    t->type = Integer;
	  }
	  break;
	case ReturnK:
	  /* l = st_type_lookup(t->attr.name); */
	  if(t->child[0] == NULL){//void return
	    if (return_type != Void)
	      typeError(t,"Function has no return, but the function is not void type");
	  }
	  else{//data return
	    if (return_type == Void)
	      typeError(t,"Function has return value, but the function is void type");
	    if(t->child[0]->kind.stmt == CallK) {
	      l = st_type_lookup(t->child[0]->attr.name);
	      if(l != NULL && l->tnode_p->type != Integer){
	    	typeError(t,"return type error");
	      }
	    } else if (t->child[0]->kind.exp == IdK) {
	      l = st_type_lookup(t->child[0]->attr.name);
	      if (l->tnode_p->array_size > 0 && t->child[0]->array_size == 0) {
	    	typeError(t,"return type error");
	      } // case : return array
	    } else{
	      if(t->child[0]->type == Void){
	    	typeError(t->child[0],"return type error");
	      }
	    }
	    t->type = Integer;
	  }
	  break;
	case CallK:
	  l = st_type_lookup(t->attr.name);
	  if(l == NULL){
	    typeError(t,"unknown function name");
	  }
	  else{
	    t->type = l->tnode_p->type;
	    if(l->tnode_p->paramnum == -1){//is not function name
	      typeError(t,"is not function name");
	    }
	    else{
	      i=0;
	      if(t->child[0] == NULL){//no argument
		if(l->tnode_p->paramnum != 0){
		  typeError(t,"arguments not match");
		}
	      }
	      else{//some argument
		s = t->child[0];
		while(s != NULL){
		  i++;
		  s = s->sibling;
		}
		if(l->tnode_p->paramnum == i){
		  s = t->child[0];
		  p = l->tnode_p->child[1];

		  while(s != NULL && p != NULL){
		    if(s->type != p->type){
		      typeError(s,"argument type is not matched");
		    }
		    if (s->nodekind == ExpK && s->kind.exp == IdK) {
		      BucketList tmp = st_type_lookup(s->attr.name);
		      if (p->array_size == 0) { // should be var
			if (tmp->tnode_p->array_size > 0 && s->array_size == 0) {
			  typeError(s,"argument type is not matched(array to var)");
			}
		      } else if (p->array_size > 0) { // should be array pointer
			if ( tmp->tnode_p->array_size == 0 ||
			     (tmp->tnode_p->array_size > 0 && s->array_size > 0) )
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
