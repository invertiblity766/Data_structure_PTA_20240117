#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;
typedef struct Lnode *PLnode;//pointer of Linked node

struct Lnode{
    ElementType data;
    PLnode next; 
};
/*创建空表(链表头节点)*/
PLnode createemptyLlist(void)
{
    PLnode newnode;
    newnode = (PLnode)malloc(sizeof(struct Lnode));
    newnode->next = NULL;
    return newnode;
}
/*返回链表的长度*/
int Llistlen(PLnode header)
{
    /*显然，时间复杂度为O(N)，但此操作不常用*/
    int cnt = 0;
    PLnode Ptr = header;
    while(Ptr->next != NULL)//检查后继节点是否为空
    {
        cnt ++;
        Ptr = Ptr->next;
    }
    return cnt;
}
/*查找链表位序为元素并返回其指针(i从0开始，i=-1表示头指针)*/
PLnode FindithLlist(PLnode header,int i)
{
    PLnode Ptr = header;
    for(int pos = -1;pos < i;pos++)
    {
        if(Ptr->next == NULL) //先检查后继节点是否为空
        {
            printf("Invaild pos.\n");
            return NULL;
        }
        Ptr = Ptr -> next;
    }
    return Ptr;//找到位序为i的元素，返回其指针
}
/*查找链表中元素X并返回其指针*/
PLnode FindXLlist(PLnode header,ElementType X)
{
    PLnode Ptr = header;
    while(Ptr->next != NULL) //排除头结点
    {
        Ptr = Ptr->next;
        if(Ptr->data == X)
            return Ptr;
    }
    return NULL;
}
/*在链表位序i的结点后插入新元素X*/
void InsertLlist(PLnode header,ElementType X,int i)
{
    /*若给定前继结点，复杂度为O(1)，否则复杂度为O(N)*/
    PLnode newnode;//生成待插入的新结点
    newnode = (PLnode)malloc(sizeof(struct Lnode));
    newnode->data = X;
    PLnode Ptr = FindithLlist(header,i);//寻找位序为i的结点
    newnode->next = Ptr->next;//插入结点
    Ptr->next = newnode;
    return ;
}
/*删除链表位序为i的结点*/
void DeleteLlist(PLnode header,int i)
{
    /*若给定前继结点，复杂度为O(1)，否则复杂度为O(N)*/
    PLnode Ptr = FindithLlist(header,i - 1);//寻找要删除的结点的前继结点
    if(Ptr -> next == NULL)//要删除的是空结点
        return;
    PLnode Banishednode = Ptr->next; //要删除的结点
    Ptr->next = Banishednode->next;
    free(Banishednode);
}
/*打印链表*/
void PrintLlist(PLnode header)
{
    PLnode Ptr = header;
    while(Ptr->next !=  NULL) //排除头结点
    {
        Ptr = Ptr->next;
        printf("%d ",Ptr->data);
    }
    putchar('\n');
    return;
}

int main()
{
    system("cls");
    const int arrsize = 15;
    PLnode header = createemptyLlist();
    for(int i = 0;i < arrsize;i++)
        InsertLlist(header,i,i-1);
    PrintLlist(header);
    return 0;
}