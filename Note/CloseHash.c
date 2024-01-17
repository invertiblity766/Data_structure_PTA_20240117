#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;
/*闭散列定义及其操作集*/
typedef struct Hashstruct *CloseHash;
enum STATE {EMPTY,DELETE,EXIST};//定义结点空、被删除、存在三种状态
struct Hashnode{
    enum STATE state;
    ElementType data;
};
struct Hashstruct{
    int size;//记录Hash表当前有效元素个数
    int Tablesize;//记录Hash表长度
    struct Hashnode* data;//Hash表结点数组
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
    //获取大于N的第一个素数
    int NextPrime;
    for(NextPrime = N;!IsPrime(NextPrime);NextPrime ++);
    return NextPrime;
}
CloseHash CreateHash(int Tablesize)
{
    //创建空Hash表并返回
    CloseHash H = (CloseHash)malloc(sizeof(struct Hashstruct));
    H->size = 0;H->Tablesize = GetNextPrime(Tablesize);
    H->data = (struct Hashnode*)malloc(H->Tablesize * sizeof(struct Hashnode));
    for(int i = 0;i < H->Tablesize;i++)
        H->data[i].state = EMPTY;
    return H;
}
void DestroyHash(CloseHash H)
{
    free(H->data);free(H);
    return ;
}
int HashFunc(CloseHash H,ElementType key)
{
    //Hash函数，这里用除余取整法
    return key % H->Tablesize;
}
int ProbeFunc(int i)
{
    //探测增量函数
    return i;
}
int FindHash(CloseHash H,ElementType key)
{
    //查找关键字所在位置，若查找成功返回pos，查找失败返回-1
    if(!H->Tablesize)//表大小为0，查找失败
        return -1;
    int pos = HashFunc(H,key);//计算Hash地址
    if(H->data[pos].state != EXIST)//若初始不为EXIST，则直接查找失败
        return -1;
    for(int i = 1;i <= H->Tablesize;pos = (HashFunc(H,key) + ProbeFunc(i++)) % H->Tablesize)
    {
        if(H->data[pos].state == EXIST)
        {
            if(H->data[pos].data == key)
                return pos;//查找成功
            else
                continue;
        }
        else if(H->data[pos].state == EMPTY)
            return -1;
        else
            continue;
    }
    return -1;//若查找了H->Tablesize次还没找到，则查找失败
}
CloseHash InsertHash(CloseHash H,ElementType key)
{
    //插入结点，以线性探测为例
    if(FindHash(H,key) == -1)//若哈希表中已有关键字，则不插入
    {
        if(H->size * 10 / H->Tablesize > 7)//若装填因子大于0.7，则进行再散列
        {
            printf("Rehash!\n");
            CloseHash newH = CreateHash(2 * H->Tablesize);
            for(int i = 0;i < H->Tablesize;i++)//遍历旧Hash表，将状态为EXIST的结点重新映射到新Hash表中
            {
                if(H->data[i].state == EXIST)
                    newH = InsertHash(newH,H->data[i].data);
            }
            CloseHash temp = H;
            H = newH;newH->size = H->size;
            DestroyHash(temp);
        }
        int pos = HashFunc(H,key);
        for(int i = 1;i <= H->Tablesize;pos = (HashFunc(H,key) + ProbeFunc(i++)) % H->Tablesize)
        {
            if(H->data[pos].state != EXIST)//找到状态为DELETE或EMPTY的结点
            {
                H->data[pos].state = EXIST;
                H->data[pos].data = key;
                break;
            }
        }
        H->size ++;
        return H;
    }
    return H;
}
void DeleteHash(CloseHash H,ElementType key)
{
    //删除哈希表中的关键字
    int pos = FindHash(H,key);
    if(pos >= 0)
    {
        H->data[pos].state = DELETE;
        H->size --;
    }
    return ;

}
void DisplayHash(CloseHash H)
{
    //打印哈希表
    for(int i = 0;i < H->Tablesize;i++)
    {
        if(H->data[i].state == EXIST)
            printf("%d ",H->data[i].data);
        else if(H->data[i].state == EMPTY)
            printf("- ");
        else if(H->data[i].state == DELETE)
            printf(". ");
    }
    putchar('\n');
    return ;
}
int main()
{
    system("cls");
    CloseHash H = CreateHash(1);
    while(1)
    {
        char op = getchar();getchar();//读取操作符，'I'为插入，'D为删除，'S'为结束
        if(op == 'S')
            break;
        int key;scanf("%d",&key);getchar();
        if(op == 'I')
            H = InsertHash(H,key);
        else if(op == 'D')
            DeleteHash(H,key);
        DisplayHash(H);
    }
    return 0;
}