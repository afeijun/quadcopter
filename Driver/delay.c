#include "delay.h"

#ifdef NOINT_DELAY //重载法延时

static uint32_t fac_us; //微秒系数
static uint32_t fac_ms; //毫秒系数

/****************************************************************************************************
*函  数: void Delay_Init(void)
*功  能: 延时函数初始化
*参  数: 无
*返回值：无
*备  注: T(s) = 1/F(Hz) //赫兹时间转换公式
****************************************************************************************************/
void Delay_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//选择系统时钟（HCLK/8）作为SysTick定时器的时钟
	SysTick->CTRL = 0x00;
	fac_us = (SystemCoreClock/8)/1000000; //获取微秒系数
	fac_ms = fac_us*1000; //获取毫秒系数
}

/****************************************************************************************************
*函  数: void delay_ms(uint32_t nTime)
*功  能: 毫秒级延时
*参  数: nTime 延时时间
*返回值：无
*备  注: SysTick的计数器为24位 nTime最大值为 0xffffff/fac_ms = 1342
****************************************************************************************************/
void delay_ms(uint32_t nTime)
{
	uint32_t temp;
	SysTick->CTRL = 0x00; //失能SysTick定时器
	SysTick->LOAD = nTime*fac_ms; //计算重装载值
	SysTick->VAL  = 0x00; //清空计数器
	SysTick->CTRL|= 0x01; //启动SysTick定时器
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16))); //循环等待延时结束
	SysTick->CTRL = 0x00; //失能SysTick定时器
	SysTick->VAL  = 0x00;//清空计数器
}

/****************************************************************************************************
*函  数: void delay_us(uint32_t nTime)
*功  能: 微妙级延时
*参  数: nTime 延时时间
*返回值：无
*备  注: SysTick的计数器为24位 nTime最大值为 0xffffff/fac_us = 798177
****************************************************************************************************/
void delay_us(uint32_t nTime)
{
	uint32_t temp;	
	SysTick->CTRL = 0x00; //失能SysTick定时器
	SysTick->LOAD = nTime*fac_us; //计算重装载值
	SysTick->VAL  = 0x00; //清空计数器
	SysTick->CTRL|= 0x01; //启动SysTick定时器
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16))); //循环等待延时结束
	SysTick->CTRL = 0x00; //失能SysTick定时器
	SysTick->VAL  = 0x00;//清空计数器(这个必须有否则将会死在while)
}
#endif //#ifdef NOINT_DELAY 

#ifdef INT_DELAY	//开中断延时

static uint32_t usTicks = 0;		 // Systick 每中断一次记一次数
volatile uint32_t sysTickUptime = 0; // 微妙计数

void Delay_Init(void)
{
	SysTick_Config(SystemCoreClock/1000); 		//选择系统时钟（HCLK）作为SysTick定时器的时钟，开SysTick中断 1ms一次
	usTicks = SystemCoreClock/1000000; //100M/1M = 100次 也就是计数100次为1us
}

//微妙计数器
uint32_t micros(void)
{
	//register请求编译器尽可能的将变量存在CPU内部寄存器，而不是通过内存寻址访问
	register uint32_t ms, cycle_cnt;
	do
	{
		ms = sysTickUptime;
		cycle_cnt = SysTick->VAL;	// SysTick->VAL 的计数范围 100000 ~ 0就是1ms,递减计数器
	}while(ms != sysTickUptime);
	return (ms*1000)+(100000-cycle_cnt)/usTicks;
}


//毫秒计数器ֵ
uint32_t millis(void)
{
	return sysTickUptime;
}

//毫秒延时
void delay_ms(uint32_t nms)
{
	uint32_t mstart = 0;
	mstart = millis();	//毫秒开始值
	while(millis()-mstart <= nms){}
}
//微妙延时
void delay_us(uint32_t nus)
{
	uint32_t ustart = 0;
	ustart = micros();//微秒开始值
	while(micros()-ustart <= nus){}
}

#endif

//粗略延时
void delay(int cnt)
{
	int i,j;
	for(i=0;i<cnt;i++)
	{
		for(j=0;j<0xfff;j++)
		{}
	}
}
