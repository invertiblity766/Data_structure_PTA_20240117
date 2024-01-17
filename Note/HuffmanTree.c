#include <stdio.h>
#include <stdlib.h>

/*Huffman树结点与最小堆定义*/
typedef struct Treenode *PHTree;
struct Treenode{
    int weight;
    PHTree lchild;
    PHTree rchild;
};
typedef struct Heapstruct *MinHeap;
struct Heapstruct{
    int size;//堆目前元素的个数
    int Capacity;//堆的最大容量
    PHTree* data;//存放结点权重的数组
};

/*建立容量为N的空堆*/
MinHeap CreateHeap(int N)
{
    MinHeap H = (MinHeap)malloc(sizeof(struct Heapstruct));
    H->data = (PHTree*)malloc((N + 1) * sizeof(PHTree));//先申请指针大小的数组空间
    for(int i = 0;i <= N;i++)
        H->data[i] = (PHTree)malloc(sizeof(struct Treenode));//再为每个指针申请结构体空间
    H->Capacity = N;H->size = 0;
    H->data[0]->weight = -9999;//哨兵，类似于头结点方便操作
    H->data[0]->lchild = NULL;H->data[0]->rchild = NULL;
    return H;
}
/*向上调整算法*/
void procolateUp(MinHeap H,int root)
{
    int i = root;
    PHTree X = H->data[root];
    for(;H->data[i / 2]->weight > X->weight;i /= 2)//反复与父节点比较，哨兵保证了比较最多到根节点
        H->data[i] = H->data[i / 2];
    H->data[i] = X;
}
/*向下调整算法*/
void procolateDown(MinHeap H,int root)
{
    int parent,child;
    PHTree X = H->data[root];
    for(parent = root;2 * parent <= H->size;parent = child)
    {
        child = 2 * parent;//首先指向左儿子
        if((child != H->size) && (H->data[child + 1]->weight < H->data[child]->weight))
            child ++;//指向较小的儿子
        if(X->weight <= H->data[child]->weight)//若比较小的儿子还小，则找到位置
            break;
        else
            H->data[parent] = H->data[child];
    }
    H->data[parent] = X;
}
/*最小堆的插入*/
void InsertHeap(MinHeap H,PHTree X)
{
    if(H->size == H->Capacity)//若堆已满，则无法插入
        return ;
    H->data[++H->size] = X;//若没满，则插入到队尾
    procolateUp(H,H->size);//使用向上调整算法调整为最小堆
}
/*最小堆的删除*/
PHTree DeleteHeap(MinHeap H)
{
    if(H->size == 0)//若堆为空，则返回哨兵值
        return H->data[0];
    PHTree Minval = H->data[1];//取出堆顶元素(最小值)
    H->data[1] = H->data[H->size--];//用堆尾元素取代堆顶元素
    procolateDown(H,1);//用向下调整算法调整为最小堆
    return Minval;
}
/*建堆*/
MinHeap BuildHeap(int N)
{
    MinHeap H = CreateHeap(N);//首先建立容量为N的空堆
    for(int i = 1;i <= N;i++)
    {
        //scanf("%d",&H->data[i]->weight);//读入各结点权值信息
        H->data[i]->weight = i;
        H->data[i]->lchild = NULL;
        H->data[i]->rchild = NULL;//初始化结点左右儿子为空
    }
    H->size = N;
    for(int i = N / 2;i > 0;i--)
        procolateDown(H,i);//使用向下调整算法从后到前调整
    return H;
}
/*构造Huffman树*/
PHTree CreateHuffmanTree(void)
{
    PHTree T;
    int N;scanf("%d",&N);getchar();
    MinHeap H = BuildHeap(N);//读取叶结点的权值信息并建立最小堆
    for(int i = 1;i < N;i++)//进行H->size - 1次调整
    {
        T = (PHTree)malloc(sizeof(struct Treenode));
        T->lchild = DeleteHeap(H);
        T->rchild = DeleteHeap(H);//取出权值最小的两个结点作为左右子树
        T->weight = T->lchild->weight + T->rchild->weight;
        InsertHeap(H,T);
    }
    T = DeleteHeap(H);//上述循环最后将建成的Huffman树压入堆中，需要重新出堆
    return T;
}
/*先序遍历*/
void PreorderTraversal(PHTree T)
{
    if(T)
    {
        printf("%d ",T->weight);
        PreorderTraversal(T->lchild);
        PreorderTraversal(T->rchild);
    }
    return ;
}
/*中序遍历*/
void InorderTraversal(PHTree T)
{
    if(T)
    {
        InorderTraversal(T->lchild);
        printf("%d ",T->weight);
        InorderTraversal(T->rchild);
    }
    return ;
}
int main()
{
    system("cls");
    PHTree T = CreateHuffmanTree();
    printf("HuffmanTree Created!\n");
    PreorderTraversal(T);putchar('\n');
    InorderTraversal(T);putchar('\n');
    return 0;
}