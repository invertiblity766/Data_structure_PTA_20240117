#include <stdio.h>
#include <stdlib.h>
/*此例程实现最大堆操作集，最小堆的操作类似*/
typedef int ElementType;
typedef struct Heapstruct *MaxHeap;
struct Heapstruct{
    int size;//表示当前存放元素的数量
    int Capacity;//表示堆的容量
    ElementType* data;//存放数据的数组
};
/*创建最大容量为Maxsize的空堆*/
MaxHeap CreateEmptyHeap(int Maxsize)
{
    MaxHeap H = (MaxHeap)malloc(sizeof(struct Heapstruct));//申请堆结构空间
    H->data = (ElementType*)malloc((Maxsize + 1) * sizeof(ElementType));//申请一个Maxsize + 1大小的数组用于存放数据(因为堆从1开始储存数据)
    H->size = 0;H->Capacity = Maxsize;
    H->data[0] = 9999;//类似于链表头结点，在data[0]处设置一个大于任何可能节点的值称为“哨兵”，以方便后续操作
    return H;
}
/*判断堆是否已满*/
int IsFullHeap(MaxHeap H)
{
    return (H->size == H->Capacity);
}
/*向最大堆插入元素X*/
void InsertHeap(MaxHeap H,ElementType X)
{
    /*算法复杂度为O(logN)*/
    if(IsFullHeap(H))//若堆已满，则无法插入
        return ;
    int i = ++H->size;//首先指向数组尾
    for(;H->data[i / 2] < X;i /= 2)//判断父节点是否小于X，若小于则将父节点移至子节点
        H->data[i] = H->data[i / 2];//“哨兵”保证了至多比较到根节点就结束
    H->data[i] = X;//将元素X插入合适的位置
}
/*判断堆是否为空*/
int IsEmptyHeap(MaxHeap H)
{
    return (H->size == 0);
}
/*向下调整算法*/
void procolateDown(MaxHeap H,int root)
{
    int parent,child;
    ElementType X = H->data[root];//记录根节点值
    for(parent = root;2 * parent <= H->size;parent = child)
    {
        child = 2 * parent;//首先指向左儿子
        if((child != H->size) && (H->data[child + 1] > H->data[child]))
            child++;//指向较大的儿子
        if(X >= H->data[child])//若根节点值还大于两个儿子，则说明找到合适位置
            break;
        else//否则，用子节点替代根节点
            H->data[parent] = H->data[child];
    }
    H->data[parent] = X;
}
/*删除最大堆顶元素*/
ElementType DeleteHeap(MaxHeap H)
{
    /*算法复杂度为O(logN)*/
    if(IsEmptyHeap(H))//若堆已空，则无法删除堆顶元素
        return ;
    ElementType X = H->data[1];//取出堆顶元素
    /*用最大堆尾部元素从上到下过滤元素*/
    H->data[1] = H->data[H->size --];//将堆尾元素放到根节点
    procolateDown(H,1);//使用向下调整算法调整为最大堆
    return X;//返回堆顶元素    
}
/*根据N个关键字建最大堆*/
MaxHeap BuildMaxHeap(int* arr,int N)
{
    /*算法复杂度为O(N)*/
    MaxHeap H = CreateEmptyHeap(N);//建立容量为N的堆
    for(int i = 1;i <= N;i++)//将N个关键字首先存入，建立无序堆
        H->data[i] = arr[i - 1];
    H->size = N;//此时堆中有N个元素
    for(int i = N / 2;i > 0;i--)
        procolateDown(H,i);//使用向下调整算法调整为最大堆
    return H;
}
int main()
{
    system("cls");
    const int N = 10;
    ElementType arr[] = {3,1,5,4,2,8,7,6,9,0};
    MaxHeap H = BuildMaxHeap(arr,N);
    while(!IsEmptyHeap(H))
        printf("%d ",DeleteHeap(H));//建堆+依次删除 = 堆排序，复杂度为O(NlogN)
    return 0;
}