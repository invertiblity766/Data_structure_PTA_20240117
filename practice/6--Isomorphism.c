#include <stdio.h>
#include <stdlib.h>

#define NULL -1
#define MAXTreesize 10
/*采用结构数组方式建树，数组下标对应结点编号*/
typedef struct Treenode *PTree;
struct Treenode{
    char data;
    int lchild;
    int rchild;
}T1[MAXTreesize],T2[MAXTreesize];//树结点定义与建立
/*读取并建立二叉树*/
int CreateBintree(PTree T)
{
    int N = 0;
    scanf("%d",&N);getchar();//scanf每次输入完后'\n'会留在数据缓冲区
    for(int i = 0;i < N;i++)
    {
        char data,lchild,rchild;
        scanf("%c %c %c",&data,&lchild,&rchild);getchar();
        T[i].data = data;
        T[i].lchild = (lchild == '-') ? NULL : (lchild - '0');
        T[i].rchild = (rchild == '-') ? NULL : (rchild - '0');
    }
    return N;
}
/*寻找树的根节点*/
int FindRoot(PTree T,int N)
{
    int flags[MAXTreesize];//用于标记是否出现在子结点内
    for(int i = 0;i < N;i++)
        flags[i] = 0;//初始化为均没出现的情况
    for(int i = 0;i < N;i++)
    {
        if(T[i].lchild != NULL)
            flags[T[i].lchild] = 1;
        if(T[i].rchild != NULL)
            flags[T[i].rchild] = 1;
    }
    for(int i = 0;i < N;i++)
    {
        if(flags[i] == 0)
            return i;
    }
    return NULL;
}
/*同构判断*/
int isojudge(PTree T1,PTree T2,int T1_pos,int T2_pos)//1表示同构，0表示不同构
{
    //要把左右儿子也当成一株树来思考，根节点同构+左子树同构+右子树同构=整棵树同构
    if(T1[T1_pos].data != T2[T2_pos].data)
        return 0;//若当前结点不等，直接返回不同构
    /*
    按照先序遍历顺序，有以下细分情况
    1、T1左右儿子均为非空
        首先检视T2左右儿子是否也是全非空，若不是则直接输出不同构。比较T1与T2左儿子的值，若相等均按左右儿子顺序访问，若不等T2按右左儿子顺序访问
    2、T1左右儿子均为空
        检视T2左右儿子是否全空，若不是直接输出不同构，否则返回同构
    3、T1左/右儿子为空
        检视T2是否只有一个儿子非空，若不是直接输出不同构，否则T1,T2进入非空的儿子
    总的来说，即处理二叉树四种情况下，T2树的访问顺序
    */
   if(T1[T1_pos].lchild == NULL && T1[T1_pos].rchild == NULL)//
   {
        if(T2[T2_pos].lchild != NULL || T2[T2_pos].rchild != NULL)
            return 0;
        return 1;
   }
   else if(T1[T1_pos].lchild != NULL && T1[T1_pos].rchild != NULL)
   {
        if(T2[T2_pos].lchild == NULL || T2[T2_pos].rchild == NULL)
            return 0;
        if(T1[T1[T1_pos].lchild].data == T2[T2[T2_pos].lchild].data)
            return isojudge(T1,T2,T1[T1_pos].lchild,T2[T2_pos].lchild) && isojudge(T1,T2,T1[T1_pos].rchild,T2[T2_pos].rchild);
        else
            return isojudge(T1,T2,T1[T1_pos].lchild,T2[T2_pos].rchild) && isojudge(T1,T2,T1[T1_pos].rchild,T2[T2_pos].lchild);
   }
   else
   {
        if((T2[T2_pos].lchild == NULL) + (T2[T2_pos].rchild == NULL) != 1)
            return 0;
        int nonvoid1 = (T1[T1_pos].lchild == NULL) ? T1[T1_pos].rchild : T1[T1_pos].lchild;
        int nonvoid2 = (T2[T2_pos].lchild == NULL) ? T2[T2_pos].rchild : T2[T2_pos].lchild;
        return isojudge(T1,T2,nonvoid1,nonvoid2);
   } 
}
int main()
{
    system("cls");
    int T1_num = CreateBintree(T1);
    int T2_num = CreateBintree(T2);
    if(T1_num != T2_num)//若两棵树结点数不相等，则肯定不同构
        printf("No\n");
    else if(T1_num == 0 && T2_num == 0)//若两棵树均为空树，则肯定同构
        printf("Yes\n");
    else
    {
        int T1_root = FindRoot(T1,T1_num);
        int T2_root = FindRoot(T2,T2_num);
        int ismorphism = isojudge(T1,T2,T1_root,T2_root);
        printf(ismorphism ? "Yes\n" : "No\n");
    }
    return 0;
}