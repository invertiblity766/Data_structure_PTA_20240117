#include <stdio.h>
#include <stdlib.h>

/*思路：
    1.多项式用一个单链表表示，结点按指数递减方式排列
    2.多项式加法：依序比较两多项式链表的各项，指数大的插入结果多项式，指数相等的情况下则两项相加后再插入
    3.多项式乘法：依次相乘两多项式链表的各项，结果按指数递减的方式查找并插入结果多项式(顺序查找，引入指向上一次插入位置的指针，若比其小，那么可以舍弃位置左边部分的查找)
*/

typedef struct Llist *PLlist;
struct Llist{
    int coef;//非零项系数
    int expon;//非零项指数
    PLlist next;//指向下一结点的指针
};

/*创建空结点*/
PLlist CreateLlist(void)
{
    PLlist header;
    header = (PLlist)malloc(sizeof(struct Llist));
    header->next = NULL;
    return header;
}
/*删除链表中的结点*/
void Llistdelete(PLlist Polyhead,PLlist BanishedNode)
{
    PLlist Ptr = Polyhead;
    while(Ptr->next != NULL)
    {
        if(Ptr->next == BanishedNode)
        {
            Ptr->next = BanishedNode->next;
            free(BanishedNode);
            return;
        }
        Ptr = Ptr->next;
    }
}
/*在链表尾部添加结点*/
void Llistappend(PLlist Polytail,int coef,int expon)
{
    /*算法复杂度为O(1)*/
    PLlist newnode;
    newnode = (PLlist)malloc(sizeof(struct Llist));
    newnode->coef = coef;
    newnode->expon = expon;
    newnode->next = NULL; //创建新的尾结点
    PLlist tail = Polytail->next;
    tail->next = newnode;
    Polytail->next = newnode;
    return;
}
/*寻找插入位置并插入(插入在该指针后面或相等情况下相加)*/
void FindandInsertPoly(PLlist header,PLlist newnode,PLlist lastres)
{
    PLlist Ptr = header;
    while(1)
    {
        if(Ptr->next == NULL)
        {
            newnode->next = Ptr->next;
            Ptr->next = newnode;
            lastres->next = newnode;
            return;
        }
        else
        {
            if(newnode->expon == Ptr->next->expon)
            {
                Ptr->next->coef += newnode->coef;
                if(Ptr->next->coef == 0)
                {
                    Llistdelete(header,Ptr->next);
                    lastres->next = Ptr;
                }
                lastres->next = Ptr->next;
                return;
            }
            else if(newnode->expon > Ptr->next->expon)
            {
                newnode->next = Ptr->next;
                Ptr->next = newnode;
                lastres->next = newnode;
                return;
            }
            else
                Ptr = Ptr->next;
        }
    }
}
/*打印链表*/
void PrintLlist(PLlist header)
{
    PLlist Ptr = header;
    if(Ptr->next == NULL)
    {
        printf("0 0\n");
        return;
    }
    while(Ptr->next != NULL)
    {
        Ptr = Ptr->next; //跳过头结点
        printf("%d %d",Ptr->coef,Ptr->expon);
        if(Ptr->next != NULL)
            putchar(' ');
    }
    putchar('\n');
}
/*读取多项式*/
void ReadPoly(PLlist Polytail)
{
    /*算法复杂度为O(N)*/
    int nzeronum;
    scanf("%d",&nzeronum);
    for(int i = 1;i <= nzeronum;i++)
    {
        int coef,expon;
        scanf("%d %d",&coef,&expon);
        Llistappend(Polytail,coef,expon);
    }
    return;
}
/*一元多项式的加法运算*/
PLlist Polysum(PLlist Polyhead1,PLlist Polyhead2)
{
    /*创建结果多项式*/
    PLlist reshead = CreateLlist();
    PLlist restail = CreateLlist();
    restail->next = reshead;
    /*分别遍历两个多项式*/
    PLlist Ptr1 = Polyhead1;
    PLlist Ptr2 = Polyhead2;
    /*特殊情况处理*/
    if(Ptr1->next == NULL && Ptr2->next != NULL)//若多项式1全零且多项式2非全零，结果为多项式2
        return Ptr2;
    if(Ptr2->next == NULL && Ptr1->next != NULL)//若多项式2全零且多项式1非全零，结果为多项式1
        return Ptr1;
    if(Ptr1->next == NULL && Ptr2->next == NULL)//若多项式1，2均为零多项式，返回0，0结点
    {
        Llistappend(restail,0,0);
        return reshead;
    }
    Ptr1 = Ptr1->next;
    Ptr2 = Ptr2->next;//多项式1，2均非全零，移动至第一项开始比较
    while(Ptr1 != NULL || Ptr2 != NULL)//当多项式1，2均运算完毕
    {
        if(Ptr1 != NULL && Ptr2 != NULL) //当多项式1，2都仍有非零项未参与运算
        {
            if(Ptr1->expon > Ptr2->expon)
            {
                Llistappend(restail,Ptr1->coef,Ptr1->expon);
                Ptr1 = Ptr1->next;
            }
            else if(Ptr1->expon == Ptr2->expon)
            {
                if(Ptr1->coef + Ptr2->coef != 0)
                    Llistappend(restail,Ptr1->coef + Ptr2->coef,Ptr1->expon);
                Ptr1 = Ptr1->next;
                Ptr2 = Ptr2->next;
            }
            else
            {
                Llistappend(restail,Ptr2->coef,Ptr2->expon);
                Ptr2 = Ptr2->next;
            }
        }
        else if(Ptr1 == NULL)//当多项式1全部运算完毕而多项式2未运算完毕
        {
            Llistappend(restail,Ptr2->coef,Ptr2->expon);
            Ptr2 = Ptr2->next;
        }
        else//当多项式2全部运算完毕而多项式1未运算完毕
        {
            Llistappend(restail,Ptr1->coef,Ptr1->expon);
            Ptr1 = Ptr1->next;
        }
    }
    return reshead;
}
/*一元多项式的乘法运算*/
PLlist Polymul(PLlist Polyhead1,PLlist Polyhead2)
{
    /*创建结果多项式*/
    PLlist reshead = CreateLlist();
    PLlist restail = CreateLlist();
    restail->next = reshead;
    /*分别遍历两个多项式*/
    PLlist Ptr1 = Polyhead1;
    PLlist Ptr2 = Polyhead2;
    /*处理特殊情况*/
    if(Ptr1->next == NULL || Ptr2->next == NULL)//多项式1，2中有零多项式，结果返回零多项式
    {
        Llistappend(restail,0,0);
        return reshead;
    }
    PLlist lastres = CreateLlist();//指向上一次运算并插入到结果的结点，初始为NULL表示结果多项式为空表
    Ptr1 = Ptr1->next;//多项式1，2均为非零多项式，移动至第一项
    while(Ptr1 != NULL)//当多项式1有项未参与运算
    {
        Ptr2 = Polyhead2->next;//每次运算从头开始乘
        while(Ptr2 != NULL)//当多项式2有项未参与运算
        {
            int coef = Ptr1->coef * Ptr2->coef;
            int expon = Ptr1->expon + Ptr2->expon;
            if(lastres->next == NULL)//没有运算结果，直接插入
            {
                Llistappend(restail,coef,expon);
                lastres->next = restail;
            }
            else//有运算结果
            {
                if(expon == lastres->next->expon)//与上一次运算结果指数相等，系数相加
                {
                    lastres->next->coef += coef;
                    if(lastres->next->coef == 0)
                    {
                        Llistdelete(reshead,lastres->next);
                        lastres->next = reshead->next;
                    }
                }
                else
                {
                    PLlist newnode;
                    newnode = (PLlist)malloc(sizeof(struct Llist));
                    newnode->coef = coef;
                    newnode->expon = expon;
                    FindandInsertPoly((newnode->expon < lastres->next->expon) ? lastres->next : reshead,newnode,lastres);
                }
            }
            Ptr2 = Ptr2->next;
        }
        Ptr1 = Ptr1->next;
    }
    return reshead;
}

int main()
{
    system("cls");
    /*初始化*/
    PLlist Polyhead1 = CreateLlist();//创建多项式1的空表
    PLlist Polytail1 = CreateLlist();//创建多项式1的尾指针
    Polytail1->next = Polyhead1;
    PLlist Polyhead2 = CreateLlist();//创建多项式2的空表
    PLlist Polytail2 = CreateLlist();//创建多项式2的尾指针
    Polytail2->next = Polyhead2;
    /*读取多项式*/
    ReadPoly(Polytail1);//读取多项式1
    ReadPoly(Polytail2);//读取多项式2
    /*进行多项式加法运算*/
    PLlist Sumres = Polysum(Polyhead1,Polyhead2);
    /*进行多项式乘法运算*/
    PLlist Mulres = Polymul(Polyhead1,Polyhead2);
    /*输出运算结果*/
    PrintLlist(Mulres);
    PrintLlist(Sumres);
    return 0;
}