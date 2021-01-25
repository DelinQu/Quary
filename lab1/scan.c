/****************************************************/
/* File: scan.c                                     */
/* updated  by qdl in 2020/11/06                    */
/****************************************************/

#include "globals.h"
#include "util.h"
#include "scan.h"

/* DFA状态集合 10*/
typedef enum{ 
  START,INID,INNUM,
  INGT,INLS,INEQ,INNE,
  INASSIGN,INCOMMENT,
  DONE
}StateType;

/* 识别标识符串 */
char tokenString[MAXTOKENLEN+1];

/* 源码输入文件大小 */
#define BUFLEN 256

static char lineBuf[BUFLEN];                          // 缓冲，保存当前行
static int linepos = 0;                               // 当前行字母的下标
static int bufsize = 0;                               // 当前缓冲的大小
static int EOF_flag = FALSE;                          // 错误标识

/*获取当前行的下一个非空字符，如果当前行已经结束，则读取下一行*/
static int getNextChar(void)
{ 
  if (!(linepos < bufsize))
  { 
    lineno++;
    if (fgets(lineBuf,BUFLEN-1,source))
    { 
      if (EchoSource) fprintf(listing,"%4d: %s",lineno,lineBuf);
      bufsize = strlen(lineBuf);
      linepos = 0;
      return lineBuf[linepos++];
    }
    else
    { EOF_flag = TRUE;
      return EOF;
    }
  }
  else return lineBuf[linepos++];
}

/* 回退1字符 */
static void ungetNextChar(void)
{ 
  if (!EOF_flag) linepos--;
}

/* 保留字查找表 */
static struct{ 
  char* str;
  TokenType tok;
} reservedWords[MAXRESERVED]= {
  {"if",IF},
  {"else",ELSE},
  {"int",INT},
  {"char",CHAR},
  {"return",RETURN},
  {"void",VOID},
  {"while",WHILE},
  {"forEach",FOREACH},
  {"and",AND},
  {"or",OR},
  {"not",NOT},
  {"input",INPUT},
  {"print",PRINT},
  {"in",IN}
};


/* 查找保留字表中的保留字，线性搜索*/
static TokenType reservedLookup (char * s)
{ 
  int i;
  for (i=0;i<MAXRESERVED;i++)
    if (!strcmp(s,reservedWords[i].str))  //返回保留字
      return reservedWords[i].tok;      
  return ID;                              //返回ID枚举类型
}


/*词法识别器，返回表示符*/
TokenType getToken(void)                                  //返回标识符
{  
  int tokenStringIndex = 0;                               //标识符数组下标
  TokenType currentToken;                                 //当前标识符
  StateType state = START;                                //状态
  int save;                                               //是否记录当前标识符
  while (state != DONE)                                   //当为非终结态的时候  
  { 
    int c = getNextChar();
    save = TRUE;
    switch (state)                                         //分别讨论10个状态
    { 
      /*1.START开始状态*/
      case START:                                         //1.开始状态
        if ((c == ' ') || (c == '\t') || (c == '\n'))     //跳过无效字符
          save = FALSE;
        else if (isdigit(c))                              //数字
          state = INNUM;
        else if (isalpha(c))                              //标识符
          state = INID;
        else if (c=='>')                                  //大于
          state=INGT;     
        else if (c=='<')                                  //小于
          state=INLS;
        else if (c=='!')                                  //不等于                 
          state=INNE;   
        else if (c == '=')                                //赋值  
          state = INASSIGN; 
        else if (c == '#')                                //注释
        {
          save=FALSE;
          state = INCOMMENT;                               
        }
        else                                              //其他特殊字符+ - * / , ; () [] {}  
        { 
          state = DONE;                                    /*直接到达DONE*/
          switch (c)
          { 
            case EOF:                                      //文件结束
              save = FALSE;                          
              currentToken = ENDFILE;
              break;
            case '+':
              currentToken = ADD;                          //加法
              break;
            case '-':
              currentToken = SUB;                          //减法结束
              break;
            case '*':
              currentToken = MUL;                          //乘法结束        
              break;
            case '/':
              currentToken = DIV;                          //除法结束
              break;
            case ';':
              currentToken = SEMI;                         //分号
              break;
            case ',':                                      //逗号
              currentToken = COMMA;                         
              break;             
            case '(':                                      //左括号
              currentToken = LPAREN;
              break;
            case ')':                                      //右括号  
              currentToken = RPAREN;
              break;
            case '[':                                      //左方
              currentToken = LFPAREN;
              break;
            case ']':                                      //右方  
              currentToken = RFPAREN;
              break;
            case '{':                                      //左花
              currentToken = LDPAREN;
              break;
            case '}':                                      //右花  
              currentToken = RDPAREN;
              break;
            case ':':                                      //右花  
              currentToken = COLON;
              break;
            default:
              currentToken = ERROR;                        //异常 
              break;
          }
        }
        break;

      /*2.注释状态INCOMMENT*/
      case INCOMMENT:                                     //注释状态
        save = FALSE;                                     //不保存注释内容      
        if (c == EOF)                                     //文件末                              
        { 
          state = DONE;
          currentToken = ENDFILE;
        }
        else if (c == '#') state = START;                 //重新回到START
        break;

      /*3.数字状态*/
      case INNUM:                                         //数字状态                                 
        if (!isdigit(c))                                  //如果继续读取的是一个others，回退                                 
        { /* backup in the input */
          ungetNextChar();                                //回退
          save = FALSE;                                   //不保存保留字
          state = DONE;                                   //结束
          currentToken = NUM;                             //当前标识符为NUM    
        }
        break;

      /*4.标识符状态*/
      case INID:                                           //字母状态
        if (!isalpha(c))
        {             
          ungetNextChar();                                //回退  
          save = FALSE; 
          state = DONE;
          currentToken = ID;                              //当前标识符为ID
        }
        break;

      /*5.大于状态*/
      case INGT:                                          //大于状态
        state = DONE;
        if (c == '=')                                     //读取到第二个=说明是一个>=号
          currentToken =  GE;                             //读取到GE
        else
        {                                                 //others，回退
          ungetNextChar();                           
          save = FALSE;   
          currentToken = GT;
        }
        break;

      /*6.小于状态*/
      case INLS:                                          //小于状态
        state = DONE;
        if (c == '=')                                     //读取到第二个=说明是一个<=号
          currentToken =  LE;                             //读取到LE
        else
        {                                                 //others，回退
          ungetNextChar();                           
          save = FALSE;   
          currentToken = LS;
        }
        break;

      /*7.不相等状态*/
      case INNE:                                          //不相等状态
        state = DONE;
        if (c == '=')                                     //读取到第二个等于号，说明是一个!=  
          currentToken = NE;                              //标识符号为！=
        else
        {                                                 //others回退
          ungetNextChar();                           
          save = FALSE;
          currentToken = ERROR;                           //未定义!
        }
        break;

      /*8.赋值状态*/
      case INASSIGN:                                      //赋值状态
        state = DONE;
        if (c == '=')                                     //读取到第二个等于号，说明是一个==，进入INEQ状态
          currentToken = EQ;                              
        else                                    
        {                                                 //others回退
          ungetNextChar();                           
          save = FALSE;
          currentToken = ASSIGN;                          //赋值
        }
        break;

      /*9.相等状态*/
      case INEQ:                                          //相等状态
        state = DONE;                                     //直接回退
          ungetNextChar();                                
          save = FALSE;
          currentToken = EQ;                              //相等
          break;

      /*10.终结态*/
      case DONE:
        break;
      
      default:                                               
        fprintf(listing,"Scanner Bug: state= %d\n",state);//其他状态表示出现异常
        state = DONE;                                     //结束
        currentToken = ERROR;
        break;
    }

    if ((save) && (tokenStringIndex <= MAXTOKENLEN))      //如果保留标识符，并且未越界
      tokenString[tokenStringIndex++] = (char) c;         //保存当前读入的字符c
    
    if (state == DONE)                                    //如果识别出一个串
    { 
      tokenString[tokenStringIndex] = '\0';
      if (currentToken == ID)
        currentToken = reservedLookup(tokenString);       //如果标识符是ID类型，则搜索保留字表，找到保留字
    }
  }
  if (TraceScan) {                                        //一个编译选项，令牌信息，打印被scanner识别的词法单元到列表文件
    fprintf(listing,"\t%d: ",lineno);
    printToken(currentToken,tokenString);
  }

  return currentToken;                                    //返回识别结果
}

