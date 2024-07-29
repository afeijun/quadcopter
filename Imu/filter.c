#include "filter.h"


#define N 20      //滤波缓存数组大小
#define M_PI_F 3.1416f

/*******************************************************************************
*函  数 ：float FindPos(float*a,int low,int high)
*功  能 ：确定一个元素位序
*参  数 ：a  数组首地址
*         low数组最小下标
*         high数组最大下标
*返回值 ：返回元素的位序low
*备  注 : 无
*******************************************************************************/
float FindPos(float*a,int low,int high)
{
    float val = a[low];                      //选定一个要确定值val确定位置
    while(low<high)
    {
        while(low<high && a[high]>=val)
             high--;                       //如果右边的数大于VAL下标往前移
        a[low] = a[high];             	//当右边的值小于VAL则复值给A[low]

        while(low<high && a[low]<=val)
             low++;                        //如果左边的数小于VAL下标往后移
        a[high] = a[low];             	//当左边的值大于VAL则复值给右边a[high]
    }
    a[low] = val;//
    return low;
}
/*******************************************************************************
*函  数 ：void QuickSort(float* a,int low,int high)
*功  能 ：快速排序
*参  数 ：a  数组首地址
*         low数组最小下标
*         high数组最大下标
*返回值 ：无
*备  注 : 无
*******************************************************************************/
 void QuickSort(float* a,int low,int high)
 {
     int pos;
     if(low<high)
     {
         pos = FindPos(a,low,high); //排序一个位置
         QuickSort(a,low,pos-1);    //递归调用
         QuickSort(a,pos+1,high);
     }
 }

/*******************************************************************************
*函  数 ：void  SortAver_FilterXYZ(INT16_XYZ *acc,FLOAT_XYZ *Acc_filt,uint8_t n)
*功  能 ：去最值平均值滤波三组数据
*参  数 ：*acc 要滤波数据地址
*         *Acc_filt 滤波后数据地址
*返回值 ：返回滤波后的数据
*备  注 : 无
*******************************************************************************/
void  SortAver_FilterXYZ(INT16_XYZ *acc,FLOAT_XYZ *Acc_filt,uint8_t n)
{
	static float bufx[N],bufy[N],bufz[N];
	static uint8_t cnt =0,flag = 1;
	float temp1=0,temp2=0,temp3=0;
	uint8_t i;
	bufx[cnt] = acc->X;
	bufy[cnt] = acc->Y;
	bufz[cnt] = acc->Z;
	cnt++;      //这个的位置必须在赋值语句后，否则bufx[0]不会被赋值
	if(cnt<n && flag) 
		return;   //数组填不满不计算
	else
		flag = 0;
	
	QuickSort(bufx,0,n-1);
	QuickSort(bufy,0,n-1);
	QuickSort(bufz,0,n-1);
	for(i=1;i<n-1;i++)
	{
		temp1 += bufx[i];
		temp2 += bufy[i];
		temp3 += bufz[i];
	}

	if(cnt>=n) cnt = 0;
	Acc_filt->X  = temp1/(n-2);
	Acc_filt->Y  = temp2/(n-2);
	Acc_filt->Z  = temp3/(n-2);
}

/*******************************************************************************
*函  数 ：void Aver_Filter(float data,float *filt_data,uint8_t n)
*功  能 ：滑动窗口滤波一组数据
*参  数 ：data  要滤波数据
*         *filt_data 滤波后数据地址
*返回值 ：返回滤波后的数据
*备  注 : 无
*******************************************************************************/
void Aver_Filter(float data,float *filt_data,uint8_t n)
{
	static float buf[N];
	static uint8_t cnt =0,flag = 1;
	float temp=0;
	uint8_t i;
	buf[cnt] = data;
	cnt++;
	if(cnt<n && flag) 
		return;   //数组填不满不计算
	else
		flag = 0;
	
	for(i=0;i<n;i++)
	{
		temp += buf[i];
	}
	if(cnt>=n) cnt = 0;
	 *filt_data = temp/n;
}

