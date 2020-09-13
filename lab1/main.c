#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

#define SIZE 100 //类数学表达式的最大字符数
#define MAX 10000 //链表创建时头指针赋指数值

typedef struct Node{
    double p;
    int e;//e意味着expn（指数）
    struct Node *next;
}LNode,*LinkList;   //LinkList作为头指针，增加可读性

void OrderInsert(LinkList L, double p, int e);//有序插入新结点
LinkList CreatePolyn(char po[]);//创建多项式
void PrintPolyn(LinkList L);//打印多项式
LinkList AddPolyn(LinkList La, LinkList Lb, LinkList Lc);//求和
LinkList SubPolyn(LinkList La, LinkList Lb, LinkList Lc);//求差
LinkList Differential_once(LinkList L);//求导数
double Value(LinkList L, double x);//求多项式值
LinkList CopyPolyn(LinkList La, LinkList Lb);//复制多项式（La复制到Lb处）

/*
A B +
A B -
A *
A d
*/

int main(int argc, char* argv[]){
	LinkList La,Lb,Lc;
	double f;
	if(argc==4){
		La=CreatePolyn(argv[1]);
		Lb=CreatePolyn(argv[2]);

		if(strcmp(argv[3],"+")==0){
			Lc=AddPolyn(La,Lb,Lc);
			if(Lc->next==NULL)
				printf("0\n");
			else
        		PrintPolyn(Lc);
		}
		if(strcmp(argv[3],"-")==0){
			Lc=SubPolyn(La,Lb,Lc);
        	if(Lc->next==NULL)
				printf("0\n");
			else
        		PrintPolyn(Lc);
		}
	}
	
	if(argc==3){
		if(strcmp(argv[2],"d")==0){
			La=CreatePolyn(argv[1]);
			Lc=Differential_once(La);
			PrintPolyn(Lc);
		}
		else{
			La=La=CreatePolyn(argv[1]);
			f=Value(La,atof(argv[2]));
			printf("%g",f);
		}
	}
}

void OrderInsert(LinkList L, double p, int e)//有序插入新结点
{
    LinkList q1=L,q2=L;

    if(p==0)
        return;
    while(q1 && e<q1->e)
    {
        q2=q1;
        q1=q1->next;
    }
    if(q1 && e==q1->e)
    {
        q1->p+=p;
        if(q1->p==0)
        {
            q2->next=q1->next;
            free(q1);
            return;
        }
        return;
    }
    LinkList s=(LinkList)malloc(sizeof(LNode));
    q2->next=s;
    s->e=e;
    s->p=p;
    s->next=q1;
}

LinkList CreatePolyn(char po[]){
    char p[SIZE];
    char e[SIZE];
    double px;
    int ex;
    int i=0,j,k;

	LinkList head,L=(LinkList)malloc(sizeof(LNode));//申请头结点
    L->p=0;
    L->e=MAX;
    L->next=NULL;
    head=L;

    while(po[i]!='\0'){
    	memset(p, '\0', SIZE*sizeof(char));//初始化
    	//求出px,ex;
        for(j=i+1;po[j]!='\0';j++){
        	if(po[j]=='+' || po[j]=='-')
        		if(po[j-1]!='^')
        			break;
        }
        strncpy(p,po+i,(j-i)*sizeof(char));

        i=j;

        if(p[0]=='x' || (p[0]=='+'&&p[1]=='x'))
        	px=1;//一次函数kx
        else if(p[0]=='-'&&p[1]=='x')
        	px=-1;
        else{
        	//double atof(const char *str)
        	for(j=0;p[j]!='x'&&p[j]!='\0'; j++)
        		;
        	if(p[j]=='\0')
        		px=atof(p);//常数C
        	else{
        		memset(e, '\0', SIZE*sizeof(char));
        		strncpy(e, p, j*sizeof(char));
        		px=atof(e);
        	}	
        }

        for(j=0;p[j]!='x'&&p[j]!='\0'; j++)
        	;
        if(p[j]=='\0')
        	ex=0;
        else if(p[j+1]!='^')
        	ex=1;
        else{
        	j+=2;
        	memset(p, ' ', j);
        	ex=atof(p);
        }
		OrderInsert(L,px,ex);

    }

    return head;
}

void PrintPolyn(LinkList L)//打印多项式
{
    LinkList p=L;
    double px;
    int ex;
    int flag=0;

    if(p==NULL)
    {
        printf("该多项式不存在\n");
        return;
    }

    while(p->next!=NULL)
    {
    	px=p->next->p;
    	ex=p->next->e;

    	if(flag==0){
    		flag=1;
    		if(px==-1){
    			printf("-");
    		}
    		else if(px!=1){
    			printf("%g",px);
    		}
    	}
    	else{
    		if(px==-1){
    			printf("-");
    		}
    		else if(px!=1){
    			if(px>0)
    				printf("+");
    			printf("%g",px);
    		}
    		else{
    			printf("+");
    		}
    	}
    	if(ex==1)
    		printf("x");
    	else if(ex>1 ||ex<0)
    		printf("x^%d",ex);
    	else{
    		if(px==1||px==-1)
    			printf("1");
    	}
    	p=p->next;
    }
    printf("\n");
}


LinkList SubPolyn(LinkList La, LinkList Lb, LinkList Lc)//求差
{
    Lc=CopyPolyn(La,Lc);
    LinkList q=Lb;
    while(q!=NULL)
    {
        OrderInsert(Lc, -q->p, q->e);
        q=q->next;
    }
    return Lc;
}

LinkList AddPolyn(LinkList La, LinkList Lb, LinkList Lc)//求和
{
    Lc=CopyPolyn(La,Lc);
    LinkList q=Lb;
    while(q!=NULL)
    {
        OrderInsert(Lc, q->p, q->e);
        q=q->next;
    }
    return Lc;
}

LinkList CopyPolyn(LinkList La, LinkList Lb)//复制多项式（La复制到Lb处）
{
    LinkList head,q;
    Lb=(LinkList)malloc(sizeof(LNode));
    head=Lb;
    while(La)
    {
        Lb->e=La->e;
        Lb->p=La->p;
        Lb->next=(LinkList)malloc(sizeof(LNode));
        q=Lb;
        Lb=Lb->next;
        La=La->next;
    }
    q->next=NULL;

    return head;
}

LinkList Differential_once(LinkList L)//求导数
{
    LinkList t=L,k=L;
    while(L!=NULL)
    {
        if(L->e==0)
        {
            k->next=NULL;
            L=k->next;
            continue;
        }
        L->p*=L->e;
        L->e--;
        k=L;
        L=L->next;
    }
    return t;
}

double Value(LinkList L, double x)//求多项式值
{
    double sum=0;
    if(L==NULL)
    {
        printf("多项式不存在\n");
        return -1;
    }
    L=L->next;
    while(L!=NULL)
    {
        sum+=L->p*pow(x,L->e);
        L=L->next;
    }
    return sum;
}
