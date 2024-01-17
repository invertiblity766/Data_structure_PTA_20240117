#include <stdio.h>
#include <stdlib.h>

typedef struct Heapstruct *MinHeap;
struct Heapstruct{
    int size;
    int* data;
    int Capacity;
};
/*创建空堆*/
MinHeap CreateEmptyHeap(int Maxsize)
{
    MinHeap H = (MinHeap)malloc(sizeof(struct Heapstruct));
    H->data = (int*)malloc((Maxsize + 1) * sizeof(int));
    H->size = 0;H->Capacity = Maxsize;
    H->data[0] = -19999;
    return H;
}
/*向上调整算法*/
void procolateUp(MinHeap H,int X)
{
    if(H->size >= H->Capacity)//若堆已满，则无法插入
        return ;
    int i = ++H->size;
    for(;H->data[i / 2] > X && i > 1;i /= 2)//若父节点比其大，则向上调整
        H->data[i] = H->data[i / 2];
    H->data[i] = X;
}
/*向下调整算法*/
void procolateDown(MinHeap H,int Root)
{
    int parent,child;
    int X = H->data[Root];
    for(parent = Root;2 * parent <= H->size;parent = child)
    {
        child = 2 * parent;//首先指向左儿子
        if((child != H->size) && (H->data[child + 1] < H->data[child]))
            child ++;//指向较小的儿子
        if(X < H->data[child])//若根节点比两个儿子都小
            break;
        else//否则用子节点替代父节点
            H->data[parent] = H->data[child];
    }
    H->data[parent] = X;
    return ;
}
/*最小堆建立*/
MinHeap BuildHeap(int N)//N:堆中元素个数
{
    MinHeap H = CreateEmptyHeap(N);
    for(int i = 1;i <= N;i++)
        scanf("%d",&H->data[i]);
    getchar();//吃掉换行空格
    H->size = N;
    for(int i = N / 2;i > 0;i--)
        procolateDown(H,i);
    for(int i = 1;i <= N;i++)
        printf("%d ",H->data[i]);
    putchar('\n');
    return H;
}
/*打印从下标i到根节点的路径*/
void PrintPath(MinHeap H,int i)
{
    int Nodei = i;
    while(Nodei > 0)
    {
        printf("%d",H->data[Nodei]);
        if(Nodei > 1)
            putchar(' ');
        Nodei = Nodei / 2;
    }
    putchar('\n');
    return ;
}
int main()
{
    system("cls");
    int N,M;
    scanf("%d %d",&N,&M);getchar();
    MinHeap H = CreateEmptyHeap(N);
    for(int i = 1;i <= N;i++)
    {
        int node_data;
        scanf("%d",&node_data);
        procolateUp(H,node_data);
    }
    getchar();
    for(int i = 1;i <= M;i++)
    {
        int Nodei;
        scanf("%d",&Nodei);
        PrintPath(H,Nodei);
    }
    return 0;
}