#include <stdio.h>
#include <stdlib.h>

typedef struct AVLTree *PATree;
struct AVLTree{
    int height;//记录当前结点高度，用于旋转操作
    int keyval;
    PATree lchild;
    PATree rchild;
};
/*较大值函数*/
int MAX(int a,int b)
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
PATree InsertAVLTree(PATree AT,int X)
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
/*根据插入序列建立AVL树*/
PATree CreateAVLTree(void)
{
    int N;
    scanf("%d",&N);getchar();
    PATree Root = NULL;
    for(int i = 1;i <= N;i++)
    {
        int keyval;
        scanf("%d",&keyval);
        Root = InsertAVLTree(Root,keyval);
    }
    return Root;
}
int main()
{
    system("cls");
    PATree Root = CreateAVLTree();
    printf("%d",Root->keyval);
    return 0;
}