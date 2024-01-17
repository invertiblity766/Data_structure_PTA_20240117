#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;
/*首先实现单链表及其操作集*/
typedef struct node *LinkList;
struct node{
    ElementType data;
    LinkList next;
};//单链表结构定义
LinkList CreateList(void)
{
    //生成空结点并返回
    LinkList L = (LinkList)malloc(sizeof(struct node));
    L->next = NULL;//空结点默认接地，不对data赋值
    return L;
}
LinkList FindList(LinkList L,ElementType data)
{
    //寻找数据域为data的结点，并返回其指针，若没找到返回NULL
    LinkList Ptr;
    for(Ptr = L->next;Ptr != NULL && Ptr->data != data;Ptr = Ptr->next);
    return Ptr;
}
void InsertList(LinkList L,ElementType data)
{
    //用头插法插入数据为data的新节点
    LinkList newnode = CreateList();
    newnode->data = data;
    newnode->next = L->next;
    L->next = newnode;
    return ;
}
void DeleteList(LinkList L,ElementType data)
{
    //寻找数据域为data的结点并删除
    LinkList Ptr;
    for(Ptr = L;Ptr->next != NULL && Ptr->next->data != data;Ptr = Ptr->next);
    LinkList Banishednode = Ptr->next;
    if(Banishednode)
    {
        Ptr->next = Banishednode->next;
        free(Banishednode);
    }
    return ;
}
void DisplayList(LinkList L)
{
    //打印链表
    for(LinkList Ptr = L->next;Ptr != NULL;Ptr = Ptr->next)
        printf("%d ",Ptr->data);
    putchar('\n');
    return ;
}
/*实现开散列及其操作集*/
typedef struct Hashnode *OpenHash;
struct Hashnode{
    int size;//记录哈希表当前有效元素个数
    int Tablesize;//记录哈希表的长度
    LinkList* data;//哈希表数据域
};
int IsPrime(int number)
{
    //判断number是否为质数
    if(number == 2 || number == 3)
        return 1;
    if(number % 2 == 0 || number % 3 == 0)
        return 0;
    int divisor = 6;
    while(divisor * divisor - 2 * divisor + 1 <= number)
    {
        if(number % (divisor - 1) == 0)
            return 0;
        if(number % (divisor + 1) == 0)
            return 0;
        divisor = divisor + 6;
    }
    return 1;
}
int GetNextPrime(int N)
{
    //获取大于N的第一个指数
    int NextPrime;
    for(NextPrime = N;!IsPrime(NextPrime);NextPrime ++);
    return NextPrime;
}
OpenHash CreateHash(int Tablesize)
{
    //申请大于Tablesize的第一个素数大小的开散列
    OpenHash H = (OpenHash)malloc(sizeof(struct Hashnode));
    H->size = 0;H->Tablesize = GetNextPrime(Tablesize);
    H->data = (LinkList*)malloc(H->Tablesize * sizeof(LinkList));
    for(int i = 0;i < H->Tablesize;i++)
        H->data[i] = CreateList();
    return H;
}
void DestroyHash(OpenHash H)
{
    //逐层释放Hash表内存
    for(int i = 0;i < H->Tablesize;i++)
        free(H->data[i]);
    free(H->data);free(H);
    return ;
}
int HashFunc(OpenHash H,ElementType key)
{
    //哈希函数，这里采用除留余数法
    return key % H->Tablesize;
}
LinkList FindHash(OpenHash H,ElementType key)
{
    if(!H->Tablesize)
        return NULL;
    //在哈希表中查找关键字所在结点并返回
    return FindList(H->data[HashFunc(H,key)],key);
}
OpenHash InsertHash(OpenHash H,ElementType key)
{
    //向哈希表内插入新关键字
    LinkList Listno;
    if(Listno = FindList(H->data[HashFunc(H,key)],key) && Listno->data == key)//若有相同的元素，则不插入
        return H;
    if(H->size == H->Tablesize)//装填因子为1，进行再散列操作
    {
        printf("Rehash!\n");
        OpenHash newH = CreateHash(2 * H->Tablesize);//申请更大的Hash表
        for(int i = 0;i < H->Tablesize;i++)//遍历老Hash表，对其中元素再映射到新Hash表
        {
            for(LinkList Ptr = H->data[i]->next;Ptr != NULL;Ptr = Ptr->next)
                newH = InsertHash(newH,Ptr->data);
        }
        OpenHash temp = H;H = newH;
        DestroyHash(temp);
    }
    InsertList(H->data[HashFunc(H,key)],key);
    H->size ++;
    return H;
}
void DeleteHash(OpenHash H,ElementType key)
{
    //删除哈希表中关键字为key的结点
    if(FindHash(H,key))
    {
        DeleteList(H->data[HashFunc(H,key)],key);
        H->size --;
    }
    return ;
}
int main()
{
    system("cls");
    int N;scanf("%d",&N);getchar();
    OpenHash H = CreateHash(5);
    for(int i = 1;i <= N;i++)
    {
        ElementType key;
        scanf("%d",&key);
        H = InsertHash(H,key);
    }
    printf("Hash len = %d\n",H->Tablesize);
    for(int i = 0;i < H->Tablesize;i++)
        DisplayList(H->data[i]);
    printf("---------------------\n");
    getchar();
    while(1)
    {
        ElementType deldata;
        scanf("%d",&deldata);
        if(deldata < 0)
            break;
        DeleteHash(H,deldata);
        for(int i = 0;i < H->Tablesize;i++)
            DisplayList(H->data[i]);
    }
    return 0;
}