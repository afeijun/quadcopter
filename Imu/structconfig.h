#ifndef STRUCTCONFIG__H
#define STRUCTCONFIG__H

#include "stm32f4xx.h"

/************************************************PID参数调试上位机波形显示*************************************************/
/* 如果宏定义WIFI_DEBUG开启无线调参，否则就是有线调参 */
#define WIFI_DEBUG
/* 如果想观察哪个轴的PID波形，就取消掉哪个注释，但是一次只能取消掉一个注释 */
//#define ROL_PID_DEBUG    
//#define PIT_PID_DEBUG
//#define YAW_PID_DEBUG
//#define ALT_PID_DEBUGE
/**************************************************************************************************************************/

/********************************************飞机状态记录组SENSER_OFFSET_FLAG**********************************************/
extern uint8_t   SENSER_OFFSET_FLAG; //标志位组

//每一位对应功能
#define GYRO_OFFSET 0x01 //第一位陀螺仪校准标志位
#define ACC_OFFSET 0x02  //第二位加速度校准标志位
#define BAR_OFFSET 0x04  //第三位气压计校准标志位
#define MAG_OFFSET 0x08  //第四位磁力计校准标志位
#define FLY_ENABLE 0x10  //第五位解锁上锁
#define WiFi_ONOFF 0x20  //第六位WiFi开关
#define FLY_MODE   0x40  //第七位模式选择(0:无头模式(默认) 1:有头模式)

//对 SENSER_OFFSET_FLAG 的位的操作
#define SENSER_FLAG_SET(FLAG)   SENSER_OFFSET_FLAG|=FLAG                //标志位置1
#define SENSER_FLAG_RESET(FLAG) SENSER_OFFSET_FLAG&=~FLAG               //标志位值0
#define GET_FLAG(FLAG)         (SENSER_OFFSET_FLAG&FLAG)==FLAG ? 1 : 0  //获取标志位状态
/**************************************************************************************************************************/

//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆分成单独字节进行发送
#define Byte0(data)       ( *( (char *)(&data)		) )
#define Byte1(data)       ( *( (char *)(&data) + 1) )
#define Byte2(data)       ( *( (char *)(&data) + 2) )
#define Byte3(data)       ( *( (char *)(&data) + 3) )

//三轴整型（MPU9250原始数据）
typedef struct
{
	int16_t X;
	int16_t Y;
	int16_t Z;
}INT16_XYZ;

//三轴浮点型（三轴陀螺仪和三轴加速度计）
typedef struct
{
	float X;
	float Y;
	float Z;
}FLOAT_XYZ;

//姿态解算后的角度
typedef struct
{
	float rol;	//横滚
	float pit;	//俯仰
	float yaw;	//航向
}FLOAT_ANGLE;

//遥控器的数据结构 
typedef struct
{
	int16_t ROLL;
	int16_t PITCH;
	int16_t THROTTLE;
	int16_t YAW;
}RC_TYPE;

//FBM32的数据结构 
typedef struct FBMTYPE
{
  int32_t  ADPress;
  int32_t  ADTemp;
  float  RP;
  float  RT;
  float  RPFilter;
  float  InitPress;
  float  Altitude;
  float  AltitudeFilter;
  uint16_t C0, C1, C2, C3, C6, C8, C9, C10, C11, C12;
  uint32_t C4, C5, C7;
	
} FBMTYPE;

//高度融合的数据结构
typedef struct NAV_tt
{
    float x;
    float y;
    float z;
    float vx;
    float vy;
    float vz;
    float ax;
    float ay;
    float az;
} nav_t;

//PID算法的数据结构 
typedef struct PID
{
  float P;          //参数Kp
  float I;			//参数Ki
  float D;			//参数Kd
  float Error;     //比例项
  float Integral;  //积分项
  float Differ;    //微分项
  float PreError;	//上一次误差
  float PrePreError;	//上上次误差
  float Ilimit; 		//积分分离
//  float MotoCompensation; //电机补偿  
  float Irang;			//积分限幅
  uint8_t Ilimit_flag;    //积分分离标志
  float Pout;		//比例项输出
  float Iout;		//积分项输出
  float Dout;		//微分项输出
  float OutPut;     //PID控制器总输出
}PID_TYPE; 

//保存的参数数据结构 
typedef struct PIDSave
{
	//陀螺仪校准数据 
	u16  ACC_OFFSET_X;
	u16  ACC_OFFSET_Y;
	u16  ACC_OFFSET_Z;
	u16  GYRO_OFFSET_X;
	u16  GYRO_OFFSET_Y;
	u16  GYRO_OFFSET_Z;
	//角度环 
	u16  ROL_Angle_P;
	u16  ROL_Angle_I;
	u16  ROL_Angle_D;
	u16  PIT_Angle_P;
	u16  PIT_Angle_I;
	u16  PIT_Angle_D;
	u16  YAW_Angle_P;
	u16  YAW_Angle_I;
	u16  YAW_Angle_D;
	//角速度环 
	u16  ROL_Rate_P;
	u16  ROL_Rate_I;
	u16  ROL_Rate_D;
	u16  PIT_Rate_P;
	u16  PIT_Rate_I;
	u16  PIT_Rate_D;
	u16  YAW_Rate_P;
	u16  YAW_Rate_I;
	u16  YAW_Rate_D;
	//高度环
	u16  ALT_Rate_P;
	u16  ALT_Rate_I;
	u16  ALT_Rate_D;
	u16  ALT_P;
	u16  ALT_I;
	u16  ALT_D;
	u16  NRFaddr;

}PID_SAVE;

//电池电压管理数据结构 
typedef struct BATT_TYPE
{
	float BattAdc;
	float BattRealV;
	float BattMeasureV;
	float BattAlarmV;
	float BattFullV;
}BATT_TYPE;
//姿态解算
extern FLOAT_XYZ 	 Gyr_degree,Gyr_rad;	                //把陀螺仪的各通道读出的数据，转换成弧度制和度
extern INT16_XYZ   MPU9250_ACC_RAW,MPU9250_GYRO_RAW;	  //MPU最新一次原始数据
extern INT16_XYZ	 GYRO_OFFSET_RAW,ACC_OFFSET_RAW;			//MPU零漂值
extern FLOAT_XYZ 	 Acc_filt,Gry_filt;	                  //滤波后的各通道数据
extern float       DCMgb[3][3];
extern float       Zacc,accb[3];
//高度解算
extern uint8_t   ALTIUDE_OK,ALT_Updated;//气压计初始化高度标志
extern FBMTYPE   FBM;                                   //气压计数据
extern nav_t     nav;
//遥控数据缓存
extern uint8_t NRFaddr;
extern float Moto_PWM_1,Moto_PWM_2,Moto_PWM_3,Moto_PWM_4;
//角度环PID
extern PID_TYPE PID_ROL_Angle;
extern PID_TYPE PID_PIT_Angle;
extern PID_TYPE PID_YAW_Angle;
//角速度环PID
extern PID_TYPE PID_ROL_Rate;
extern PID_TYPE PID_PIT_Rate;
extern PID_TYPE PID_YAW_Rate;
//高度PID
extern PID_TYPE PID_ALT_Rate;
extern PID_TYPE PID_ALT;
//参数保存
extern uint8_t        InitDefaultParam; //初始化默认参数
extern PID_SAVE       PIDflash;   //各轴PID参数保存实例
extern FLOAT_ANGLE    Att_Angle;	//ATT函数计算出的姿态角
extern RC_TYPE        RC_Control;
//功能键
extern uint8_t Airplane_Enable,Run_flag;
extern uint8_t BATT_LEDflag;	//低电量标志
extern uint8_t WiFi_LEDflag,WiFi_Controlflag;

extern uint8_t LED_Scan ;
extern uint8_t IMU_Scan ;
extern uint8_t MPU_Scan ;
extern uint8_t IRQ_Scan ;
extern uint8_t Batt_Scan;
extern uint8_t ANO_Scan ;

extern float THROTTLE;
extern BATT_TYPE BAT;
#endif
