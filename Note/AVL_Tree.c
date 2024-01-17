#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;
typedef struct AVLTree *PATree;
struct AVLTree{
    int height;//记录当前结点高度，用于旋转操作
    ElementType keyval;
    PATree lchild;
    PATree rchild;
};
/*较大值函数*/
int MAX(ElementType a,ElementType b)
{
    return (a > b) ? a : b;
}
/*得到结点高度*/
int GetTreeHeight(PATree AT)
{
    return (AT == NULL) ? -1 : AT->height;
}
/*LL单旋转，麻烦结点在根节点的左子树的左子树上*/
PATree SingleRotateLL(PATree AT)//表现为右旋转
{
    PATree ATLeft = AT->lchild;//表示根节点左子树
    AT->lchild = ATLeft->rchild;//用左子树的右子树替代根节点的左子树
    ATLeft->rchild = AT;//用左子树代替根节点
    //更新两个结点的高度
    AT->height = MAX(GetTreeHeight(AT->lchild),GetTreeHeight(AT->rchild)) + 1;
    ATLeft->height = MAX(GetTreeHeight(ATLeft->lchild),GetTreeHeight(AT)) + 1;
    return ATLeft;//返回新的根结点 
}
/*RR单旋转，麻烦结点在根节点的右子树的右子树上*/
PATree SingleRotateRR(PATree AT)//表现为左旋转
{
    PATree ATRight = AT->rchild;//表示根节点右子树
    AT->rchild = ATRight->lchild;//用右子树的左子树替代根节点的右子树
    ATRight->lchild = AT;//用右子树代替根节点
    //更新两个节点的高度
    AT->height = MAX(GetTreeHeight(AT->lchild),GetTreeHeight(AT->rchild)) + 1;
    ATRight->height = MAX(GetTreeHeight(ATRight->rchild),GetTreeHeight(AT)) + 1;
    return ATRight;//返回新的根节点
}
/*LR双旋转，麻烦结点在根节点左子树的右子树上*/
PATree DoubleRotateLR(PATree AT)
{
    AT->lchild = SingleRotateRR(AT->lchild);//首先通过左旋变成LL树
    return SingleRotateLL(AT);//再通过右旋得到平衡树
}
/*RL双旋转，麻烦结点再根节点右子树的左子树上*/
PATree DoubleRotateRL(PATree AT)
{
    AT->rchild = SingleRotateLL(AT->rchild);//首先通过右旋变成RR树
    return SingleRotateRR(AT);//再通过左旋得到平衡树
}
/*AVL树插入操作*/
PATree InsertAVLTree(PATree AT,ElementType X)
{
    if(AT == NULL)
    {
        AT = (PATree)malloc(sizeof(struct AVLTree));
        AT->keyval = X;AT->height = 0;
        AT->lchild = NULL;AT->rchild = NULL;
    }
    else if(AT->keyval > X)//比目标值大，进入左子树搜索
    {
        AT->lchild = InsertAVLTree(AT->lchild,X);
        if(GetTreeHeight(AT->lchild) - GetTreeHeight(AT->rchild) >= 2)//若插入导致失衡
        {
            if(AT->lchild->keyval > X)//插入结点在左子树的左子树上
                AT = SingleRotateLL(AT);
            else//插入结点在左子树的右子树上
                AT = DoubleRotateLR(AT);
        }
    }
    else if(AT->keyval < X)//比目标值小，进入右子树搜索
    {
        AT->rchild = InsertAVLTree(AT->rchild,X);
        if(GetTreeHeight(AT->rchild) - GetTreeHeight(AT->lchild) >= 2)//若插入导致失衡
        {
            if(AT->rchild->keyval < X)//插入节点在右子树的右子树上
                AT = SingleRotateRR(AT);
            else//插入结点在右子树的左子树上
                AT = DoubleRotateRL(AT);
        }
    }
    //若AT->keyval == X则什么都不做
    AT->height = MAX(GetTreeHeight(AT->lchild),GetTreeHeight(AT->rchild)) + 1;
    return AT;
}
/*寻找AVL树中键值最大的结点*/
PATree FindMaxAVLTree(PATree ST)
{
    if(ST == NULL)//若为空树
        return NULL;
    PATree Ptr = ST;//设置当前结点为根节点
    while(Ptr->rchild != NULL)
        Ptr = Ptr->rchild;
    return Ptr;
}
/*寻找AVL树中键值最小的结点*/
PATree FindMinAVLTree(PATree ST)
{
    if(ST == NULL)//若为空树
        return NULL;
    PATree Ptr = ST;//设置当前结点为根节点
    while(Ptr->lchild != NULL)
        Ptr = Ptr->lchild;
    return Ptr;
}
PATree DeleteAVLTree(PATree AT,ElementType X)
{
    if(AT == NULL)
        return NULL;
    else
    {
        if(AT->keyval > X)//比目标值大，进入左子树搜索
        {
            AT->lchild = DeleteAVLTree(AT->lchild,X);
            if(GetTreeHeight(AT->rchild) - GetTreeHeight(AT->lchild) >= 2)//若删除导致失衡
            {
                printf("RightTree unbalanced.\n");
                PATree ATRight = AT->rchild;//检查根节点右子树的左右子树高度情况
                if(GetTreeHeight(ATRight->rchild) > GetTreeHeight(ATRight->lchild))//若右子树高于左子树，则为RR失衡
                    AT = SingleRotateRR(AT);
                else//否则左子树高于右子树，为RL失衡
                    AT = DoubleRotateRL(AT);
                    
            }
        }
        else if(AT->keyval < X)//比目标值小，进入右子树搜索
        {
            AT->rchild = DeleteAVLTree(AT->rchild,X);
            if(GetTreeHeight(AT->lchild) - GetTreeHeight(AT->rchild) >= 2)//若删除导致失衡
            {
                printf("LeftTree unbalanced.\n");
                PATree ATLeft = AT->lchild;//检查根节点左子树的左右子树高度情况
                if(GetTreeHeight(ATLeft->lchild) > GetTreeHeight(ATLeft->rchild))//若左子树高于右子树，则为LL失衡
                    AT = SingleRotateLL(AT);
                else//否则右子树高于左子树，为LR失衡
                    AT = DoubleRotateLR(AT);
            }
        }
        else//若找到待删除的结点
        {
            if(AT->lchild && AT->rchild)//删除的是带两个儿子的满根节点
            {
                if(GetTreeHeight(AT->lchild) > GetTreeHeight(AT->rchild))//若左子树高于右子树，则找左侧最大值代替根节点并删除相应结点
                {
                    PATree LeftMax = FindMaxAVLTree(AT->lchild);
                    AT->keyval = LeftMax->keyval;
                    AT->lchild = DeleteAVLTree(AT->lchild,LeftMax->keyval);
                }
                else//若右子树高于左子树，则找右侧最小值替代根节点并删除相应结点
                {
                    PATree RightMin = FindMinAVLTree(AT->rchild);
                    AT->keyval = RightMin->keyval;
                    AT->rchild = DeleteAVLTree(AT->rchild,RightMin->keyval);
                }
            }
            else//删除的是只带一个儿子或全空的结点
            {
                PATree temp = AT;
                AT = AT->lchild ? AT->lchild : AT->rchild;
                free(temp);
            }
        }
        if(AT)
            AT->height = MAX(GetTreeHeight(AT->lchild),GetTreeHeight(AT->rchild)) + 1;
    }
    return AT;
}
/*层序遍历*/
typedef struct Queue_node *PQnode;
struct Queue_node{
    struct AVLTree node;
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
void InQueue(PQueue Q,struct AVLTree T)
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
struct AVLTree OutQueue(PQueue Q)
{
    if(IsemptyQueue(Q))
        return ;
    else
    {
        if(Q->front->next == Q->rear)
            Q->rear = Q->front;
        PQnode Banishednode = Q->front->next;
        Q->front->next = Banishednode->next;
        struct AVLTree node = Banishednode->node;
        free(Banishednode);
        return node;
    }
}
void Layerorder_Traversal(PATree T)
{
    PQueue TreeQueue = CreateQueue();
    InQueue(TreeQueue,*T);
    while(!IsemptyQueue(TreeQueue))
    {
        struct AVLTree node = OutQueue(TreeQueue);
        printf("%d ",node.keyval);
        if(node.lchild != NULL)
            InQueue(TreeQueue,*(node.lchild));
        if(node.rchild != NULL)
            InQueue(TreeQueue,*(node.rchild));
    }
}
/*根据插入序列建立AVL树*/
PATree CreateAVLTree(ElementType* arrX,int N)
{
    PATree Root = NULL;
    for(int i = 0;i < N;i++)
        Root = InsertAVLTree(Root,arrX[i]);
    return Root;
}
int main()
{
    system("cls");
    const int N = 8;
    ElementType arrX[N];
    for(int iX = 0;iX < N;iX++)
        arrX[iX] = iX + 1;
    PATree Root = CreateAVLTree(arrX,N);
    Layerorder_Traversal(Root);putchar('\n');
    Root = DeleteAVLTree(Root,7);
    Root = InsertAVLTree(Root,7);
    Root = DeleteAVLTree(Root,5);
    Layerorder_Traversal(Root);putchar('\n');
    printf("Tree Height is %d\n",Root->height);
    return 0;
}