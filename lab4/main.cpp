#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stack>
#define MAX 20
#define TRUE 1
#define FALSE 0
using namespace std;

//全局变量
int visited[MAX];
int Count;
int low[MAX];
int flag[MAX];

//邻接矩阵
typedef int AdjMatrix[MAX][MAX];
typedef struct{
    int vexs[MAX];//顶点向量
    AdjMatrix arcs;//邻接矩阵
    int vexnum;//顶点数
}MGraph;

//求下标为V的顶点的第一个邻接点
int FirstAdjVex(MGraph G, int V){
    int i=0;
    while(i<G.vexnum && G.arcs[V][i]==0)
        i++; //找第v行第一个非0元
    if(i>=G.vexnum)
        return -1;
    else
        return i;
}

//求下标为V的顶点的w之后的下一个邻接点
int NextAdjVex(MGraph G, int V, int w){
    int i=w+1;
    while(i<G.vexnum && G.arcs[V][i]==0)
        i++;
    if(i>=G.vexnum)
        return -1;
    else
        return i;

}

void CreateMGraph(MGraph &G){
    int k;
    int vi,vj;
    FILE *fp;
    if((fp=fopen("test3.txt","r"))==NULL)
        printf("file cannot open\n");
    fscanf(fp, "%d", &G.vexnum);
    //scanf("%d", &G.vexnum);
    for(k=0;k<G.vexnum;k++)
        G.vexs[k]=k;
    for(int i=0;i<MAX;i++)
        memset(G.arcs[i], 0, sizeof(int)*MAX);
    while(fscanf(fp, "%d %d", &vi, &vj)!=EOF){
        G.arcs[vi][vj]=G.arcs[vj][vi]=1;
    }
    fclose(fp);
}

typedef struct{
    MGraph G;
    int v0,min,p;
}node;

struct record{
    node a;
    int state;
    record(node a,int state):a(a),state(state){}
};

void non_recursive_DFSArticul(MGraph G,int v0){
    stack<record> s;
    node* cur=(node*)malloc(sizeof(node));//初始化
    cur->G=G;
    cur->v0=v0;
    int state=0;
    while(1){
        if(state == 2){
            if(s.empty())
                break;
            *cur=s.top().a;
            state=s.top().state; //返回上层状态
            s.pop();
        }
        else if(state == 0){
            visited[cur->v0]=cur->min=++Count;
            cur->p=FirstAdjVex(cur->G, cur->v0);
            if(cur->p!=-1){
                if(visited[cur->p]==0){
                    s.push(record(*cur,1));//保存本层状态
                    cur->v0=cur->p;//更新到下层状态
                    state=0;
                }
                else if(visited[cur->p]<cur->min){
                    cur->min=visited[cur->p];
                    state=3;
                }
                else{
                    state=3;
                }
            }
            else{
                low[cur->v0]=cur->min;
                state=2;
            }
        }
        else if(state == 1){
            if(low[cur->p]<cur->min)
                cur->min=low[cur->p];
            if(low[cur->p]>=visited[cur->v0])
                flag[cur->v0]=TRUE;
            state=3;
        }
        else if(state == 3){
            cur->p=NextAdjVex(cur->G, cur->v0, cur->p);
            if(cur->p!=-1){
                if(visited[cur->p]==0){
                    s.push(record(*cur,1));//保存本层状态
                    cur->v0=cur->p;        //更新到下层状态
                    state=0;
                }
                else if(visited[cur->p]<cur->min){
                    cur->min=visited[cur->p];
                    state=3;
                }
                else{
                    state=3;
                }
            }
            else{
                low[cur->v0]=cur->min;
                state=2;
            }
        }
    }
}
/*
void DFSArticul(MGraph G, int v0){
    //从第v0个顶点出发深度优先遍历G，查找并输出关节点
    int min;
    visited[v0]=min=++Count;
    for(int p=FirstAdjVex(G, v0); p!=-1; p=NextAdjVex(G, v0, p)){
        if(visited[p]==0){
            DFSArticul(G, p);
            if(low[p]<min)
                min=low[p];
            if(low[p]>=visited[v0])
                flag[v0]=TRUE;
        }
        else if(visited[p]<min)
            min=visited[p];
    }
    low[v0]=min;
}*/

void FindArticul(MGraph G){
    //查找输出全部关节点
    Count=1;
    visited[0]=1;//0号顶点为生成树的根
    for(int i=1;i<G.vexnum;i++)
        visited[i]=0;
    int v=FirstAdjVex(G,0);
    non_recursive_DFSArticul(G, v);
    if(Count<G.vexnum){
        flag[0]=TRUE;
        while(NextAdjVex(G, 0, v)!=-1){
            v=NextAdjVex(G, 0, v);
            if(visited[v]==0)
                non_recursive_DFSArticul(G, v);
        }
    }
}

//邻接表
typedef struct ArcNode{//边结点
    int adjvex; //邻接点的下标
    struct ArcNode *nextarc; //后继链指针
}ArcNode;
typedef struct VNode{//顶点结点
    ArcNode *firstarc;//边链头指针
}AdjList[MAX];

typedef struct{
    AdjList vertices;//邻接表
    int vexnum;//顶点数和边数
    unsigned kind;//图种类标志
}ALGraph;

void CreateALGraph(ALGraph &G){
    int k,vi,vj;
    FILE *fp;
    if((fp=fopen("test3.txt","r"))==NULL)
        printf("file cannot open\n");
    fscanf(fp, "%d", &G.vexnum);
    for(k=0;k<G.vexnum;k++)
        G.vertices[k].firstarc=NULL;
    ArcNode* p;
    while(fscanf(fp, "%d %d", &vi, &vj)!=EOF){
        p=(ArcNode*)malloc(sizeof(ArcNode));
        p->adjvex=vj;
        p->nextarc=G.vertices[vi].firstarc;//头插法
        G.vertices[vi].firstarc=p;
        //无向图
        p=(ArcNode*)malloc(sizeof(ArcNode));
        p->adjvex=vi;
        p->nextarc=G.vertices[vj].firstarc;
        G.vertices[vj].firstarc=p;
    }
    fclose(fp);
}

int LocateVex(ALGraph G, int v0, int v){
    //v0和v之间连不连通
    ArcNode* p;
    for(p=G.vertices[v0].firstarc;p!=NULL;p=p->nextarc){
        if(p->adjvex==v)
            return 1;
    }
    return MAX;
}

void DijkstraALGraph(ALGraph G, int v0, int P[][MAX], int D[]){
    //P存路径
    int v, i, j, w, min, vj, k, l;
    int S[MAX];
    for(v=0;v<G.vexnum;v++){
        S[v]=0;
        D[v]=LocateVex(G, v0, v);
        //printf("D[%d]=%d\n",v,D[v]);
        for(j=0;j<G.vexnum;j++)
            P[v][j]=-1;//初始化存数为-1
        if(D[v]!=MAX){
            P[v][0]=v0;
            P[v][1]=v;
        }
    }
    D[v0]=0;
    //printf("D[%d]=%d\n",v0,D[v0]);
    S[v0]=1;
    for(i=0;i<G.vexnum;i++){
        min=MAX;
        for(w=0;w<G.vexnum;w++)
            if(S[w]==0&&D[w]<min){
                min=D[w];
                vj=w;
            }
        S[vj]=1;
        for(w=0;w<G.vexnum;w++){
            l=LocateVex(G, vj, w);
            if(S[w]==0 && min+l<D[w]){
                D[w]=min+l;
                //printf("D[%d]=%d\n",w,D[w]);
                for(k=0; P[vj][k]!=-1;k++)
                    P[w][k]=P[vj][k];
                P[w][k]=w;
                P[w][k+1]=-1;
            }
        }
    }
    //输出
    for(k=1;k<G.vexnum;k++){
        printf("%d ",D[k]);
        for(int j=0;P[k][j]!=-1;j++)
            if(P[k][j+1]!=-1)
                printf("%d->",P[k][j]);
            else
                printf("%d",P[k][j]);
        printf("\n");
    }
}

void CreatePic(){
    FILE *fp,*fq;
    if((fp=fopen("test3.txt","r"))==NULL)
        printf("file cannot open\n");
    int vexnum, vi, vj;
    fscanf(fp, "%d", &vexnum);
    if((fq=fopen("pic.dot","w"))==NULL)
        printf("file cannot open\n");
    char* str="digraph pic{\n";
    fwrite(str, sizeof(char), strlen(str), fq);
    while(fscanf(fp, "%d %d", &vi, &vj)!=EOF){
        putc(vi+'0',fq);
        fputs(" -> ",fq);
        putc(vj+'0',fq);
        fputs(" [arrowhead=\"none\"]\n",fq);
    }
    fputs("}\n",fq);
    fclose(fp);
    fclose(fq);
}

int main(){
    //DFS
    MGraph G1;
    memset(visited, FALSE, MAX * sizeof(int));
    memset(low, MAX, MAX * sizeof(int));
    CreateMGraph(G1);
    memset(flag, FALSE, MAX * sizeof(int));
    FindArticul(G1);
    for(int i=0;i<G1.vexnum;i++)
        if(flag[i])
            printf("%d ",i);
    printf("\n");
    //BFS
    ALGraph G2;
    memset(visited, FALSE, MAX * sizeof(int));
    CreateALGraph(G2);
    int P[MAX][MAX], D[MAX];
    DijkstraALGraph(G2, 0, P, D);
    //pic
    CreatePic();
}