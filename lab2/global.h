#include <iostream>
#include <vector>
#include <set>
#include <fstream>
using namespace std;
const int maxn=20;

struct Node{         
    int id;                             //字节点编号
    char alpha;                         //转换条件字符
    Node(int ii=0,char aa=' '):id(ii),alpha(aa){}
};

extern vector<Node> stateGraph[maxn];   //状态图
extern vector<Node> minDFAGraph[maxn];  //最小化DFA状态图
extern vector<set<int>> stateSet;       //状态集合
extern int stateList[maxn][maxn];       //状态转换表
extern char alphaList[maxn];            //转换字符列表
extern bool isEnd[maxn];                //是否为终点

extern int graphSize;                   //图大小
extern int minSize;                     //最小化DFA大小
extern int alphaSize;                   //字符集大小

void buildGraph(string filename);

/*打印图*/
void printStateGraph(int graphSize,vector<Node>stateGraph[]);

/*计算I的eclosure集合*/             
set<int> getMoveTo(set<int> I,char a);

/*计算I的eclosure集合*/             
set<int> get_eclosure(set<int> I);


/*查找状态集合*/
int findState(set<int>st);

/*NFA确定化*/
void deterNFA(int start);

/*构建确定化DFA*/
void buildDeterDFA();

/*判断两个等价状态*/
int findEqual(int u,int v);

/*合并两个状态，u，v->u*/
void mergeState(int u,int v);

/*最小化DFA*/
void MinDFA();

//重新构建DFA，整合下标问题
void rebuildDFA();

void printSet(set<int>s);