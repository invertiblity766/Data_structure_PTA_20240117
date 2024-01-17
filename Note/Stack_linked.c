#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;

typedef struct Stack *PStack;
struct Stack{
    ElementType data;
    PStack next;
};

/*创建空栈*/
PStack CreateStack(void)
{
    PStack top;
    top = (PStack)malloc(sizeof(struct Stack));
    top->next = NULL;
}
/*判断栈是否为空*/
int isemptyStack(PStack top)
{
    return (top->next == NULL);
}
/*将元素X压入栈内*/
void PushStack(PStack top,ElementType X)
{
    PStack newnode;
    newnode = (PStack)malloc(sizeof(struct Stack));
    newnode->data = X;
    newnode->next = top->next;
    top->next = newnode;
}
/*弹出栈顶元素，删除结点*/
ElementType PopStack(PStack top)
{
    if(isemptyStack(top))
    {
        printf("Stack is empty.\n");
        return NULL;
    }
    PStack BanishedNode = top->next;
    top->next = BanishedNode->next;
    ElementType data = BanishedNode->data;
    free(BanishedNode);
    return data;
}
void PrintStack(PStack top)
{
    PStack Ptr = top;
    while(Ptr->next != NULL) //跳过头结点
    {
        Ptr = Ptr->next;
        printf("%d ",Ptr->data);
    }
    putchar('\n');
}


int main()
{
    system("cls");
    const int arrsize = 10;
    PStack top = CreateStack();
    for(int i = 0;i < arrsize;i++)
        PushStack(top,i);
    PopStack(top);
    PrintStack(top);
    return 0;

}
