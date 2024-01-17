#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;
/*定义二叉查找树*/
typedef struct SearchTree *PSTree;
struct SearchTree{
    ElementType keyval;
    PSTree lchild;
    PSTree rchild;
};
/*在二叉查找树内寻找元素并返回结点地址*/
PSTree FindSearchTree(PSTree ST,ElementType X)
{
    if(ST == NULL)//若当前结点为空，表示没找到
        return NULL;
    if(ST->keyval == X)//若当前结点键值相等，表示找到
        return ST;
    if(ST->keyval > X)//若当前结点键值大于目标值，在左子树内找
        FindSearchTree(ST->lchild,X);
    else
        FindSearchTree(ST->rchild,X);
}
/*寻找二叉查找树中键值最大的结点*/
PSTree FindMaxSearchTree(PSTree ST)
{
    if(ST == NULL)//若为空树
        return NULL;
    PSTree Ptr = ST;//设置当前结点为根节点
    while(Ptr->rchild != NULL)
        Ptr = Ptr->rchild;
    return Ptr;
}
/*寻找二叉查找树中键值最小的结点*/
PSTree FindMinSearchTree(PSTree ST)
{
    if(ST == NULL)//若为空树
        return NULL;
    PSTree Ptr = ST;//设置当前结点为根节点
    while(Ptr->lchild != NULL)
        Ptr = Ptr->lchild;
    return Ptr;
}
/*向二叉查找树中插入新的结点*/
PSTree InsertSearchTree(PSTree ST,ElementType X)//每次向上一级返回子树的根节点
{
    if(ST == NULL)//若为空，表示找到合适位置，创建新节点
    {
        ST = (PSTree)malloc(sizeof(struct SearchTree));
        ST->keyval = X;
        ST->lchild = NULL;ST->rchild = NULL;
    }
    else
    {
        if(ST->keyval > X)//若比目标值大，进入左子树搜索
            ST->lchild = InsertSearchTree(ST->lchild,X);
        if(ST->keyval < X)//若比目标值小，进入右子树搜索
            ST->rchild = InsertSearchTree(ST->rchild,X);
        //若相等，则什么都不做
    }
    return ST;
}
/*删除二叉搜索树指定元素*/
PSTree DeleteSearchTree(PSTree ST,ElementType X)
{
    if(ST != NULL)
    {
        if(ST->keyval > X)//根节点大于目标值，进入左子树查询
            ST->lchild = DeleteSearchTree(ST->lchild,X);
        else if(ST->keyval < X)//根节点小于目标值，进入右子树查询
            ST->rchild = DeleteSearchTree(ST->rchild,X);
        else//找到目标结点
        {
            if(ST->lchild != NULL && ST->rchild != NULL)//若左右儿子非空
            {
                PSTree Rightmin = FindMinSearchTree(ST->rchild);
                ST->keyval = Rightmin->keyval;
                ST->rchild = DeleteSearchTree(ST->rchild,Rightmin->keyval);
            }
            else//只有一个儿子或全空
            {
                PSTree Banishednode = ST;
                if(ST->lchild == NULL)
                    ST = ST->rchild;
                else
                    ST = ST->lchild;
                free(Banishednode);                
            }
        }
    }
    return ST;
}
/*从插入序列建立二叉搜索树*/
PSTree CreateSearchTree(ElementType* arrX,int N)
{
    PSTree BSTree = NULL;
    for(int i = 0;i < N;i++)
        BSTree = InsertSearchTree(BSTree,arrX[i]);
    return BSTree;
}
/*前序遍历二叉树*/
void Preorder_traversal(PSTree ST)
{
    if(ST == NULL)
        return;
    printf("%d",ST->keyval);
    Preorder_traversal(ST->lchild);
    Preorder_traversal(ST->rchild);
}
int main()
{
    system("cls");
    const int N = 7;
    ElementType arrX[] = {4,3,1,2,7,5,6};
    PSTree BSTree = CreateSearchTree(arrX,N);
    Preorder_traversal(BSTree);putchar('\n');
    DeleteSearchTree(BSTree,3);
    Preorder_traversal(BSTree);putchar('\n');
    return 0;
}