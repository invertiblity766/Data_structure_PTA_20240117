#include <stdio.h>
#include <stdlib.h>

#define left 0;
#define right 1;
#define MAXSIZE 30

typedef struct List *PList;
struct List{
    int data[MAXSIZE];
    int last;
};
/*创建空表*/
PList CreateList(void)
{
    PList Ptr;
    Ptr = (PList)malloc(sizeof(struct List));
    Ptr->last = -1;
    return Ptr;
}
/*向表尾插入数据*/
void Listappend(PList L,int num)
{
    if(L->last == MAXSIZE - 1)
        return;
    L->last = L->last + 1;
    L->data[L->last] = num;
}
/*寻找线性表中元素并返回下标*/
int FindList(PList L,int num)
{
    for(int i = 0;i <= L->last;i++)
    {
        if(L->data[i] == num)
            return i;
    }
    return -1;
}
/*打印线性表*/
void DisplayList(PList L)
{
    for(int i = 0;i <= L->last;i++)
        printf("%d ",L->data[i]);
    putchar('\n');
    return;
}
typedef struct Stack *PStack;
struct Stack{
    int nodenum;//树结点编号
    PStack next;
};
/*创建空栈*/
PStack CreateStack(void)
{
    PStack Ptr;
    Ptr = (PStack)malloc(sizeof(struct Stack));
    Ptr->next = NULL;
    return Ptr;
}
/*入栈*/
void PushStack(PStack S,int nodenum)
{
    PStack newnode;
    newnode = (PStack)malloc(sizeof(struct Stack));
    newnode->nodenum = nodenum;
    newnode->next = S->next;
    S->next = newnode;//头插法入栈
    return ;
}
/*判断栈是否为空*/
int isEmptyStack(PStack S)
{
    return (S->next == NULL);
}
/*出栈*/
int PopStack(PStack S)
{
    if(isEmptyStack(S))
        return NULL;
    PStack Banishednode = S->next;
    S->next = Banishednode->next;
    int nodenum = Banishednode->nodenum;
    free(Banishednode);
    return nodenum;
}
/*打印栈*/
void PrintStack(PStack S)
{
    PStack Ptr = S;
    while(Ptr->next != NULL)
    {
        Ptr = Ptr->next;
        printf("%d",Ptr->nodenum);
        if(Ptr->next != NULL)
            putchar(' ');
    }
    putchar('\n');  
    return;
}
/*根据中序与先序序列得到后序序列*/
void getPostseq(PList Preorder_seq,PList Inorder_seq,PStack S)
{
    if(Preorder_seq->last == -1 || Inorder_seq->last == -1)//若序列为空，终止递归
        return;
    //第一步，取出先序序列第一项，入栈
    PushStack(S,Preorder_seq->data[0]);
    //第二步：找到中序序列中入栈元素的位置
    int RootPos = FindList(Inorder_seq,Preorder_seq->data[0]);
    //第三步：先对右子树，再对左子树做以上操作
    PList PreleftTree = CreateList();
    PList PrerightTree = CreateList();
    PList InleftTree = CreateList();
    PList InrightTree = CreateList();//用于复制子序列
    for(int i = 0;i < RootPos;i++)
        Listappend(InleftTree,Inorder_seq->data[i]);
    for(int i = RootPos + 1;i <= Inorder_seq->last;i++)
        Listappend(InrightTree,Inorder_seq->data[i]);
    for(int i = 1;i <= RootPos;i++)
        Listappend(PreleftTree,Preorder_seq->data[i]);
    for(int i = RootPos + 1;i <= Preorder_seq->last;i++)
        Listappend(PrerightTree,Preorder_seq->data[i]);
    getPostseq(PrerightTree,InrightTree,S);
    getPostseq(PreleftTree,InleftTree,S);
    return;
}
/*根据输入得到树的先序与中序序列,并推导得到后序序列*/
void getPostTraversal(void)
{
    int N;
    scanf("%d",&N);getchar();//表示结点总数
    PStack S = CreateStack();//创建一空栈
    PList Inorder_seq = CreateList();
    PList Preorder_seq = CreateList();//创建储存先序与中序序列的线性表
    for(int i = 1;i <= 2 * N;i++)
    {
        int nodenum;
        char cmd[5];
        scanf("%s",cmd);//首先读入指令
        if(cmd[1] == 'u')//若为PUSH指令
        {
            scanf("%d",&nodenum);
            getchar();//获取结点编号
            PushStack(S,nodenum);
            Listappend(Preorder_seq,nodenum);
        }
        else//若为POP指令
        {
            nodenum = PopStack(S);
            Listappend(Inorder_seq,nodenum);
        }
    }
    PStack PostS = CreateStack();
    getPostseq(Preorder_seq,Inorder_seq,PostS);
    PrintStack(PostS);
    return ;
}
int main()
{
    system("cls");
    getPostTraversal();
    return 0;
}