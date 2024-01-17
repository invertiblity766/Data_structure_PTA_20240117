#include <stdio.h>
#include <stdlib.h>
#define MaxSize 1000
#define NULL -1

typedef struct List *PList;
struct List{
    int keyval[MaxSize];
    int last;
};
/*创建空表*/
PList CreateList(void)
{
    PList Ptr = (PList)malloc(sizeof(struct List));
    Ptr->last = NULL;
    return Ptr;
}
/*向空线性表申请N个空间*/
void SetNSpaceList(PList L,int N)
{
    if(N > MaxSize)
        return ;
    for(int i = 0;i < N;i++)
        L->keyval[i] = NULL;
    L->last = N - 1;
}
/*打印线性表*/
void DisplayList(PList L)
{
    for(int i = 0;i <= L->last;i++)
    {
        printf("%d",L->keyval[i]);
        if(i < L->last)
            putchar(' ');
    }
    putchar('\n');
    return ;
}
/*对线性表进行快速排序*/
void swapList(PList L,int pos1,int pos2)
{
    int temp = L->keyval[pos1];
    L->keyval[pos1] = L->keyval[pos2];
    L->keyval[pos2] = temp;
    return ;
}
int SelectBaseKey(PList L,int left,int right)
{
    if(left < right)
    {
        int mid = (left + right) / 2;
        if(L->keyval[mid] > L->keyval[right])
            swapList(L,mid,right);
        if(L->keyval[left] > L->keyval[right])
            swapList(L,left,right);
        if(L->keyval[mid] > L->keyval[left])
            swapList(L,mid,left);
    }
    return L->keyval[left];
}
void Quicksort(PList L,int left,int right)
{
    int left_ptr = left;
    int right_ptr = right;
    int BaseKey = SelectBaseKey(L,left,right);
    if(left > right)
        return ;
    while(left_ptr < right_ptr)
    {
        /*右指针寻找第一个比基准值小的位置*/
        while((left_ptr < right_ptr) && (L->keyval[right_ptr] >= BaseKey))
            right_ptr --;
        /*左指针寻找第一个比基准值大的位置*/
        while((left_ptr < right_ptr) && (L->keyval[left_ptr] <= BaseKey))
            left_ptr ++;
        if(left_ptr == right_ptr)//找到基准值位置，交换正确位置与基准值所在位置
            swapList(L,left,left_ptr);
        else
            swapList(L,left_ptr,right_ptr);//否则交换左右指针出元素
    }
    Quicksort(L,left,left_ptr - 1);//对左侧序列使用快速排序
    Quicksort(L,right_ptr + 1,right);//对右侧序列使用快速排序
    return ;
}
/*复制有序序列的子序列*/
PList GetSubList(PList L,int left,int right)
{
    PList Ptr = CreateList();
    SetNSpaceList(Ptr, right - left + 1);
    for(int i = left;i <= right;i++)
        Ptr->keyval[i - left] = L->keyval[i];
    return Ptr;
}
/*求取有序序列N左子树的数量*/
int GetLeftTreenum(int N)
{
    int power_res = 2;//初始化指数运算结果为2^1
    while(power_res - 1< N)
        power_res = power_res * 2;
    power_res = power_res / 2;//得到不大于N + 1的2^k运算结果
    int remain = N + 1 - power_res;
    power_res = power_res / 2;
    return power_res - 1 + ((remain < power_res) ? remain : power_res);
}
/*求取一有序序列中完全二叉树的根节点*/
void SetRootCBT(PList Treenode,PList CBT,int RootPos)
{
    /*
        CBT:完全二叉树序列
        Treenode:有序树结点序列
        Rootpos:当前根节点在完全二叉树序列中的下标
    */
    int N = Treenode->last + 1;//获取序列长度
    if(N == 1)//若序列只有一个元素，其为根节点
        CBT->keyval[RootPos] = Treenode->keyval[0];
    else if(N >= 2)//若序列中有不止一个元素
    {
        int LeftTreenum = GetLeftTreenum(N);//获得左子树的数量
        CBT->keyval[RootPos] = Treenode->keyval[LeftTreenum];//设置根节点
        PList LeftTree = GetSubList(Treenode,0,LeftTreenum - 1);
        PList RightTree = GetSubList(Treenode,LeftTreenum + 1,Treenode->last);
        SetRootCBT(LeftTree,CBT,2 * RootPos + 1);//设置左儿子
        SetRootCBT(RightTree,CBT,2 * RootPos + 2);//设置右儿子
    }
    //else N <= 0，则什么都不做
    return ;
}
int main()
{
    system("cls");
    int N;
    scanf("%d",&N);getchar();//吃掉换行的空格
    PList CBT = CreateList();SetNSpaceList(CBT,N);//用于储存二叉树
    PList TreeNodes = CreateList();SetNSpaceList(TreeNodes,N);//用于储存树结点序列
    for(int i = 0;i < N;i++)
    {
        int keyval;
        scanf("%d",&keyval);
        TreeNodes->keyval[i] = keyval;
    }
    Quicksort(TreeNodes,0,TreeNodes->last);//得到待分类树结点的有序序列
    SetRootCBT(TreeNodes,CBT,0);
    DisplayList(CBT);
    return 0;
}