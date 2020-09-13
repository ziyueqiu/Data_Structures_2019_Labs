#include <stdio.h>
#include <queue>
#include <string.h>
#include <stdlib.h>

#define MAXLEN 100

using namespace std;

typedef enum{ Link, Thread} PointerTag;
typedef struct BiThrNode{
    char data;
    struct BiThrNode *lchild,*rchild,*parent;
    PointerTag ltag,rtag;
}BiThrNode, *BiThrTree;

typedef struct
{
    int num;    //存储整形数字
    char ch;    //存储运算符号
} ElemType;

typedef struct BiNode{
    ElemType data;
    struct BiNode *lchild,*rchild;
}BiNode, *BiTree;

int n;//the total number of knots
char save[MAXLEN];
queue<BiThrTree> que;

BiThrTree CreateTree(){
    int tn=n;
    int point=0;//pointer of the save[]
    BiThrTree p,t,q;
    //p for root, q for create a new knot, t for exchange

    if(save[point]=='#')
        return NULL;//if root even not exist, just return NULL
    else{
        p=(BiThrTree)malloc(sizeof(BiThrNode));
        p->data=save[point];
        que.push(p);
        tn--;
    }
    while(!que.empty()){
        t=que.front();
        que.pop();
        if(tn==0)
            break;
        point++;
        if(save[point]!='#'){
            q=(BiThrTree)malloc(sizeof(BiThrNode));
            q->data=save[point];
            t->lchild=q;
            t->lchild->parent=t;
            que.push(q);
            tn--;
        }
        else{
            t->lchild=NULL;
            tn--;
        }
        point++;
        if(save[point]!='#'){
            q=(BiThrTree)malloc(sizeof(BiThrNode));
            q->data=save[point];
            t->rchild=q;
            t->rchild->parent=t;
            que.push(q);
            tn--;
        }
        else {
            t->rchild = NULL;
            tn--;
        }
    }
    return p;
}

void PreOrder(BiThrTree Root){
    if(!Root)
        return;
    printf("%c",Root->data);
    PreOrder(Root->lchild);
    PreOrder(Root->rchild);
}

void InOrder(BiThrTree Root){
    if(!Root)
        return;
    InOrder(Root->lchild);
    printf("%c",Root->data);
    InOrder(Root->rchild);
}

void PostOrderThreading(BiThrTree& Root,BiThrTree& Prev){
    if(Root==NULL)
        return;
    PostOrderThreading(Root->lchild, Prev);
    PostOrderThreading(Root->rchild, Prev);

    if(Root->lchild==NULL){
        Root->lchild=Prev;
        Root->ltag=Thread;
    }
    if(Prev!=NULL && Prev->rchild==NULL){
        Prev->rchild=Root;
        Prev->rtag=Thread;
    }
    Prev=Root;
}

void PostOrder(BiThrTree pRoot)
{
    BiThrTree pCur = pRoot;
    BiThrTree prev = NULL;

    while (pCur)
    {

        while (pCur->lchild!=prev && pCur->ltag == Link)//找到最左边的结点
            pCur = pCur->lchild;

        while (pCur->rtag == Thread)//访问连在一起的后续结点
        {
            printf("%c", pCur->data);
            prev = pCur;
            pCur = pCur->rchild;
        }
        while (pCur && pCur->rchild == prev)//访问当前结点
        {
            printf("%c", pCur->data);
            prev = pCur;
            pCur = pCur->parent;
        }
        if (pCur && pCur->rtag == Link)//开始访问右子树
        {
            pCur = pCur->rchild;
        }
    }
}

int isOperator(char s[]){
    if(strcmp(s,"+")==0)
        return 1;
    if(strcmp(s,"-")==0)
        return 1;
    if(strcmp(s,"*")==0)
        return 1;
    if(strcmp(s,"/")==0)
        return 1;
    return 0;
}

void RevPolish(BiTree Root){
    if(!Root)
        return;
    RevPolish(Root->lchild);
    RevPolish(Root->rchild);
    if(Root->data.num==-1)
        printf("%c ",Root->data.ch);
    else if(Root->data.ch=='#')
        printf("%d ",Root->data.num);
    else{
        printf("num and char error!\n");
    }
}

BiTree CreateExpression(){
    char s[MAXLEN];
    scanf("%s",s);
    BiTree p=(BiTree)malloc(sizeof(BiNode));
    if(!isOperator(s)){
        p->lchild=p->rchild=NULL;
        p->data.num=atof(s);
        p->data.ch='#';//initial
    }
    else{
        p->data.ch=s[0];
        p->data.num=-1;//initial
        p->lchild=CreateExpression();
        p->rchild=CreateExpression();
    }
    return p;
}

int Lower(char a,char b){
    if((a=='+'||a=='-')&&(b=='*'||b=='/'))
        return 1;
    return 0;
}

void ArExp(BiTree& T){
    if(T){
        if(T->lchild){
            if(Lower(T->lchild->data.ch,T->data.ch)){
                printf("(");
                ArExp(T->lchild);
                printf(")");
            }
            else
            {
                ArExp(T->lchild);
            }
        }
        if(T->data.num==-1)
            printf("%c",T->data.ch);
        else if(T->data.ch=='#')
            printf("%d",T->data.num);
        else{
            printf("num and char error!\n");
        }
        if(T->rchild){
            if(Lower(T->rchild->data.ch,T->data.ch)){
                printf("(");
                ArExp(T->rchild);
                printf(")");
            }
            else
            {
                ArExp(T->rchild);
            }
        }
    }
    return;
}

double Calculate(BiTree T){
    if(T->data.ch=='#')
        return T->data.num;

    switch(T->data.ch){
        case '+': return Calculate(T->lchild)+Calculate(T->rchild);
        case '-': return Calculate(T->lchild)-Calculate(T->rchild);
        case '*': return Calculate(T->lchild)*Calculate(T->rchild);
        case '/': return Calculate(T->lchild)/Calculate(T->rchild);
        default: printf("Error.");
    }
}

int main() {
    int num;
    printf("put in 1 for threading\nput in 2 for arithmetic\n");
    scanf("%d",&num);
    if(num==1){
        scanf("%s",save);
        n=strlen(save);
        BiThrTree p=CreateTree(),prev=NULL;
        //先序遍历
        PreOrder(p);
        printf("\n");
        //中序遍历
        InOrder(p);
        printf("\n");
        //后续线索化及遍历
        PostOrderThreading(p, prev);
        PostOrder(p);
    }
    else if(num==2){
        BiTree p=CreateExpression();
        ArExp(p);
        printf("\n");
        RevPolish(p);
        printf("\n");
        printf("%g",Calculate(p));
    }
    return 0;
}