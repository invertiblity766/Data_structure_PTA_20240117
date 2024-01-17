#include <stdio.h>
#include <stdlib.h>

typedef struct SetStruct *PSet;
struct SetStruct{
    int* Data;//并查集数据域
    int* Parent;//并查集父指针域
};

/*初始化并查集*/
PSet InitSet(int N)
{
    PSet MFSet = (PSet)malloc(sizeof(struct SetStruct));
    MFSet->Data = (int*)malloc(N * sizeof(int));
    MFSet->Parent = (int*)malloc(N * sizeof(int));
    for(int i = 0;i < N;i++)
    {
        MFSet->Data[i] = i + 1;//网络中各编号为1~N
        MFSet->Parent[i] = -1;//初始化根节点的父节点为-1，表示当前结点为根节点且只有一个元素
    }
    return MFSet;
}
/*查找某元素所在的集合*/
int FindSet(PSet MFSet,int nodei)
{
    if(MFSet->Parent[nodei] < 0)//当前结点的父节点小于0，表示当前结点是根节点
        return nodei;
    else//表示当前结点还有父节点
        return MFSet->Parent[nodei] = FindSet(MFSet,MFSet->Parent[nodei]);//采用路径压缩方法优化
}
/*合并两元素所在的集合*/
void JoinSet(PSet MFSet,int node1_i,int node2_i)
{
    int Root1 = FindSet(MFSet,node1_i);
    int Root2 = FindSet(MFSet,node2_i);
    if(Root1 != Root2)//若两元素在不同的集合中，则合并
    {
        //采用按大小合并方法，元素少的集合并到元素多的集合
        if(MFSet->Parent[Root1] > MFSet->Parent[Root2])
        {
            MFSet->Parent[Root2] += MFSet->Parent[Root1];
            MFSet->Parent[Root1] = Root2;
        }
        else
        {
            MFSet->Parent[Root1] += MFSet->Parent[Root2];
            MFSet->Parent[Root2] = Root1;
        }
    }
    return ;
}
int main()
{
    system("cls");
    int N;
    scanf("%d",&N);getchar();
    PSet MFSet = InitSet(N);
    while(1)
    {
        char op = getchar();//操作符
        if(op == 'S')
            break;
        int c1,c2;
        scanf("%d %d",&c1,&c2);
        getchar();//吃掉换行空格
        if(op == 'I')//连接两个结点
            JoinSet(MFSet,c1 - 1,c2 - 1);
        else if(op == 'C')//检查两个结点是否联通
            printf(FindSet(MFSet,c1 - 1) == FindSet(MFSet,c2 - 1) ? "yes\n" : "no\n");
    }
    if(MFSet->Parent[FindSet(MFSet,0)] == -N)//若全连通
        printf("The network is connected.\n");
    else
    {
        int k = 0;
        for(int i = 0;i < N;i++)
        {
            if(MFSet->Parent[i] < -1)
                k++;
        }
        printf("There are %d components.\n",k);
    }
    return 0;
}