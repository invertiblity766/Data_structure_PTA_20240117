#include <stdio.h>
#include <stdlib.h>
#define Quick_sort

/*本例程中对整型变量进行排序*/
void Displayarr(int* arr,int N)
{
    //打印数组数组
    for(int i = 0;i < N;i++)
    {
        printf("%d",arr[i]);
        if(i < N - 1)
            putchar(' ');
    }
    putchar('\n');
    return ;
}
void Swap(int* arr,int a1,int a2)
{
    //交换数组的a1,a2项
    int temp = arr[a1];
    arr[a1] = arr[a2];
    arr[a2] = temp;
    return;
}
#ifdef Bubble_sort
void Bubblesort(int* arr,int arrsize)
{
    /*冒泡排序，时间复杂度O(N^2)*/
    for(int i = 0;i < arrsize - 1;i++)
    {
        //共进行arrsize - 1次，每次排序完后最大/最小的元素会位于遍历序列的尾端
        int Is_sorted = 1;
        for(int j = 0;j < arrsize - 1 - i;j++)
        {
            if(arr[j] > arr[j + 1])
            {
                Swap(arr,j,j+1);
                Is_sorted = 0;
            }
        }
        if(Is_sorted)//若一轮排序后没有发生交换，则表示已经有序了
            break; 
    }
    return ;
}
#endif
#ifdef Insert_sort
void Insertsort(int* arr,int arrsize)
{
    /*插入排序，时间复杂度O(N^2)*/
    int i,j;
    for(i = 1;i < arrsize;i++)//从第2个元素开始依次插入前面的序列
    {
        int temp = arr[i];
        for(j = i;j > 0 && temp < arr[j - 1];j--)
            arr[j] = arr[j - 1];//依次让出空位
        arr[j] = temp;
    }
}
#endif
#ifdef Shell_sort
void Shellsort(int* arr,int arrsize)
{
    //原始希尔排序，令Dm = [N / 2],D_k = [D_k+1 / 2]
    int D,i,j;
    for(D = arrsize / 2;D >= 1;D /= 2)
    {
        //对序列做D间隔的插入排序
        for(i = D;i < arrsize;i++)
        {
            int temp = arr[i];
            for(j = i;j >= D && temp < arr[j - D];j -= D)
                arr[j] = arr[j - D];
            arr[j] = temp;
        }
    }
    return ;
    /*原始希尔排序由于相邻增量元素不互质，最坏时间复杂度有O(N^2)*/  
}
//Hibbard增量序列：Dk = 2^k - 1，最坏情况为O(N^1.5)，平均时间复杂度为O(N^1.25)
//Sedgewick增量序列：最坏情况为O(N^{7/6})，平均时间复杂度为O(N^{4/3})
#endif
#ifdef Quick_sort
int SelectKeyval(int * arr,int left,int right)
{
    /*选取left,mid,right中的中间值*/
    int mid = (left + right) / 2;
    if(arr[mid] > arr[right]) //arr[mid] <= arr[right]
        Swap(arr,mid,right);
    if(arr[left] > arr[right])//arr[left] <= arr[right]
        Swap(arr,left,right);
    if(arr[mid] > arr[left])//arr[left] >= arr[mid] 
        Swap(arr,mid,left);
    return arr[left]; //arr[mid] <= arr[left] <= arr[right]
}
void Quicksort_recursive(int* arr,int left,int right)
{
    /*快速排序，平均时间复杂度O(NlogN)*/
    if(left > right)//若left > right，则不进行快速排序
        return;
    int left_ptr = left; //左指针
    int right_ptr = right;//右指针
    int keyval = SelectKeyval(arr,left,right);//使用三数取中法选取基准值，这样算法复杂度才是O(NlogN)
    while(left_ptr < right_ptr)
    {
        /*右指针寻找第一个比基准数小的位置*/
        while((left_ptr < right_ptr) && (arr[right_ptr] >= keyval))
            right_ptr--;
        /*左指针寻找第一个比基准数大的位置*/
        while((left_ptr < right_ptr) && (arr[left_ptr] <= keyval))
            left_ptr++;
        if(left_ptr == right_ptr)
        {
            Swap(arr,left,left_ptr);
            break; //找到基准值正确位置，交换正确位置与基准值所在位置元素并退出
        }
        else
            Swap(arr,left_ptr,right_ptr);//未找到位置，交换左右指针处的元素
    }
    Quicksort_recursive(arr,left,left_ptr - 1); //对左侧序列使用快速排序
    Quicksort_recursive(arr,right_ptr + 1,right);//对右侧序列使用快速排序
    return;
}
void Quicksort(int* arr,int arrsize)
{
    Quicksort_recursive(arr,0,arrsize - 1);
}
#endif
#ifdef Heap_sort
typedef struct Heapstruct *MaxHeap;
struct Heapstruct{
    int size;//表示当前存放元素的数量
    int Capacity;//表示堆的容量
    int* data;//存放数据的数组
};
/*创建最大容量为Maxsize的空堆*/
MaxHeap CreateEmptyHeap(int Maxsize)
{
    MaxHeap H = (MaxHeap)malloc(sizeof(struct Heapstruct));//申请堆结构空间
    H->data = (int*)malloc((Maxsize + 1) * sizeof(int));//申请一个Maxsize + 1大小的数组用于存放数据(因为堆从1开始储存数据)
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
void InsertHeap(MaxHeap H,int X)
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
    int X = H->data[root];//记录根节点值
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
int DeleteHeap(MaxHeap H)
{
    /*算法复杂度为O(logN)*/
    if(IsEmptyHeap(H))//若堆已空，则无法删除堆顶元素
        return ;
    int X = H->data[1];//取出堆顶元素
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
void Heapsort(int* arr,int arrsize)
{
    MaxHeap H = BuildMaxHeap(arr,arrsize);//建堆，平均时间复杂度O(N)
    for(int i = arrsize - 1;i >= 0;i--)//依次删除堆顶元素，平均时间复杂度O(NlogN)
        arr[i] = DeleteHeap(H);
    return ;
}
#endif
#ifdef Merge_sort
void Merge(int* arr,int* resarr,int Left,int Right,int Rightend)//将两个有序子序列进行归并
{
    int Leftend = Right - 1;//左子序列为[Left,Leftend]，右子序列为[Right,Rightend]
    int pos = Left;//存放到resarr数组的初始位置
    int Numelements = Rightend - Left + 1;
    while(Left <= Leftend && Right <= Rightend)//当有一个子序列空了就退出
        resarr[pos++] = (arr[Left] <= arr[Right]) ? arr[Left++] : arr[Right++];
    while(Left <= Leftend)//将左序列剩余元素放到resarr中
        resarr[pos++] = arr[Left++];
    while(Right <= Rightend)//将右序列剩余元素放到resarr中
        resarr[pos++] = arr[Right++];
    for(int i = 1;i <= Numelements;i++,Rightend--)
        arr[Rightend] = resarr[Rightend];
    return ;
}
void Merge_sort_recursive(int* arr,int* resarr,int Left,int Rightend)
{
    //递归归并算法,算法时间复杂度为O(NlogN)
    if(Left < Rightend)
    {
        int mid = (Left + Rightend) / 2;
        Merge_sort_recursive(arr,resarr,Left,mid);
        Merge_sort_recursive(arr,resarr,mid + 1,Rightend);
        Merge(arr,resarr,Left,mid + 1,Rightend);
    }
}
void Mergesort_recursive(int* arr,int arrsize)
{
    int* resarr = (int*)malloc(arrsize * sizeof(int));
    Merge_sort_recursive(arr,resarr,0,arrsize - 1);
    free(resarr);
    return ;
}
void Merge_pass(int* arr,int* resarr,int arrsize,int seqlen)//seqlen是当前要归并的子序列的长度
{
    int i;
    for(i = 0;i <= arrsize - 2 * seqlen;i = i + 2 * seqlen)//依次合并成对的子序列
        Merge(arr,resarr,i,i+seqlen,i + 2 * seqlen - 1);
    if(i + seqlen < arrsize)//最后还有两个长度不等的子列
        Merge(arr,resarr,i,i + seqlen,arrsize - 1);
    else//只有一个子列,则将剩下的子列直接放到arr中
    {
        for(int j = i;j < arrsize;j++) 
            resarr[j] = arr[j];
    }
    return ;
}
void Mergesort_nonrecursive(int* arr,int arrsize)
{
    int seqlen = 1;//初始化子序列长度为1
    int* resarr = (int*)malloc(arrsize * sizeof(int));
    while(seqlen < arrsize)
    {
        Merge_pass(arr,resarr,arrsize,seqlen);
        seqlen = seqlen * 2;
        Merge_pass(arr,resarr,arrsize,seqlen);
        seqlen = seqlen * 2;
    }
    free(resarr);
    return ;
}
#endif
int main()
{
    system("cls");
    int arrsize = 10;
    int arr[] = {3,1,2,5,4,6,7,9,0,8};
    Quicksort(arr,arrsize);
    Displayarr(arr,arrsize);
    return 0;
}