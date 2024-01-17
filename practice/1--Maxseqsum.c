#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 10000
#define solution2

typedef struct {
    int maxsum;
    int leftnum;
    int rightnum;
}Maxsubseq;

#ifdef solution1
int Maxsubseqsum1(int* arr,int arrsize)
{
    //最简单的想法：将所有子列求出并比较出最大值,与solution2一样，不再重复改造
    int thissum = 0, maxsum = 0;
    //索引i表示左侧位置，j表示右侧位置
    for(int i = 0;i < arrsize;i++)
    {
        for(int j = i;j < arrsize;j++)
        {
            thissum = 0;
            for(int k = i;k <= j;k++)
                thissum += arr[k];
            maxsum = (thissum > maxsum) ? thissum : maxsum;
        }
    }
    return (maxsum > 0) ? maxsum : 0;

}
#endif
#ifdef solution2
    Maxsubseq Maxsubseqsum2(int* arr,int arrsize)
    {
        //对算法1进行优化，不重复每次计算子列和，而是在上一次结果上自增即可
        int thissum = 0;
        Maxsubseq maxsubseq = {0,arr[0],arr[arrsize - 1]}; 
        //索引i表示左侧位置,j表示右侧位置
        for(int i = 0;i < arrsize;i++)
        {
            thissum = 0;
            for(int j = i;j < arrsize;j++)
            {
                thissum += arr[j];
                if(thissum > maxsubseq.maxsum)
                {
                    maxsubseq.maxsum = thissum;
                    maxsubseq.leftnum = arr[i];
                    maxsubseq.rightnum = arr[j];
                }
            }
        }
        if(maxsubseq.maxsum <= 0)
        {
            int cnt = 0;
            while((arr[cnt] < 0) & (cnt < arrsize))
                cnt++;
            maxsubseq.maxsum = 0;
            if(cnt < arrsize - 1)
            {
                maxsubseq.leftnum = 0;
                maxsubseq.rightnum = 0;
            }
            else
            {
                maxsubseq.leftnum = arr[0];
                maxsubseq.rightnum = arr[arrsize - 1];
            }
        }
        return maxsubseq;
    }
#endif
#ifdef solution3
    Maxsubseq Maxsubseqsum3(int* arr,int left,int right)
    {
        //采用分治算法，T(N) = O(NlogN)
        int mid = (left + right) / 2;
        if(left == right)
        {
            Maxsubseq maxsubseq = {arr[left],arr[left],arr[right]};
            return maxsubseq;
        }
        Maxsubseq leftmaxsum = Maxsubseqsum3(arr,left,mid); //寻找左子序列最大值
        Maxsubseq rightmaxsum = Maxsubseqsum3(arr,mid + 1,right);//寻找右子序列最大值
        //寻找跨越边界的最大子序列和 = 含左序列最后一个元素的最大子列和 + 含右序列第一个元素的最大子列和
        int thissum = 0;
        Maxsubseq rightboadersum = {arr[mid + 1],arr[mid + 1],arr[mid + 1]};
        Maxsubseq leftboadersum = {arr[mid],arr[mid],arr[mid]};
        //寻找含右序列第一个元素的最大子列和
        for(int i = mid + 1;i <= right;i++)
        {
            thissum += arr[i];
            if(thissum > rightboadersum.maxsum) //找到更大子序列，更新右边界与最大值
            {
                rightboadersum.maxsum = thissum;
                rightboadersum.rightnum = arr[i];
            }
        }
        thissum = 0;
        for(int i = mid;i >= left;i--) //找到更大子序列，更新左边界与最大值
        {
            thissum += arr[i];
            if(thissum > leftboadersum.maxsum)
            {
                leftboadersum.maxsum = thissum;
                leftboadersum.leftnum = arr[i];
            }
        }
        Maxsubseq boadermaxsum = {leftboadersum.maxsum + rightboadersum.maxsum,leftboadersum.leftnum,rightboadersum.rightnum};
        //输出三者最大值
        Maxsubseq maxsubseq; 
        if(leftmaxsum.maxsum >= rightmaxsum.maxsum)
            maxsubseq = (leftmaxsum.maxsum >= boadermaxsum.maxsum) ? leftmaxsum : boadermaxsum;
        else
            maxsubseq = (rightmaxsum.maxsum > boadermaxsum.maxsum) ? rightmaxsum : boadermaxsum;
        return maxsubseq;
    }
#endif
#ifdef solution4
    Maxsubseq Maxsubseqsum4(int* arr,int arrsize)
    {
        //在线处理，考虑到负数和不可能使得后续子列和增大直接舍弃，T(N) = O(N)
        int thissum = 0;
        int possible_leftnum = arr[0]; //在切换序列时记录可能的左边界
        Maxsubseq maxsubseq = {0,arr[0],arr[0]};
        for(int i = 0;i < arrsize;i++)
        {
            thissum += arr[i];
            if(thissum > maxsubseq.maxsum) //此时找到更大子序列，替代之
            {
                maxsubseq.maxsum = thissum;
                maxsubseq.leftnum = possible_leftnum;
                maxsubseq.rightnum = arr[i];
            }
            if(thissum < 0) //此时包含当前序列的子列不可能时最大子列和，舍弃，切换至后续子列
            {
                thissum = 0;
                if(i < arrsize - 1)
                    possible_leftnum = arr[i + 1]; //记录新序列左边界，若新序列为最大子列和可查找得到序列左边界
            }
        }
        if(maxsubseq.maxsum == 0) //当计算结果是零，说明序列全为负或含有零
        {
            int cnt = 0;
            while((arr[cnt] < 0) & (cnt < arrsize))
                cnt++;
            if(cnt < arrsize)
            {
                maxsubseq.leftnum = arr[cnt];
                maxsubseq.rightnum = arr[cnt];
            }
            else
            {
                maxsubseq.leftnum = arr[0];
                maxsubseq.rightnum = arr[arrsize - 1];
            }
        }
        return maxsubseq;
    }
#endif

int main()
{
    system("cls");
    int arrsize = 0;
    int arr[MAX_SIZE] = {0};
    scanf("%d",&arrsize);
    for(int cnt = 0;cnt < arrsize;cnt++)
        scanf("%d",&arr[cnt]);
    #ifdef solution1 
        printf("%d\n",Maxsubseqsum1(arr,arrsize));
    #endif
    #ifdef solution2
        Maxsubseq maxsubseq = Maxsubseqsum2(arr,arrsize);
        printf("%d %d %d\n",maxsubseq.maxsum,maxsubseq.leftnum,maxsubseq.rightnum);
    #endif
    #ifdef solution3
        Maxsubseq maxsubseq = Maxsubseqsum3(arr,0,arrsize - 1);
        if(maxsubseq.maxsum <= 0)
        {
            int cnt = 0;
            while((arr[cnt] < 0) & (cnt < arrsize))
                cnt ++;
            maxsubseq.maxsum = 0;
            if(cnt < arrsize - 1)
            {
                maxsubseq.leftnum = 0;
                maxsubseq.rightnum = 0;
            }
            else
            {
                maxsubseq.leftnum = arr[0];
                maxsubseq.rightnum = arr[arrsize - 1];
            }
        }
        printf("%d %d %d\n",maxsubseq.maxsum,maxsubseq.leftnum,maxsubseq.rightnum);
    #endif
    #ifdef solution4
        Maxsubseq maxsubseq = Maxsubseqsum4(arr,arrsize);
        printf("%d %d %d\n",maxsubseq.maxsum,maxsubseq.leftnum,maxsubseq.rightnum);
    #endif
    return 0;
}