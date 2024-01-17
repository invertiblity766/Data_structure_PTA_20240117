#include <stdio.h>
#include <stdlib.h>
#define QQnumber_size 11
#define Password_size 17

/*首先实现单链表及其操作集*/
typedef struct node *LinkList;
struct node{
    char* Password;//用户密码
    char* QQnumber;//用户QQ账号
    LinkList next;
};//单链表结构定义
LinkList CreateList(void)
{
    //生成空结点并返回
    LinkList L = (LinkList)malloc(sizeof(struct node));
    L->QQnumber = (char*)malloc(QQnumber_size * sizeof(char));//申请储存QQ号码的空间
    L->Password = (char*)malloc(Password_size * sizeof(char));//申请储存密码的空间
    L->next = NULL;//空结点默认接地，不对data赋值
    return L;
}
int Cmpstring(char* str1,char* str2)
{
    //比较两字符串，不相等返回0，相等返回1
    char* Ptr1 = str1;char* Ptr2 = str2;
    while(1)
    {
        if(*Ptr1 != *Ptr2)//若对应位置不相等，则返回0
            return 0;
        else//若对应位置相等
        {
            if(*Ptr1 == '\0' && *Ptr2 == '\0')//若同时到达字符串尾，则表示字符串相等，返回1
                return 1;
            else//否则移动到下一个位置继续比较
            {
                Ptr1++;
                Ptr2++;
            }
        }
    }
}
void CopyString(char* target,char* str)
{
    //复制str到target中
    char* Ptr1 = target;char* Ptr2 = str;
    while(1)
    {
        *Ptr1 = *Ptr2;
        if(*Ptr2 == '\0')
            return ;
        Ptr1 = Ptr1 + 1;
        Ptr2 = Ptr2 + 1;
    }
}
LinkList FindList(LinkList L,char* QQnumber)
{
    //寻找数据域为QQnumber的结点，并返回其指针，若没找到返回NULL
    LinkList Ptr = L->next;
    for(Ptr = L->next;Ptr != NULL && !Cmpstring(Ptr->QQnumber,QQnumber);Ptr = Ptr->next);
    return Ptr;
}
void InsertList(LinkList L,char* QQnumber,char* Password)
{
    //用头插法插入数据为data的新节点
    LinkList newnode = CreateList();
    CopyString(newnode->QQnumber,QQnumber);
    CopyString(newnode->Password,Password);
    newnode->next = L->next;
    L->next = newnode;
    return ;
}
void DisplayList(LinkList L)
{
    //打印链表
    for(LinkList Ptr = L->next;Ptr != NULL;Ptr = Ptr->next)
        printf("QQnumber:%s,Password:%s|",Ptr->QQnumber,Ptr->Password);
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
    if(number % 2 == 0 || number % 3 == 0 || number == 1)
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
unsigned int HashFunc(OpenHash H,char* str)
{
    //哈希函数，这里采用除留余数法
    unsigned int hashval = 1315423911;
    for(char* Ptr = str;*(Ptr) != '\0';Ptr ++)
        hashval ^= ((hashval << 5) + *Ptr + (hashval >> 2));
    return hashval % H->Tablesize;
}
LinkList FindHash(OpenHash H,char* QQnumber)
{
    if(!H->Tablesize)
        return NULL;
    //在哈希表中查找关键字所在结点并返回
    return FindList(H->data[HashFunc(H,QQnumber)],QQnumber);
}
OpenHash InsertHash(OpenHash H,char* QQnumber,char* Password)
{
    //向哈希表内插入新关键字
    LinkList Listno = FindHash(H,QQnumber);
    if(Listno && Cmpstring(Listno->QQnumber,QQnumber))//若有相同的元素，则不插入
        return H;
    InsertList(H->data[HashFunc(H,QQnumber)],QQnumber,Password);
    H->size ++;
    return H;
}
int main()
{
    system("cls");
    int N;scanf("%d",&N);getchar();
    OpenHash H = CreateHash(N + 1);
    for(int i = 1;i <= N;i++)
    {
        char op = getchar();getchar();//获得操作符并吃掉后面的空格
        char QQnumber[QQnumber_size];
        char Password[Password_size];
        scanf("%s %s",QQnumber,Password);getchar();
        if(op == 'L')//登录指令
        {
            LinkList Ptr = FindHash(H,QQnumber);//根据QQnumber查找哈希表
            if(Ptr == NULL)//若没找到，则输出错误信息
                printf("ERROR: Not Exist\n");
            else//若找到
            {
                if(Cmpstring(Ptr->Password,Password))//若密码也对上，则登陆成功
                    printf("Login: OK\n");
                else//若密码对不上，则显示密码错误
                    printf("ERROR: Wrong PW\n");
            }
        }
        else if(op == 'N')//创建新用户指令
        {
            LinkList Ptr = FindHash(H,QQnumber);//查找哈希表内是否已经有申请的QQ号了
            if(Ptr != NULL)//若已有，输出错误信息
                printf("ERROR: Exist\n");
            else//若没有，则创建新用户
            {
                H = InsertHash(H,QQnumber,Password);
                printf("New: OK\n");
            }
        }
    }
    return 0;
}