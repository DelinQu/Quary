#include "global.h"
#include <map>
#include<string.h>

/*
判断两个状态是否等价,返回等价寻找结果
1.如果存在两种状态同为终止或非终止状态
2.且输入所有字符，到达的状态都一样，那么就等效，可以合并 
3.优先保留有自旋的状态
*/

int findEqual(int u,int v){      
    //不同为等价状态或节点的边数不一样，不等价
    if(isEnd[u]!=isEnd[v]||stateGraph[u].size()!=stateGraph[v].size()||stateGraph[u].size()==0) 
        return -1;
    bool isEqual=false;

    for(int i=0;i<stateGraph[u].size();i++){
        int a=stateGraph[u][i].id;
        int j;
        for(j=0;j<stateGraph[v].size();j++){
            int b=stateGraph[v][j].id;
            if(a==b) break;
        }
        if(j==stateGraph[v].size()) return -1;
    }

    for(int i=0;i<stateGraph[u].size();i++)     //判断是否有自旋 
        if(stateGraph[u][i].id==u) 
            return u;
    
    return v;    
}

/*合并两个状态，u，v->u*/
void mergeState(int u,int v){
    cout<<"合并状态"<<u<<":"<<v<<endl;
    stateGraph[v].clear();
    for(int i=0;i<graphSize;i++){
        for(int j=0;j<stateGraph[i].size();j++){
            int cur=stateGraph[i][j].id;
            //指向v状态的边指向u
            if(cur==v)
                stateGraph[i][j].id=u;
        }
    }
    printStateGraph(graphSize,stateGraph);
}

//重新构建DFA，整合下标问题
void rebuildDFA(){
    int empty=0;
    map<int,int>reTable;            //重定向表
    minSize=graphSize;
    for(int i=0;i<graphSize;i++){
        if(stateGraph[i].size()==0){
            minSize--;
            empty++;
        }
        else
            reTable.insert(map<int,int>::value_type(i,i-empty));
    }
    //排除状态
    for(int i=0;i<graphSize;i++){
        if(stateGraph[i].size()!=0){
            int par=reTable[i];
            for(int j=0;j<stateGraph[i].size();j++){
                int id=stateGraph[i][j].id;
                id=reTable[id];                     //映射到新的下标
                char alpha=stateGraph[i][j].alpha;
                minDFAGraph[par].push_back(Node(id,alpha));
            }
        }
    }

    bool tempIsEnd[maxn]={0};
    
    //重构终结集合
    for(map<int,int>::iterator it=reTable.begin();it!=reTable.end();it++){
        cout<<"it: "<<it->first<<it->second<<endl;
        tempIsEnd[it->second]=isEnd[it->first];
    }
    memset(isEnd,false,sizeof(isEnd));
    for(int i=0;i<minSize;i++){
        isEnd[i]=tempIsEnd[i];
    }
}

void MinDFA(){
    //遍历所有的状态
    for(int k=0;k<graphSize-1;k++){
        for(int i=0;i<graphSize;i++){
            for(int j=0;j<graphSize;j++){
                if(i!=j){
                    //查找i，j状态是否等价
                    int u=findEqual(i,j);
                    if(u!=-1){
                        //合并两个等价状态
                        if(u==i) mergeState(i,j);
                        else mergeState(j,i);
                    }
                }
            }
        }        
    }
    // cout<<"打印DFA未重构最小化集合"<<endl;
    // printStateGraph(graphSize,stateGraph);
    //重构
    rebuildDFA();
}