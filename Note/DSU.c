/*Disjoint Set Union,并查集*/
#include <stdio.h>
#include <stdlib.h>
#define NULL -1

typedef int ElementType;
typedef struct SetStruct *PSet;
struct SetStruct{
    int* Parent;//设置父节点数组指针
    ElementType* Data;//设置数据数组指针
};//并查集定义

/*查找对应元素在Data数组中的下标*/
//int GetElement(PSet MFSet,ElementType X)

/*初始化含有N个元素的并查集*/
PSet InitSet(int N)
{
    PSet MFSet = (PSet)malloc(sizeof(struct SetStruct));
    MFSet->Data = (ElementType*)malloc(N * sizeof(ElementType));
    MFSet->Parent = (int*)malloc(N * sizeof(int));
    /*读入N个数据储存在Data数组中,可以利用哈希表等方法提高查找速度*/
    for(int i = 0;i < N;i++)
        MFSet->Parent[i] = NULL;//任意含有N个元素的数组可以映射为下标0~N - 1，具体元素对应下标的查找可以通过查找算法另外实现
    return MFSet;
}
/*查找结点属于哪个集合*/
int FindSet(PSet MFSet,int nodei)
{
    if(MFSet->Parent[nodei] <= NULL)//当前结点的父节点不大于NULL，表示当前结点即为根节点
        return nodei;
    else//表示当前结点仍然有父节点，则递归查找
        return MFSet->Parent[nodei] = FindSet(MFSet,MFSet->Parent[nodei]);//路径压缩，可将时间复杂度控制在常数范围内
}
/*合并集合*/
void JoinSet(PSet MFSet,int node1_i,int node2_i)
{
    int Root1 = FindSet(MFSet,node1_i);
    int Root2 = FindSet(MFSet,node2_i);//查找两结点的根节点
    if(Root1 != Root2)//若两结点属于不同集合，则合并其根节点
    {
        //按大小求并，总是让元素较少的集合称为元素较多集合的子树
        //集合元素数量n可由根节点的父节点记录为-n而不占用额外空间
        if(MFSet->Parent[Root1] > MFSet->Parent[Root2])
        {
            MFSet->Parent[Root2] += MFSet->Parent[Root1];
            MFSet->Parent[Root1]= Root2; 
        }
        else
        {
            MFSet->Parent[Root1] += MFSet->Parent[Root2];
            MFSet->Parent[Root2] = Root1;
        }
    }
    return ;//按大小合并与路径压缩兼容，而按秩合并不兼容
}
int main()
{
    system("cls");
    int N = 10;
    ElementType arr[N];
    for(int i = 0;i < N;i++)
        arr[i] = i;
    PSet MFSet = InitSet(N);
    while(1)
    {
        char op = getchar();//操作符
        if(op == 'S')
            break;
        int c1,c2;
        scanf("%d %d",&c1,&c2);
        getchar();//吃掉空格
        if(op == 'I')
            JoinSet(MFSet,c1,c2);
        else if(op == 'C')
            printf(FindSet(MFSet,c1) == FindSet(MFSet,c2) ? "Yes\n" : "No\n");
    }
    return 0;
}