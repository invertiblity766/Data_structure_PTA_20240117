#include <stdio.h>
#include <stdlib.h>

/*Huffman树结点与最小堆定义*/
typedef struct Treenode *PHTree;
struct Treenode{
    char data;
    int weight;
    PHTree lchild;
    PHTree rchild;
};
typedef struct Heapstruct *MinHeap;
struct Heapstruct{
    int size;//表示堆中元素的个数
    int Capacity;//表示堆中元素的最大数量
    PHTree* data;//储存树结点指针的数组
};
/*输入信息处理*/
typedef struct InputInfo *PInfo;
struct InputInfo{
    int N;
    PHTree* data;
};
PInfo GetInputInfo(void)
{
    PInfo I = (PInfo)malloc(sizeof(struct InputInfo));
    scanf("%d",&I->N);getchar();
    I->data = (PHTree*)malloc(I->N * sizeof(PHTree));
    for(int i = 0;i < I->N;i++)
    {
        I->data[i] = (PHTree)malloc(sizeof(struct Treenode));
        I->data[i]->data = getchar();getchar();
        scanf("%d",&I->data[i]->weight);
        I->data[i]->lchild = NULL;
        I->data[i]->rchild = NULL;
    }
    getchar();
    return I;
}
/*最小堆操作集*/
void DisplayHeap(MinHeap H)
{
    //打印堆
    for(int i = 1;i <= H->size;i++)
        printf("%d ",H->data[i]->weight);
    putchar('\n');
    return ;
}
MinHeap CreateHeap(int N)
{
    //建立最大容量为N的空堆
    MinHeap H = (MinHeap)malloc(sizeof(struct Heapstruct));
    H->data = (PHTree*)malloc((N + 1) * sizeof(PHTree));
    for(int i = 0;i <= N;i++)
        H->data[i] = (PHTree)malloc(sizeof(struct Treenode));
    H->Capacity = N;H->size = 0;
    H->data[0]->data = '-';
    H->data[0]->weight = -9999;//哨兵，其值小于所有可能值
    H->data[0]->lchild = NULL;H->data[0]->rchild = NULL;
    return H;
}
void procolateUp(MinHeap H,int root)
{
    //向上调整算法，从root处向上调整为最小堆
    int i;
    PHTree X = H->data[root];//首先将root处的元素保存
    for(i = root;H->data[i / 2]->weight > X->weight;i /= 2)
        H->data[i] = H->data[i / 2];
    H->data[i] = X;
    return ;
}
void procolateDown(MinHeap H,int root)
{
    //向下调整算法，从root处向下调整为最小堆
    int parent,child;
    PHTree X = H->data[root];
    for(parent = root;2 * parent <= H->size;parent = child)
    {
        child = 2 * parent;//首先指向左儿子
        if(child != H->size && H->data[child + 1]->weight < H->data[child]->weight)
            child ++;//指向较小的结点
        if(X->weight < H->data[child]->weight)//比较小的孩子还小，那么找到位置
            break;
        else//否则用子节点替代父节点
            H->data[parent] = H->data[child];
    }
    H->data[parent] = X;
    return ;
}
void InsertHeap(MinHeap H,PHTree T)
{
    //向最小堆插入一个元素
    if(H->size == H->Capacity)//若堆已满，则无法插入
        return ;
    H->data[++H->size] = T;//将元素插入到队尾
    procolateUp(H,H->size);//向上调整为最小堆
    return ;
}
PHTree DeleteHeap(MinHeap H)
{
    //删除栈顶元素并返回
    if(H->size == 0)
        return H->data[0];//若堆为空，则无法删除，返回哨兵
    PHTree X = H->data[1];//取出堆顶元素
    H->data[1] = H->data[H->size--];//用堆尾元素替代堆顶元素
    procolateDown(H,1);//用向下调整算法调整为最小堆
    return X;
}
MinHeap BuildHeap(PInfo I)
{
    //建堆
    MinHeap H = CreateHeap(I->N);
    for(int i = 1;i <= I->N;i++)
    {
        H->data[i]->data = I->data[i - 1]->data;
        H->data[i]->weight = I->data[i - 1]->weight;
        H->data[i]->lchild = NULL;
        H->data[i]->rchild = NULL;
    }
    H->size = I->N;
    for(int i = I->N / 2;i > 0;i--)
        procolateDown(H,i);
    return H;
}
/*构造Huffman树并计算MSL*/
PHTree CreateHuffmanTree(PInfo I)
{
    PHTree T;
    int MSL = 0;//带权路径长度和
    MinHeap H = BuildHeap(I);
    for(int i = 1;i < I->N;i++)
    {
        T = (PHTree)malloc(sizeof(struct Treenode));
        T->lchild = DeleteHeap(H);
        T->rchild = DeleteHeap(H);
        T->data = '-';//表示非叶结点
        T->weight = T->lchild->weight + T->rchild->weight;
        MSL = MSL + T->weight;
        InsertHeap(H,T);
    }
    T = DeleteHeap(H);T->weight = MSL;//MSL储存在Huffman树根节点的权重上
    return T;
}
/*根据字符集建立二叉树*/
PHTree CreateTree(void)
{
    //生成空树并返回
    PHTree T = (PHTree)malloc(sizeof(struct Treenode));
    T->data = '-';T->weight = 0;
    T->lchild = NULL;T->rchild = NULL;
    return T;
}
void MakeJudgeTree(PInfo I,int Optimal_MSL)
{
    //建立判断树并给出是否为正确Huffman编码的结果
    int MSL = 0;
    int IsHuffmanCode =  1;//标志位，用于记录判断结果
    PHTree T = CreateTree();//初始化一空树
    for(int i = 0;i < I->N;i++)
    {
        PHTree Ptr = T;//用一指针初始指向根节点
        char data = getchar();getchar();//读取data
        char code;//用于记录Huffman编码
        int len = 0;//用于记录Huffman编码长度
        while((code = getchar()) != '\n')
        {
            len ++;
            if(Ptr->data != '-')//若插入过程中发现为其它有效结点的后缀，则判定失败
                IsHuffmanCode = 0;
            if(code == '0')//Huffman_code = 0，向左伸展
            {
                if(Ptr->lchild == NULL)
                    Ptr->lchild = CreateTree();
                Ptr = Ptr->lchild;
            }
            else if(code == '1')//Huffman_code = 1,向右伸展
            {
                if(Ptr->rchild == NULL)
                    Ptr->rchild = CreateTree();
                Ptr = Ptr->rchild;
            }
        }
        if(Ptr->data != '-')//待插入的结点已有有效结点
            IsHuffmanCode = 0;
        else
            Ptr->data = data;Ptr->weight = 0;//字符串读取结束，当前Ptr所指位置即为新结点位置
        if(Ptr->lchild != NULL || Ptr->rchild != NULL)//若待插入的结点不是叶子结点，那么判定失败
            IsHuffmanCode = 0;
        MSL = MSL + I->data[i]->weight * len;
    }
    if(Optimal_MSL != MSL)//若无前缀码但MSL不一致判定失败
        IsHuffmanCode = 0;
    printf(IsHuffmanCode ? "Yes\n" : "No\n");
    return ;
}

int main()
{
    system("cls");
    PInfo I = GetInputInfo();
    PHTree T = CreateHuffmanTree(I);
    int M;scanf("%d",&M);getchar();
    for(int i = 1;i <= M;i++)
        MakeJudgeTree(I,T->weight);
    return 0;
}