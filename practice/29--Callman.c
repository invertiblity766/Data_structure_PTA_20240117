#include <stdio.h>
#include <stdlib.h>
#define Phonenum_size 12
/*首先实现单链表及其操作集*/
typedef struct node *LinkList;
struct node{
    int cnt;//记录通话次数
    char* phonenum;//记录电话号码,留一位给'\0'
    LinkList next;
};//单链表结构定义
LinkList CreateList(void)
{
    //生成空结点并返回
    LinkList L = (LinkList)malloc(sizeof(struct node));
    L->cnt = 0;//初始化空结点的通话次数为0
    L->phonenum = (char*)malloc(Phonenum_size * sizeof(char));
    L->next = NULL;//空结点默认接地，不对data赋值
    return L;
}
int CmpPhonenum(char* phonenum_1,char* phonenum_2)
{
    //比较两个电话号码是否一致
    for(int i = 0;i < Phonenum_size - 1;i++)
    {
        if(phonenum_1[i] != phonenum_2[i])
            return 0;
    }
    return 1;
}
void CopyPhonenum(char* phonenum1,char* phonenum2)
{
    //复制电话号码
    for(int i = 0;i < Phonenum_size - 1;i++)
        phonenum1[i] = phonenum2[i];
    phonenum1[Phonenum_size - 1] = '\0';
    return ;
}
int SmallerPhonenum(char* phonenum1,char* phonenum2)
{
    //比较两个电话号码哪个小,返回0表示phonenum2小，返回1表示phonenum1小
    for(int i = 0;i < Phonenum_size - 1;i++)
    {
        int sub = phonenum1[i] - phonenum2[i];
        if(sub == 0)
            continue;
        if(sub < 0)
            return 1;
        if(sub > 0)
            return 0;
    }
    return 1;//两者相等,返回0或1都行
}
LinkList FindList(LinkList L,char* phonenum)
{
    //寻找电话号码为phonenum的结点，并返回其指针，若没找到返回NULL
    LinkList Ptr;
    for(Ptr = L->next;Ptr != NULL && !CmpPhonenum(Ptr->phonenum,phonenum);Ptr = Ptr->next);
    return Ptr;
}
void InsertList(LinkList L,char* phonenum,int cnt)
{
    //用头插法插入电话号码为phonenum的新节点
    LinkList newnode = CreateList();
    newnode->cnt = cnt;
    CopyPhonenum(newnode->phonenum,phonenum);
    LinkList Ptr;
    for(Ptr = L;Ptr->next != NULL && cnt < Ptr->next->cnt;Ptr = Ptr->next);
    newnode->next = Ptr->next;
    Ptr->next = newnode;
    return ;
}
void DeleteList(LinkList L,char* phonenum)
{
    //寻找电话号码为phonenum的结点并删除
    LinkList Ptr;
    for(Ptr = L;Ptr->next != NULL && !CmpPhonenum(Ptr->next->phonenum,phonenum);Ptr = Ptr->next);
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
        printf("Phonenum = %s,cnt = %d |",Ptr->phonenum,Ptr->cnt);
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
int HashFunc(OpenHash H,char* phonenum)
{
    //哈希函数，这里取电话号码最后五位数做除留余数法
    int hashval = phonenum[6];
    for(int i = 7;i <= 10;i++)
        hashval = (hashval << 5) + phonenum[i];
    return hashval % H->Tablesize;
}
LinkList FindHash(OpenHash H,char* phonenum)
{
    if(!H->Tablesize)//若Hash表为空，则返回空
        return NULL;
    //在哈希表中查找关键字所在结点并返回
    return FindList(H->data[HashFunc(H,phonenum)],phonenum);
}
OpenHash InsertHash(OpenHash H,char* phonenum)
{
    //向哈希表内插入新关键字
    LinkList Listno = FindList(H->data[HashFunc(H,phonenum)],phonenum);
    if(Listno != NULL && CmpPhonenum(Listno->phonenum,phonenum))//若有相同的元素，则通话次数+1
    {
        Listno->cnt = Listno->cnt + 1;
        return H;
    }
    if(H->size == H->Tablesize)//装填因子为1，进行再散列操作
    {
        OpenHash newH = CreateHash(2 * H->Tablesize);//申请更大的Hash表
        for(int i = 0;i < H->Tablesize;i++)//遍历老Hash表，对其中元素再映射到新Hash表
        {
            for(LinkList Ptr = H->data[i]->next;Ptr != NULL;Ptr = Ptr->next)
                InsertList(newH->data[HashFunc(newH,Ptr->phonenum)],Ptr->phonenum,Ptr->cnt);
        }
        OpenHash temp = H;H = newH;
        DestroyHash(temp);
    }
    InsertList(H->data[HashFunc(H,phonenum)],phonenum,1);
    H->size ++;
    return H;
}
void DeleteHash(OpenHash H,char* phonenum)
{
    //删除哈希表中关键字为key的结点
    if(FindHash(H,phonenum))
    {
        DeleteList(H->data[HashFunc(H,phonenum)],phonenum);
        H->size --;
    }
    return ;
}
void DisplayPhonenum(char* phonenum)
{
    for(int i = 0;i < Phonenum_size - 1;i++)
        putchar(phonenum[i]);
    return ;
}
int main()
{
    system("cls");  
    int N;scanf("%d",&N);getchar();
    OpenHash H = CreateHash(N);
    for(int i = 1;i <= N;i++)
    {
        char phonenum1[Phonenum_size];
        char phonenum2[Phonenum_size];
        scanf("%s %s",phonenum1,phonenum2);getchar();
        H = InsertHash(H,phonenum1);
        H = InsertHash(H,phonenum2);
    }
    int maxcnt = 0;//记录最大次数
    int Callman_num = 0;//记录电话狂人数量，若大于1需要另外输出
    char Callman_phonenum[12] = "00000000000\0";//记录电话狂人最小的电话号码,初始化为全零
    //遍历哈希表，寻找电话狂人
    for(int i = 0;i < H->Tablesize;i++)
    {
        for(LinkList Ptr = H->data[i]->next;Ptr != NULL;Ptr = Ptr->next)
        {
            if(Ptr->cnt < maxcnt)
                continue;
            else if(Ptr->cnt == maxcnt)//若有通话次数与最大通话次数并列的单位，记录之
            {
                Callman_num ++;//电话狂人数量增加
                if(SmallerPhonenum(Ptr->phonenum,Callman_phonenum))//若当前电话比记录的电话还小，则记录更小的电话
                    CopyPhonenum(Callman_phonenum,Ptr->phonenum);
            }
            else if(Ptr->cnt > maxcnt)//若有通话次数比最大通话次数还大的单位，替换之
            {
                Callman_num = 1;
                maxcnt = Ptr->cnt;
                CopyPhonenum(Callman_phonenum,Ptr->phonenum);
            }
        }
    }
    //输出电话狂人电话号码与通话次数，若并列的电话狂人数量大于1，则再输出电话狂人数量
    DisplayPhonenum(Callman_phonenum);
    printf(" %d",maxcnt);
    if(Callman_num > 1)
        printf(" %d",Callman_num);
    putchar('\n');
    return 0;
}