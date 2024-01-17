#include <stdio.h>
#include <stdlib.h>

#define NULL -1
#define MaxTreeSize 10

typedef struct Treenode *PTree;//树声明定义
struct Treenode{
    int number;//结点编号
    int lchild;
    int rchild;//左右子树编号
}T[MaxTreeSize];
typedef struct Queue_node *Pnode;
struct Queue_node{
    struct Treenode node;
    Pnode next;
}Queue_node;//定义队列结点
typedef struct Queue *PQueue;
struct Queue{
    Pnode front;
    Pnode rear;
};//定义队列

/*创建空队列*/
PQueue CreatenewQueue(void)
{
    PQueue Ptr;Pnode headnode;
    Ptr = (PQueue)malloc(sizeof(struct Queue));//为队列申请空间
    headnode = (Pnode)malloc(sizeof(struct Queue_node));//申请头结点
    Ptr->front = headnode;Ptr->rear = headnode;//队首队尾指针指向头结点
    headnode->next = NULL;//头结点接地，表示空表
    return Ptr;
}
/*入队*/
void InQueue(PQueue Q,struct Treenode node)
{
    Pnode newnode;
    newnode = (Pnode)malloc(sizeof(struct Queue_node));//为插入队列的新结点申请内存
    newnode->node = node;newnode->next = NULL;//插入到队尾，新节点接地
    Q->rear->next = newnode;//将当前队尾元素指向新结点
    Q->rear = newnode;//更新队尾指针
    return ;
}
/*判断队是否为空*/
int isEmptyQueue(PQueue Q)
{
    return (Q->front->next == NULL);
}
/*出队*/
struct Treenode OutQueue(PQueue Q)
{
    if(isEmptyQueue(Q))//若队为空，则出队失败
        return ;
    if(Q->front->next == Q->rear)//若只有一个有效结点，需要移动队尾指针指向头结点
        Q->rear = Q->front;
    Pnode Banishednode = Q->front->next;
    Q->front->next = Banishednode->next;
    struct Treenode treenode = Banishednode->node;
    free(Banishednode);
    return treenode;
}
/*展示队列*/
void DisplayQueue(PQueue Q)
{
    Pnode Ptr = Q->front;
    while(Ptr->next != NULL)
    {
        Ptr = Ptr->next;
        printf("%d ",Ptr->node.number);
    }
    putchar('\n');
    return;
}
/*读取并创建树*/
void CreateTree(PTree T,int N)
{
    for(int i = 0;i < N;i++)
    {
        char lchild,rchild;
        scanf("%c %c",&lchild,&rchild);getchar();
        T[i].number = i;
        T[i].lchild = (lchild == '-') ? NULL : (lchild - '0');
        T[i].rchild = (rchild == '-') ? NULL : (rchild - '0');
    }
    return ;
}
/*寻找树的根节点*/
int FindRoot(PTree T,int N)
{
    int flags[MaxTreeSize];//用于标记是否出现在子结点内
    for(int i = 0;i < N;i++)
        flags[i] = 0;//初始化为均没出现的情况
    for(int i = 0;i < N;i++)
    {
        if(T[i].lchild != NULL)
            flags[T[i].lchild] = 1;
        if(T[i].rchild != NULL)
            flags[T[i].rchild] = 1;
    }
    for(int i = 0;i < N;i++)
    {
        if(flags[i] == 0)
            return i;
    }
    return NULL;
}
/*层序遍历并输出叶结点编号*/
void Findleaves(PTree T,int root)
{
    int leave_num = 0;//统计叶结点数目，用于输出格式调整
    PQueue Q = CreatenewQueue();//创建空队列
    InQueue(Q,T[root]);//将根节点放入队列
    while(!isEmptyQueue(Q))
    {
        struct Treenode treenode = OutQueue(Q);//出队一个元素
        if(treenode.lchild != NULL)
            InQueue(Q,T[treenode.lchild]);
        if(treenode.rchild != NULL)
            InQueue(Q,T[treenode.rchild]);//将出队结点的左右儿子入队
        if(treenode.lchild == NULL && treenode.rchild == NULL)//判断当前结点是否为叶结点
        {
            if(leave_num++ > 0)
                putchar(' ');
            printf("%d",treenode.number);
        }
    }
    putchar('\n');
}

int main()
{
    system("cls");
    int N;
    scanf("%d",&N);
    getchar();//读取树的结点数
    if(N == 0)
        return 0;//空树，不输出
    CreateTree(T,N);//建树
    int root = FindRoot(T,N);//查找根节点位置
    Findleaves(T,root);
    return 0;
}