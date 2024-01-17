#include <stdio.h>
#include <stdlib.h>
#define Fail 0
#define Success 1
/*
思路：
    1.依次检查待检测序列的各项，设为元素X。
    2.创建模拟用栈，若X等于栈顶元素，则检测通过，出栈
    3.若不等于栈顶元素，则尝试压入now_top ~ X的元素，若now_top > X，那么表示模拟失败，输出
    4.若压入元素的过程中，栈满且有元素未压入，那么表示模拟失败，输出
    5.若成功压入，则元素X检测通过，出栈
*/

typedef struct Stack *PStack;
struct Stack{
    int data;
    PStack next;
};

/*创建空栈*/
PStack CreateStack(void)
{
    PStack top;
    top = (PStack)malloc(sizeof(struct Stack));
    top->next = NULL;
    return top;
}
/*检测栈是否为空*/
int isemptyStack(PStack top)
{
    return (top->next == NULL);
}
/*将元素压入栈*/
void PushStack(PStack top,int data)
{
    PStack newnode;
    newnode = (PStack)malloc(sizeof(struct Stack));
    newnode->data = data;
    newnode->next = top->next;
    top->next = newnode;
}
/*弹出栈顶元素*/
void PopStack(PStack top)
{
    if(isemptyStack(top))
        return ;
    else
    {
        PStack Banishednode = top->next;
        top->next = Banishednode->next;
        free(Banishednode);
    }
}
/*打印栈*/
void PrintStack(PStack top)
{
    PStack Ptr = top;
    while(Ptr->next != NULL)
    {
        Ptr = Ptr->next;
        printf("%d ",Ptr->data);
    }
    putchar('\n');
}
void PrintOut(int res)
{
    printf(res ? "YES\n" : "NO\n");
    return;
}
/*检测输入序列并输出结果*/
void Testsequence(int* seq,int M,int N)
{
    int top_org = 1;//有序序列可用的第一个元素，1<=top_org<=N
    PStack top = CreateStack();//创建模拟用的空栈
    int Stacknum = 0;//表示栈内元素的个数
    for(int seqcol = 0;seqcol < N;seqcol++)
    {
        if(!isemptyStack(top) && top->next->data == seq[seqcol])//当栈不为空且栈顶元素等于检测值，检测通过
        {
            PopStack(top);
            Stacknum--;
        }
        else //栈为空或栈顶元素不等于检测值
        {
            /*压入top_org~检测值*/
            if(seq[seqcol] < top_org)//无法弹出，失败
            {
                PrintOut(Fail);
                return;
            }
            for(int i = top_org;i <= seq[seqcol];i++)
            {
                if(Stacknum == M)//栈满，无法压入，失败
                {
                    PrintOut(Fail);
                    return;
                }
                PushStack(top,i);
                Stacknum++;
            }
            top_org = (seq[seqcol] + 1 <= N) ? seq[seqcol] + 1 : N;
            PopStack(top);
            Stacknum--;
        }
    }
    PrintOut(Success);
    return;
}

int main()
{
    system("cls");
    int M,N,K;//M:栈的最大长度,N:检测序列的长度,K：待检测序列的数量
    scanf("%d%d%d",&M,&N,&K);
    for(int seqrow = 0;seqrow < K;seqrow++)//用行列角度看待输入
    {
        int seq[N];//待检测序列
        for(int seqcol = 0;seqcol < N;seqcol++)
            scanf("%d",&seq[seqcol]);
        /*检测序列并输出结果*/
        Testsequence(seq,M,N);
    } 
    return 0;
}