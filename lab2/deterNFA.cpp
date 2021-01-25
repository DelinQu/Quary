#include "global.h"
#include <queue>
#include<string.h>


/*计算I的eclosure集合*/             
set<int> getMoveTo(set<int> I,char a){                          //计算集合I经过a字符到达的集合，方法广度优先搜索
    set<int> newSet;                    	
    for(set<int>::iterator it=I.begin();it!=I.end();it++)       
	{
        int cur=*it;
		for(int i=0;i<stateGraph[cur].size();i++)
		{
            int id=stateGraph[cur][i].id;
            char alpha=stateGraph[cur][i].alpha;
			if(alpha==a)    
				newSet.insert(id);
		}
	}
	return newSet;
}

/*计算I的eclosure集合*/                 
set<int> get_eclosure(set<int> I){                              //计算get_eclosure(I)集，方法广度优先搜索
    //拓扑排序算法
    bool visited[graphSize];
    memset(visited,false,sizeof(visited));
	
    set<int> newSet;                    
	queue<int> Q;                                               //辅助队列
	
    for(set<int>::iterator it=I.begin();it!=I.end();it++)       //将当前有待扩展的set，压入队列，并插入新的set集合 
	{
		Q.push(*it);
		newSet.insert(*it);
	}
	
    while(!Q.empty())                                           //队列每次弹出首位，并以首位进行扩展 
	{
		int q=Q.front();
		Q.pop();                //扩展以后弹出 
		for(int i=0;i<stateGraph[q].size();i++)
		{
            int id=stateGraph[q][i].id;
            char alpha=stateGraph[q][i].alpha;
			if(alpha=='#' && visited[id]==false)    
			{
				newSet.insert(id);
				Q.push(id);
				visited[id]=true;                               //如果找到了，那么就加入新的set，并标记已访问 
			}
		}
	}
	return newSet;
}


/*查找状态集合*/
int findState(set<int>st){
    for(int i=0;i<stateSet.size();i++)
        if(stateSet[i]==st) return i;
    return -1;
}

/*NFA确定化*/
void deterNFA(int start){
    //初始化
    bool tempisEnd[maxn]={0};
    set<int>cur;
    cur.insert(start);
    cur=get_eclosure(cur);  
    stateSet.push_back(cur);    //加入初始集合
    cout<<"初始集合"<<endl;
    printSet(cur);
    //遍历状态集合，直到集合尾，期间不断更新状态集合
    for(int i=0;i<stateSet.size();i++){
        stateList[i][0]=i;                                      //当前被扩展状态
        for(int j=0;j<alphaSize;j++){
            cur=stateSet[i];                                    //当前要扩展的集合
            //求MoveTo集合
            cur=getMoveTo(cur,alphaList[j]);
            //求e_closure
            cur=get_eclosure(cur);
            cout<<"eclurse: "<<i<<endl;
            printSet(cur);
            if(cur.size()>0){
                //判断当前集合是否在状态集合中
                int where=findState(cur);
                //不存在则加入集合
                if(where==-1){
                    stateSet.push_back(cur);
                    where=stateSet.size()-1;
                }
                stateList[i][j+1]=where;
            }
            else                                                //否则表示当前的状态无法扩展出节点，我们将无法扩展出的状态标记为-1
                stateList[i][j+1]=-1;
        }
    }

    for(int i=0;i<stateSet.size();i++){
        cur=stateSet[i];
        //记录状态
        for(set<int>::iterator it=cur.begin();it!=cur.end();it++){
            if(isEnd[*it]) tempisEnd[i]=true;
        }
    }
    memset(isEnd,false,sizeof(isEnd));
    for(int i=0;i<stateSet.size();i++){
        isEnd[i]=tempisEnd[i];
    }
}


void buildDeterDFA(){                       //根据状态表构建确定化后的DFA
    for(int i=0;i<graphSize;i++)
        stateGraph[i].clear();
    graphSize=stateSet.size();              //更新图的规模

    for(int i=0;i<stateSet.size();i++){
        int parent=stateList[i][0];         //取出父节点    
        for(int j=1;j<=alphaSize;j++){
            int id=stateList[i][j];         //状态编号
            if(id!=-1){
                char alpha=alphaList[j-1];  //转换条件
                stateGraph[parent].push_back(Node(id,alpha));
            }
        }
    }
}


