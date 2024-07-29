#include "led.h"


//跑马灯RGB三元色配出七彩跑马灯
static u32 Run_buf[][16] = 
{
	{0xFFA500,0,0,0,0xFFA500,0,0,0,0xFFA500,0,0,0,0xFFA500,0,0,0,},//橙色
	{0x00FF00,0,0,0,0x00FF00,0,0,0,0x00FF00,0,0,0,0x00FF00,0,0,0,},//绿色
	{0xFF00FF,0,0,0,0xFF00FF,0,0,0,0xFF00FF,0,0,0,0xFF00FF,0,0,0,},//紫色
	{0x00FFFF,0,0,0,0x00FFFF,0,0,0,0x00FFFF,0,0,0,0x00FFFF,0,0,0,},//青色
	{0x0000FF,0,0,0,0x0000FF,0,0,0,0x0000FF,0,0,0,0x0000FF,0,0,0,},//蓝色
	{0xFFFF00,0,0,0,0xFFFF00,0,0,0,0xFFFF00,0,0,0,0xFFFF00,0,0,0,},//黄色
	{0xFFFFFF,0,0,0,0xFFFFFF,0,0,0,0xFFFFFF,0,0,0,0xFFFFFF,0,0,0,},//白色
	
};
uint8_t Run_flag=1;//跑马灯标志

/*********************************************************************
*函  数：void led_init(void)
*功  能：初始化LED控制引脚
*参  数：无
*返回值: 无
*备  注: 无
*********************************************************************/
void LED_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);	
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_SetBits(GPIOB, GPIO_Pin_8); 
	
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;     
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14 | GPIO_Pin_15;			
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_15);
}	
/***************************************************************************
*函  数：void LED_Run(void)
*功  能：指示MCU是否工作
*参  数：无
*返回值：无
*备  注: 无
***************************************************************************/
void LED_Run(void)
{
	static uint8_t flag = 1;
	if(flag)
	{
		flag = 0;
		GPIO_SetBits(GPIOB,GPIO_Pin_8);
	}else
	{
		flag = 1;
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	}
}

void RunTimer_Test(void)
{
	static uint8_t flag = 1;
	if(flag)
	{
		flag = 0;
		GPIO_SetBits(GPIOC,GPIO_Pin_15);
	}else
	{
		flag = 1;
		GPIO_ResetBits(GPIOC, GPIO_Pin_15);
	}
}
void RunTimer_Test2(void)
{
	static uint8_t flag = 1;
	if(flag)
	{
		flag = 0;
		GPIO_SetBits(GPIOC, GPIO_Pin_14);
	}else{
		flag = 1;
		GPIO_ResetBits(GPIOC, GPIO_Pin_14);
	}
}


/***************************************************************************
*函  数：void RGB_Write0(void)
*功  能：写0
*参  数：无
*返回值：无
*备  注: 不同的系统时钟频率下需要微调（默认HCLK=100MHz）
***************************************************************************/
void RGB_Write0(void)
{
	uint8_t cnt1=2,cnt2=6;
	//高电平300ns
	SET1;
	while(cnt1--)
	__nop();
	//低电平900ns
	SET0;
	while(cnt2--)
	__nop();
	__nop();
}

/***************************************************************************
*函  数：void RGB_Write1(void)
*功  能：写1
*参  数：无
*返回值：无
*备  注: 不同的系统时钟频率下需要微调（默认HCLK=100MHz）
***************************************************************************/
void RGB_Write1(void)
{
	uint8_t cnt1=6,cnt2=6;
	//高电平600ns
	SET1;
	while(cnt1--)
	__nop();
	__nop();
	//低电平600ns
	SET0;
	while(cnt2--)
	__nop();
	__nop();
}

/***************************************************************************
*函  数：void RGB_Reset(void)
*功  能：RGB灯复位
*参  数：无
*返回值：无
*备  注: 无
***************************************************************************/
void RGB_Reset(void)
{
	u16 cnt1 = 600,cnt2 = 1;
	SET0;
	while(cnt1--)
	__nop();
	SET1;
	while(cnt2--)
	__nop();
}

/***************************************************************************
*函  数：void RGB_WriteByte(uint8_t data)
*功  能：写一个字节数据
*参  数：data 
*返回值: 无
*备  注: 无
***************************************************************************/
void RGB_WriteByte(uint8_t data)
{
	int i = 0;
	for(i = 0; i < 8; i++)
	{
		if((data<<i) & 0x80)
		{
			RGB_Write1();
		}else{
			RGB_Write0();
		}
	}
}

/***************************************************************************
*函  数：void RGB_ColorSet(uint8_t green,uint8_t red,uint8_t blue)
*功  能：设置一个灯的色彩
*参  数：green red blue，红绿蓝光所占比例大小,范围0~255
*返回值: 无
*备  注: 无
***************************************************************************/
void RGB_ColorSet(uint8_t green, uint8_t red, uint8_t blue)
{
	RGB_WriteByte(green);	
	RGB_WriteByte(red);		
	RGB_WriteByte(blue);	
}

/**************************************************************************
*函  数：void RGB_LED_Rand(void)
*功  能：随机变换颜色
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_Rand(void)
{
	uint8_t i, red=0, green=0, blue=0;
	for(i = 0; i < 4; i++)
	{
		green = rand()%18+2;	//产生一个0~20的随机数
		red   = rand()%18+2;
		blue  = rand()%18+2;
		RGB_ColorSet(green, red, blue);
	}
}
/**************************************************************************
*函  数：void RGB_LED_Runing(void)
*功  能：跑马灯
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_Runing(void)
{
	uint8_t i, red=0,green=0,blue=0;
	static uint8_t cnt = 0, wcnt = 0, times = 0;
	if(times++>=16)
	{
		times = 0;
		wcnt++;
	}
	for(i = 0; i < 4; i++)
	{
		if(cnt > 4) cnt = 0;
		red   = ((Run_buf[wcnt][cnt]>>16)&0xff);
		green = ((Run_buf[wcnt][cnt]>>8)&0xff);
		blue  = ((Run_buf[wcnt][cnt]>>0)&0xff);
		RGB_ColorSet(green, red, blue);	//合成颜色
		cnt++;
	}
	if(wcnt==7) wcnt=0;
}
/**************************************************************************
*函  数：void RGB_LED_Red(void)
*功  能：红灯
*参  数：无
*返回值：无
*备  注：还有很多种颜色可以自己慢慢调配
**************************************************************************/
void RGB_LED_Red(void)
{
	uint8_t i;
	for(i = 0; i < 4; i++)
	{
		RGB_ColorSet(0, 0xFF, 0);
	}
}
/**************************************************************************
*函  数：void RGB_LED_Orange(void)
*功  能：橙灯
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_Orange(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		RGB_ColorSet(0xA5,0xFF,0x00);
	}
}
/**************************************************************************
*函  数：void RGB_LED_Yellow(void)
*功  能：黄灯
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_Yellow(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		RGB_ColorSet(0xFF,0xFF,0);
	}
}

/**************************************************************************
*函  数：void RGB_LED_Green(void)
*功  能：绿灯
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_Green(void)
{
	uint8_t i;
	for(i = 0; i < 4; i++)
	{
		RGB_ColorSet(0xFF, 0, 0);
	}
}
/**************************************************************************
*函  数：void RGB_LED_Cyan(void)
*功  能：青灯
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_Cyan(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		RGB_ColorSet(0xFF,0,0xFF);
	}
}

/**************************************************************************
*函  数：void RGB_LED_Blue(void)
*功  能：蓝灯
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_Blue(void)
{
	uint8_t i;
	for(i = 0; i < 4; i++)
	{
		RGB_ColorSet(0, 0, 0xFF);
	}
}
/**************************************************************************
*函  数：void RGB_LED_Violet(void)
*功  能：紫灯
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_Violet(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
    RGB_ColorSet(0x00,0xCD,0xCD);
	}
}

/**************************************************************************
*函  数：void RGB_LED_FLY(void)
*功  能：两红两绿
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_FLY(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		if(i<2)
			RGB_ColorSet(0xFF,0,0);
		else
			RGB_ColorSet(0,0xFF,0);
	}
}
/**************************************************************************
*函  数：void RGB_LED_White(void)
*功  能：白灯
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_White(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		RGB_ColorSet(0x0F,0x0F,0x0F);
	}
}
/**************************************************************************
*函  数：void RGB_LED_Off(void)
*功  能：关灯
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_LED_Off(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		RGB_ColorSet(0,0,0);
	}
}
/**************************************************************************
*函  数：void GYRO_Offset_LED(void)
*功  能：陀螺仪校准完成蓝灯闪烁
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void GYRO_Offset_LED(void)
{
	 RGB_LED_Off();
	 RGB_LED_Blue();
	 delay_ms(100);
	 RGB_LED_Off();
	 delay_ms(100);
	 RGB_LED_Blue();
	 delay_ms(100);
	 RGB_LED_Off();
	 delay_ms(100);
	 RGB_LED_Blue();
	 delay_ms(100);
	 RGB_LED_Off();
}
/**************************************************************************
*函  数：void ACC_Offset_LED(void)
*功  能：加速度校准完成绿灯闪烁
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void ACC_Offset_LED(void)
{
	 RGB_LED_Off();
	 RGB_LED_Green();
	 delay_ms(100);
	 RGB_LED_Off();
	 delay_ms(100);
	 RGB_LED_Green();
	 delay_ms(100);
	 RGB_LED_Off();
	 delay_ms(100);
	 RGB_LED_Green();
	 delay_ms(100);
	 RGB_LED_Off();
}

/**************************************************************************
*函  数：void BAR_Offset_LED(void)
*功  能：气压计校准完成紫灯闪烁
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void BAR_Offset_LED(void)
{
	 RGB_LED_Off();
	 RGB_LED_Violet();
	 delay_ms(100);
	 RGB_LED_Off();
	 delay_ms(100);
	 RGB_LED_Violet();
	 delay_ms(100);
	 RGB_LED_Off();
	 delay_ms(100);
	 RGB_LED_Violet();
	 delay_ms(100);
	 RGB_LED_Off();
}

/**************************************************************************
*函  数：void WiFi_OFFON_LED(void)
*功  能：靛青色灯闪烁提示WiFi开关
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void WiFi_OFFON_LED(void)
{
	static uint8_t cnt=0,flag = 0;
	if(WiFi_LEDflag==1)//wifi开启指示灯
	{
		if(cnt++>6) 
		{
			cnt = 0;
			WiFi_LEDflag = 0;
			Run_flag = 1; //打开运行灯
			RGB_LED_Off();
		}else 
		{
			if(flag)
			{
				flag = 0;
				RGB_LED_Cyan();
			}
			else
			{
				flag = 1;
				RGB_LED_Off();
			}
		}
	}
	else if(WiFi_LEDflag==2)//wifi关闭指示灯
	{
		if(cnt++>6) 
		{
			cnt = 0;
			WiFi_LEDflag = 0;
			Run_flag = 1; //打开运行灯
			RGB_LED_Off();
		}else
		{
			if(flag)
			{
				flag = 0;
				RGB_LED_Red();
			}
			else
			{
				flag = 1;
				RGB_LED_Off();
			}
		}
	}
}
/**************************************************************************
*函  数：void BATT_Alarm_LED(void)
*功  能：低电量红灯快闪
*参  数：无
*返回值：无
*备  注: 无
**************************************************************************/
void BATT_Alarm_LED(void)
{
	static uint8_t flag = 0;
	if(BATT_LEDflag)
	{
		if(flag)
		{
			flag = 0;
			RGB_LED_Red();
		}
		else
		{
			flag = 1;
			RGB_LED_Off();
		}
	}
}
/**************************************************************************
*函  数：void RGB_Unlock(uint8_t N,uint8_t flag)
*功  能：飞机解锁指示彩色灯
*参  数：N:彩灯变换频率
         flag:使能变换标志
*返回值：无
*备  注: 无
**************************************************************************/
void RGB_Unlock(uint8_t N,uint8_t flag)
{
	static uint8_t cnt = 0;
	if(flag && cnt++>N)
	{ 
		cnt = 0;
		RGB_LED_Rand();	
	}	
}
void OneNET_LED(uint8_t color[],uint8_t num)
{
	uint8_t i;
	for(i=0;i<num;i++)
	{
		RGB_ColorSet(color[1],color[0],color[2]);
	}
}
