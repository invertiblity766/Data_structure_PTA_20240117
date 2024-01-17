#include <stdio.h>
#include <stdlib.h>

typedef struct SearchTree *PSTree;
struct SearchTree{
    int keyval;
    PSTree lchild;
    PSTree rchild;
};

/*向二叉查找树插入新结点*/
PSTree InsertSearchTree(PSTree ST,int X)
{
    if(ST == NULL)//找到合适插入位置
    {
        ST = (PSTree)malloc(sizeof(struct SearchTree));
        ST->keyval = X;
        ST->lchild = NULL;ST->rchild = NULL;
    }
    else
    {
        if(ST->keyval > X)//大于目标值，进入左子树
            ST->lchild = InsertSearchTree(ST->lchild,X);
        if(ST->keyval < X)//小于目标值，进入右子树
            ST->rchild = InsertSearchTree(ST->rchild,X);
        //相等则什么都不做
    }
    return ST;
}
/*依据插入序列建立二叉搜索树*/
PSTree CreateSearchTree(int N)
{
    PSTree BSTree = NULL;
    for(int i = 1;i <= N;i++)
    {
        int keyval;
        scanf("%d",&keyval);
        BSTree = InsertSearchTree(BSTree,keyval);
    }
    getchar();//吃掉一个序列输入完后的回车
    return BSTree;
}
/*比较两颗树是否相等*/
int IsSameTree(PSTree ST1,PSTree ST2)
{
    if(ST1 == NULL && ST2 == NULL)//两者均为空树，则相等
        return 1;
    else if(ST1 != NULL && ST2 != NULL)//两者均非空
    {
        if(ST1->keyval != ST2->keyval)//根节点不等则不相等
            return 0;
        //若相等，比较其左子树与右子树，均相等则整棵树相等
        return IsSameTree(ST1->lchild,ST2->lchild) && IsSameTree(ST1->rchild,ST2->rchild);
    }
    else//若一个空一个非空，则肯定不相等
        return 0;
}
int main()
{
    system("cls");
    while(1)
    {
        int N,L;
        scanf("%d",&N);//首先读取序列长度N
        if(N == 0)//若N = 0，结束主程序
            break;
        scanf("%d",&L);getchar();//否则继续读取待检测序列个数L，并用getchar()吃掉结尾回车
        PSTree InitTree = CreateSearchTree(N);//首先建立初始二叉搜索树
        for(int i = 1;i <= L;i++)//读取后序L个序列，并依次检测是否相同
        {
            PSTree InputTree = CreateSearchTree(N);
            printf(IsSameTree(InitTree,InputTree) ? "Yes\n" : "No\n");
        }
    }
    return 0;
}