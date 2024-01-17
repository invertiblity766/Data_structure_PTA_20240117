#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100
typedef int ElementType;
typedef struct List *PList;

struct List{
    ElementType Data[MAXSIZE]; //线性表的储存空间
    int last;//指示表尾
}; 

/*建立空表*/
PList CreateemptyList(void)
{
    PList PtrL;
    PtrL = (PList)malloc(sizeof(struct List)); //函数malloc返回的是指向一片内存块的指针，类型为void*，根据需要进行强转;
    PtrL->last = -1;
    return PtrL;
}
/*返回线性表的长度*/
int Listlen(PList L)
{
    return L->last + 1;
}
/*返回位序i处的元素*/
ElementType Listfindith(PList L,int i)
{
    if(i < 0 || i > L->last)
    {
        printf("Invaild pos!");
        return -1;
    }
    return L->Data[i];
}
/*查找元素X并返回其位序i*/
int Listfind(PList L,ElementType X)
{
    /*复杂度为O(N)*/
    int cnt = 0;
    while((cnt <= L->last) & (L->Data[cnt] != X))
        cnt++;
    if(cnt <= L->last) return cnt;
    else return -1; //没找到
}
/*在线性表位序i前插入元素X(位序i为非负整数,i = -1表示空表)*/
void insertList(PList L,ElementType X,int i)
{
    /*时间复杂度为O(N)*/
    if(L->last == MAXSIZE - 1)
    {
        printf("List is full!\n");
        return;
    }
    if(i < 0 || i > L->last + 1)
    {
        printf("invaild pos.\n");
        return;
    }
    for(int idx = L->last;idx >= i;idx--)
        L->Data[idx + 1] = L->Data[idx];
    L->last++;
    L->Data[i] = X;
}
/*删除线性表位序i处的元素*/
void deleteList(PList L,int i)
{
    /*时间复杂度为O(N)*/
    if(L->last == -1)
    {
        printf("List is empty!\n");
        return ;
    }
    if(i < 0 || i > L->last)
    {
        printf("invaild pos.\n");
        return ;
    }
    for(int idx = i;idx < L->last;idx++)
        L->Data[idx] = L->Data[idx + 1];
    L->last --;
}
void PrintList(PList L)
{
    for(int i = 0;i <= L->last;i++)
        printf("%d ",L->Data[i]);
    putchar('\n');
}

int main()
{
    system("cls");
    const int arrsize = 10;
    PList L = CreateemptyList();
    for(int i = 0;i < arrsize;i++)
        L->Data[i] = i;
    L->last = arrsize - 1;
    PrintList(L);
    return 0;
}