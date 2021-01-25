#include "global.h"
using namespace std;

int main(int argc, char * argv[]){
    if(argc!=2) return 0;

    //构图
    buildGraph(argv[1]);
    cout<<"图构建成功"<<endl;
    printStateGraph(graphSize,stateGraph);

    //确定化
    deterNFA(0);

    //根据状态表构建DFA
    buildDeterDFA();

    //打印确定化的DFA
    cout<<"NFA确定化构建成功："<<endl;
    printStateGraph(graphSize,stateGraph);

    //最小化
    MinDFA();

    //打印最小化的DFA
    cout<<"最小化的DFA构建成功："<<endl;
    printStateGraph(minSize,minDFAGraph);

    return 0;
}
