#include <stdio.h>
#include <stdlib.h>
#define solution1

#ifdef solution1
/*思路：
    1.建立结构体数组，并依序读入各结点
    2.使用快速排序算法对读入的数组进行排序
    3.从头结点开始，之后每一结点的下标由上一结点的addr_next与本结点的addr相等决定，采用二分查找算法查找每一结点下表完成结点数组重组
    4.对每个长度为K的子序列进行翻转，然后遍历重组后数组调整每项addr_next为下一项地址(最后一项指向-1)，并输出
*/
#define ADDR_SIZE 5

typedef struct
{
    int data;
    int addr;
    int addr_next;
}Node;//结点结构体定义
/*对结点数组两元素进行交换*/
void Swapnodes(Node* nodes,int i_node1,int i_node2)
{
    Node temp = nodes[i_node1];
    nodes[i_node1] = nodes[i_node2];
    nodes[i_node2] = temp;
}
/*使用三数取中法选取快速排序基准值*/
int Selectkeyval(Node* nodes,int left,int right)
{
    int mid = (left + right) / 2;
    if(nodes[mid].addr > nodes[right].addr)
        Swapnodes(nodes,mid,right);
    if(nodes[left].addr > nodes[right].addr)
        Swapnodes(nodes,left,right);
    if(nodes[mid].addr > nodes[left].addr)
        Swapnodes(nodes,mid,left);
    return nodes[left].addr;
}
/*对结点数组进行排序(使用快速排序算法)*/
void Sotrnodes(Node* nodes,int left,int right)
{
    /*若左边界大于右边界直接退出*/
    if(left > right)
        return;
    /*若不是，进入快速排序*/
    int left_ptr = left;
    int right_ptr = right;
    int keyval = Selectkeyval(nodes,left,right);
    while(left_ptr < right_ptr)
    {
        /*右指针寻找第一个比基准值小的值*/
        while((left_ptr < right_ptr) && (nodes[right_ptr].addr >= keyval))
            right_ptr --;
        /*左指针寻找第一个比基准值大的值*/
        while((left_ptr < right_ptr) && (nodes[left_ptr].addr <= keyval))
            left_ptr ++;
        if(left_ptr == right_ptr)
        {
            Swapnodes(nodes,left,left_ptr);
            break; //找到基准值，交换当前位置与基准值所在位置元素并退出
        }
        Swapnodes(nodes,left_ptr,right_ptr);//未找到，交换左右指针所指元素
    }
    /*对左右序列递归使用快速排序*/
    Sotrnodes(nodes,left,left_ptr - 1);
    Sotrnodes(nodes,right_ptr + 1,right);
    return;
}
/*读取输入结点数组,并按地址大小排序*/
void ReadNodes(Node* nodes,int N)
{
    for(int i_node = 0;i_node < N;i_node++)
        scanf("%d%d%d",&(nodes[i_node].addr),&(nodes[i_node].data),&(nodes[i_node].addr_next));
    Sotrnodes(nodes,0,N - 1);
    return;
}
/*依照地址寻找结点并返回所在下标*/
int FindNodes(Node* nodes,int N,int addr)
{
    /*采用二分查找算法*/
    int left = 0,right = N - 1;
    while(left <= right)
    {
        int mid = (left + right) / 2;
        if(nodes[mid].addr == addr) //找到
            return mid;
        else if(nodes[mid].addr < addr)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;//未找到
}
/*打印地址，修正成5位*/
void Printaddr(int addr)
{
    if(addr == -1)
        printf("%d",addr);
    else
        printf("%05d",addr);
    return;
}
/*打印结点*/
void Printnode(Node node)
{
    Printaddr(node.addr);
    putchar(' ');
    printf("%d",node.data);
    putchar(' ');
    Printaddr(node.addr_next);
    putchar('\n');
}
/*按规则反转表输出*/
void Printnodes_rev(Node* nodes,int head_addr,int N,int K)
{
    putchar('\n');
    /*按地址重组数组*/
    Node ordered_nodes[N];
    ordered_nodes[0] = nodes[FindNodes(nodes,N,head_addr)];
    for(int i_node = 1;i_node < N;i_node++)
    {
        if(ordered_nodes[i_node - 1].addr_next == -1)
        {
            N = i_node;
            break;//已经读到链表尾，说明之后的结点不属于该链表
        }
        ordered_nodes[i_node] = nodes[FindNodes(nodes,N,ordered_nodes[i_node - 1].addr_next)];
    }
    /*翻转各子序列*/
    for(int i_sublist = 0;i_sublist < (N / K);i_sublist++)
    {
        for(int i_node = K * i_sublist;i_node < K * i_sublist + K / 2;i_node++)
            Swapnodes(ordered_nodes,i_node,K * (2 * i_sublist + 1) - 1 - i_node);
    }
    /*调整数组各项的指向并输出*/
    for(int i_node = 0;i_node < N;i_node++)
    {
        if(i_node == N - 1)
            ordered_nodes[N - 1].addr_next = -1;
        else
            ordered_nodes[i_node].addr_next = ordered_nodes[i_node + 1].addr;
        Printnode(ordered_nodes[i_node]);
    }
    return;
}

int main()
{
    system("cls");
    int head_addr,N,K;//head_addr:结点的首地址，N:结点总数，K:翻转子序列长度
    scanf("%d%d%d",&head_addr,&N,&K);
    Node nodes[N];//储存各结点的数组
    ReadNodes(nodes,N);//读取结点输入并排序
    Printnodes_rev(nodes,head_addr,N,K);
    return 0;
}
#endif