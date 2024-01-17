#include <stdio.h>
#include <stdlib.h>

typedef char ElementType;
typedef struct Treenode *PTree;
struct Treenode
{
    ElementType Data;
    PTree lchild;
    PTree rchild;
};//若采用线性存储方式，则需要对结点编号，并将NULL定义为-1
/*使用先序遍历创建二叉树*/
PTree CreateBinTree(void)
{
    char data;
    scanf("%c",&data);
    if(data == '-')
        return NULL;
    else
    {
        PTree Rootnode = (PTree)malloc(sizeof(struct Treenode));
        Rootnode->Data = data;
        Rootnode->lchild = CreateBinTree();
        Rootnode->rchild = CreateBinTree();
        return Rootnode;
    }
}
/*先序遍历二叉树*/
void Preorder_Traversal(PTree T)
{
    if(T == NULL)
        return;
    printf("%c",T->Data);
    Preorder_Traversal(T->lchild);
    Preorder_Traversal(T->rchild);
}
/*中序遍历二叉树*/
void Inorder_Traversal(PTree T)
{
    if(T == NULL)
        return;
    Inorder_Traversal(T->lchild);
    printf("%c",T->Data);
    Inorder_Traversal(T->rchild);
}
/*后序遍历二叉树*/
void Postorder_Traversal(PTree T)
{
    if(T == NULL)
        return;
    Postorder_Traversal(T->lchild);
    Postorder_Traversal(T->rchild);
    printf("%c",T->Data);
}
/*层序遍历*/
typedef struct Queue_node *PQnode;
struct Queue_node{
    struct Treenode node;
    PQnode next;
};
typedef struct Queue *PQueue;
struct Queue{
    PQnode front;
    PQnode rear;
};
PQueue CreateQueue(void)
{
    PQueue Ptr = (PQueue)malloc(sizeof(struct Queue));
    PQnode headnode = (PQnode)malloc(sizeof(struct Queue_node));
    headnode->next = NULL;
    Ptr->front = headnode;
    Ptr->rear = headnode;
    return Ptr;
}
void InQueue(PQueue Q,struct Treenode T)
{
    PQnode newnode = (PQnode)malloc(sizeof(struct Queue_node));
    newnode->node = T;
    newnode->next = NULL;
    Q->rear->next = newnode;
    Q->rear = newnode;
    return;
}
int IsemptyQueue(PQueue Q)
{
    return (Q->front->next == NULL);
}
struct Treenode OutQueue(PQueue Q)
{
    if(IsemptyQueue(Q))
        return ;
    else
    {
        if(Q->front->next == Q->rear)
            Q->rear = Q->front;
        PQnode Banishednode = Q->front->next;
        Q->front->next = Banishednode->next;
        struct Treenode node = Banishednode->node;
        free(Banishednode);
        return node;
    }
}
void Layerorder_Tree(PTree T)
{
    PQueue TreeQueue = CreateQueue();
    InQueue(TreeQueue,*T);
    while(!IsemptyQueue(TreeQueue))
    {
        struct Treenode node = OutQueue(TreeQueue);
        printf("%c",node.Data);
        if(node.lchild != NULL)
            InQueue(TreeQueue,*(node.lchild));
        if(node.rchild != NULL)
            InQueue(TreeQueue,*(node.rchild));
    }
}
/*先序、中序、后序遍历的非递归实现*/
typedef struct Stack *PStack;
struct Stack{
    struct Treenode node;
    PStack next;
};
PStack CreateStack(void)
{
    PStack Ptr = (PStack)malloc(sizeof(struct Stack));
    Ptr->next = NULL;
    return Ptr;
}
void PushStack(PStack S,struct Treenode node)
{
    PStack newnode = (PStack)malloc(sizeof(struct Stack));
    newnode->node = node;
    newnode->next = S->next;
    S->next = newnode;
    return;
}
int IsemptyStack(PStack S)
{
    return (S->next == NULL);
}
struct Treenode PopStack(PStack S)
{
    if(IsemptyStack(S))
        return ;
    else
    {
        PStack Banishednode = S->next;
        S->next = Banishednode->next;
        struct Treenode node = Banishednode->node;
        free(Banishednode);
        return node;
    }
}
void Preorder_nonrecursive(PTree T)//先序遍历的非递归实现
{
    PStack TreeStack = CreateStack();
    PTree nownode = T;//将当前结点初始化为根节点
    while(1)
    {
        if(nownode != NULL)
        {
            printf("%c",nownode->Data);
            PushStack(TreeStack,*nownode);//若当前结点非空，打印当前结点并入栈
            nownode = nownode->lchild;//当前结点设置为左儿子
        }
        else//若为空
        {
            if(IsemptyStack(TreeStack))//若栈也为空，表示遍历完毕
                return;
            else
            {
                struct Treenode node = PopStack(TreeStack);
                nownode = node.rchild;//否则出栈，并将当前结点设置为栈顶元素的右儿子
            }
        }
    }
}
void Inorder_nonrecursive(PTree T)//中序遍历的非递归实现
{
    PStack TreeStack = CreateStack();
    PTree nownode = T;//将当前结点初始化为根节点
    while(1)
    {
        if(nownode != NULL)
        {
            PushStack(TreeStack,*nownode);//将当前结点先保存在栈中
            nownode = nownode->lchild;
        }
        else//若为空
        {
            if(IsemptyStack(TreeStack))//若栈为空，表示遍历完毕
                return;
            else
            {
                struct Treenode node = PopStack(TreeStack);
                printf("%c",node.Data);//输出栈顶元素
                nownode = node.rchild;//将当前结点设置为栈顶元素的右儿子
            }
        }
    }
}
void Postorder_nonrecursive(PTree T)//后序遍历的非递归实现,与前面不同采用双栈写法
{
    PStack TreeStack = CreateStack();//用于前序遍历(修改为根右左)
    PStack PostStack = CreateStack();//用于后序遍历(保存前序序列后出栈)
    PTree nownode = T;//将当前结点初始化为根节点
    while(1)
    {
        if(nownode != NULL)
        {
            PushStack(TreeStack,*nownode);//若当前结点非空，当前结点入栈
            PushStack(PostStack,*nownode);//将当前节点压入后序栈
            nownode = nownode->rchild;//当前结点设置为右儿子
        }
        else//若为空
        {
            if(IsemptyStack(TreeStack))//若栈也为空，表示遍历完毕
                break;
            else
            {
                struct Treenode node = PopStack(TreeStack);
                nownode = node.lchild;//否则出栈，并将当前结点设置为栈顶元素的左儿子
            }
        }
    }
    while(!IsemptyStack(PostStack))
    {
        struct Treenode node = PopStack(PostStack);
        printf("%c",node.Data);
    }
    return;
}

int main()
{
    system("cls");
    PTree T = CreateBinTree();
    Postorder_Traversal(T);putchar('\n');
    Postorder_nonrecursive(T);putchar('\n');
    return 0;
}