#include "main.h"

int main(void)
{
	
	NVIC_config();
	LED_Init();
	Delay_Init();
	Usart1_Init(460800);
	Usart2_Init(921600);
	IIC_Init();
	TIM4_Init();
	Exti_Init();
	NRF24l01_Init();	//NRF初始化(红)
	MPU9250_Init();		//MPU9250初始化(绿)
	SPL06_Init();	//SPL06气压计初始化
	MOTOR_Init();	//电机输出初始化
	BATT_Init();	//电池电压检测初始化
	WiFi_Switch(ENABLE);	//wifi模块开关
	OpenMV_Switch(DISABLE);	//openmv模块开关
	PID_ReadFlash();	//Flash中的数据读取
	PidParameter_init();	//PID初始化
	RGB_LED_Off();	//关闭RGB
	while(1)
	{
		if(ANO_Scan)	//500Hz
		{
			ANO_Scan = 0;
			//更新数据到上位机
			ANO_DT_Data_Exchange();
		}
		if(IMU_Scan)	//100Hz
		{
			IMU_Scan = 0;
			//获取姿态解算所需数据
			Prepare_Data();
			//四元数姿态解算
			IMUupdate(&Gyr_rad,&Acc_filt,&Att_Angle);
			//姿态控制
			Control(&Att_Angle,&Gyr_rad,&RC_Control,Airplane_Enable);
			RunTimer_Test();
			spl06_update();	//气压计数据更新
			//更新高度
			altitude_get();
		}
		if(LED_Scan)	//10Hz
		{
			LED_Scan = 0;
			LED_Run();
			if(!Airplane_Enable && Run_flag && !WiFi_LEDflag)
			{
				RGB_LED_Runing();	//飞机上锁状态灯
			}
			WiFi_OFFON_LED();	//WiFi开关状态灯
			BATT_Alarm_LED();	//电池低电压报警
		}
		if(IRQ_Scan)	//5Hz
		{
			IRQ_Scan = 0;
			NRF_SingalCheck();	//NRF信号质量检测
			SendToRemote();		//发送数据给遥控器
		}
		if(Batt_Scan)
		{
			Batt_Scan = 0;
			NRF_GetAddr();	//分配NRF地址
			LowVoltage_Alarm();	//低电压报警
		}
	}
	
}
