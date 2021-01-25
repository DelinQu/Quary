#include "global.h"
const int maxnn=20;

vector<Node> stateGraph[maxnn];      //状态图
vector<Node> minDFAGraph[maxnn];     //最小化DFA状态图
vector<set<int>> stateSet;           //状态集合
int stateList[maxnn][maxnn]={0};     //状态转换表
char alphaList[maxnn];               //转换字符列表
bool isEnd[maxnn]={0};               //是否为终点

int graphSize=maxnn;                 //图大小
int minSize=maxnn;                   //最小化DFA大小
int alphaSize=maxnn;                 //字符集大小


void buildGraph(string filename){   //构建NFA状态图
    string info;
    ifstream in(filename);          //读取文件
    
    //读入图状态个数
    in>>info;
    cout<<info<<endl;
    in>>graphSize;

    //读入图中终止状态个数和终止状态
    in>>info;
    cout<<info<<endl;
    int m,id;
    in>>m;
    
    for(int i=0;i<m;i++){
        in>>id;
        isEnd[id]=true;
    }
    //读入图中字符集大小和字符
    in>>info;
    cout<<info<<endl;
    in>>alphaSize;
    
    for(int i=0;i<alphaSize;i++){
        in>>alphaList[i];
    }    

    //读入转换条件个数和转换条件
    in>>info;
    cout<<info<<endl;
    int num,from,to;
    char s;
    in>>num;
    for(int i=0;i<num;i++){
        in>>from>>s>>to;    
        stateGraph[from].push_back(Node(to,s));
    }
    // printStateGraph();
    in>>info;
    cout<<info<<endl;
}


/*打印图*/
void printStateGraph(int size,vector<Node>graph[]){
    cout<<"----------当前状态图----------"<<endl;
    cout<<"始末状态列表:"<<endl;
    for(int i=0;i<size;i++){
        cout<<i<<":"<<isEnd[i]<<endl;
    }

    cout<<"状态图:"<<endl;
    for(int i=0;i<size;i++){
        for(int j=0;j<graph[i].size();j++){
                cout<<i<<':'<<graph[i][j].alpha<<' '<<graph[i][j].id<<endl;
            }
    }
    cout<<"------------------------------"<<endl;
}

/*打印一个状态集合*/
void printSet(set<int>s){
    for(set<int>::iterator it=s.begin();it!=s.end();it++){
        cout<<*it<<' ';
    }
    cout<<endl;
}