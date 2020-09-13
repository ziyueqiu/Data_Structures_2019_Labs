#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define LEN 50

/*
 基本要求:
1. 计算表达式的值。ok
 选做要求:
1. 增加乘方ok、单目减ok等运算;
2. 运算量可以是变量ok或实数ok类型。
 */

typedef struct StackNode1{
    double data;
    struct StackNode1 *next;
}StackNode1;

typedef struct StackNode2{
    char data;
    struct StackNode2 *next;
}StackNode2;

//设定：带头指针的链栈

//创建栈头结点
StackNode1* CreateStack1(){
    StackNode1* head = (StackNode1*)malloc(sizeof(StackNode1));
    if(head == NULL){
        printf("Memory1 allocate failed.\n");
        return NULL;
    }
    head->data = 0;
    head->next = NULL;
    return head;
}

StackNode2* CreateStack2(){
    StackNode2* head = (StackNode2*)malloc(sizeof(StackNode2));
    if(head == NULL){
        printf("Memory2 allocate failed.\n");
        return NULL;
    }
    head->data = '\0';
    head->next = NULL;
    return head;
}

//入栈
void Push1(StackNode1* head, double e){
    if(head == NULL)
        return;
    StackNode1* node = (StackNode1*)malloc(sizeof(StackNode1));
    if(node == NULL){
        printf("Memory1 allocate failed.\n");
        return;
    }
    node->data = e;
    node->next = head->next;
    head->next = node;
}

void Push2(StackNode2* head, char e){
    if(head == NULL)
        return;
    StackNode2* node = (StackNode2*)malloc(sizeof(StackNode2));
    if(node == NULL){
        printf("Memory2 allocate failed.\n");
        return;
    }
    node->data = e;
    node->next = head->next;
    head->next = node;
}

//出栈
double Pop1(StackNode1* head){
    if(head == NULL || head->next == NULL){
        printf("Error1.\n");
        return 0;
    }
    StackNode1* node = (StackNode1*)malloc(sizeof(StackNode1));
    if(node == NULL){
        printf("Memory1 allocate failed.\n");
        return 0;
    }
    StackNode1* temp = head->next;
    head->next = temp->next;
    double val = temp->data;
    free(temp);
    return val;
}

char Pop2(StackNode2* head){
    if(head == NULL || head->next == NULL){
        printf("Error2.\n");
        return 0;
    }
    StackNode2* node = (StackNode2*)malloc(sizeof(StackNode2));
    if(node == NULL){
        printf("Memory2 allocate failed.\n");
        return 0;
    }
    StackNode2* temp = head->next;
    head->next = temp->next;
    char val = temp->data;
    free(temp);
    return val;
}

//获得顶端元素值
char GetTop2(StackNode2* head){
    if(head == NULL || head->next == NULL){
        printf("Error GetTop2.\n");
        return 0;
    }
    return head->next->data;
}

char precede(char ch1, char ch2){
    switch(ch1){
        case '+':
        case '-':
            if(ch2=='+'||ch2=='-'||ch2==')'||ch2=='#')
                return '>';
            else if(ch2=='*'||ch2=='/'||ch2=='('||ch2=='^')
                return '<';
            break;
        case '*':
        case '/':
            if(ch2=='('||ch2=='^')
                return '<';
            else
                return '>';
        case '(':
            if(ch2==')')
                return '=';
            else if(ch2=='#')
                return '>';
            else
                return '<';
        case ')':
            if(ch2=='(')
                return '=';
            else
                return '>';
        case '#':
            if(ch2=='#')
                return '=';
            else
                return '<';
        case '^':
            if(ch2=='(')
                return '<';
            else
                return '>';
    }
}

int isOperator(char ch){
    char a[]="()+-*/^#";
    for(int i=0;a[i]!='\0';i++){
        if(a[i]==ch)
            return 1;
    }
    return 0;
}

double f(double a, double b, char ch){
    //计算a ch b
    switch(ch){
        case '+': return a+b;
        case '-': return a-b;
        case '*': return a*b;
        case '/': return a/b;
        case '^': return pow(a,b);
        default: printf("Error.");
    }
    return 0;
}

double transform(char exp[]){//exp是输入的表达式字符串
    StackNode1* head1 = NULL;//数值栈OPND
    StackNode2* head2 = NULL;//运算符栈OPTR
    head1 = CreateStack1();
    head2 = CreateStack2();

    Push2(head2,'#');
    Push2(head2,'#');
    int k=0,j=0;//从前往后读取
    char* p;//不影响exp
    char* save[LEN];//用在将字符串转为double保存
    double val,b,a;
    char ch;//存运算符
    int flag;

    for(p=exp;*p!='\0';p++)
        ;
    *p='#';
    *(p+1)='#';
    *(p+2)='\0';
    p=exp;

    if(*p=='-'){//单目
        while(*(p+j)!='\0' && !isOperator(*(p+j))){//交换位置
            j++;
        }
        memset(save,'\0', sizeof(char)*LEN);
        strncpy(save,p,j* sizeof(char));
        val = atof(save);
        Push1(head1, val);
        k=j;
    }

    while(*(p+k)!='#' || GetTop2(head2)!='#'){
        if(!isOperator(*(p+k)) || (*(p+k)=='('&&*(p+k+1)=='-')){
            if(!isOperator(*(p+k))){
                j = k;
                while(*(p+j)!='\0' && !isOperator(*(p+j))){//交换位置
                    j++;
                }
                memset(save,'\0', sizeof(char)*LEN);
                strncpy(save,p+k,(j-k)* sizeof(char));
                val = atof(save);
                Push1(head1, val);
                k=j;
            }
            else{
                j = k+2;
                while(*(p+j)!=')'){//交换位置
                    j++;
                }
                memset(save,'\0', sizeof(char)*LEN);
                strncpy(save,p+k+1,(j-k-1)* sizeof(char));
                val = atof(save);
                Push1(head1, val);
                k=j+1;
            }
        }
        else{
            flag=1;
            while(flag==1){
                flag=0;
                ch=GetTop2(head2);
                switch(precede(ch,*(p+k))){
                    case '<': Push2(head2,*(p+k)); break;
                    case '=': Pop2(head2); break;
                    case '>': b=Pop1(head1); a=Pop1(head1);
                              ch=Pop2(head2); Push1(head1,f(a,b,ch));
                              flag=1; break;
                }
            }
            k++;
        }
    }
    printf("%g\n",head1->next->data);
}

int main(){
    char s[LEN];
    int i;
    char x[LEN],save[LEN];
    memset(save,'\0', sizeof(char)*LEN);
    memset(x,'\0', sizeof(char)*LEN);
    while(scanf("%s",s)){
        /*
         过滤替换x
         */
        for(i=0;*(s+i)!='\0';i++){
            if(*(s+i)=='x'){
                printf("x=");
                scanf("%s",x);
                break;
            }
        }
        for(i=0;*(s+i)!='\0';i++) {
            if(*(s+i)=='x'){
                memset(save,'\0', sizeof(char)*LEN);
                strncpy(save,s+i+1,LEN * sizeof(char));//把后半段存起来
                strncpy(s+i,x,strlen(x) * sizeof(char));
                strncpy(s+i+strlen(x),save, LEN* sizeof(char));
            }
        }
        transform(s);
    }
    return 0;
}