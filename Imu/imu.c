#include "imu.h"

#define Kp_New      0.9f              //互补滤波当前数据的权重
#define Kp_Old      0.1f              //互补滤波历史数据的权重  
//#define G					  9.80665f		      // m/s^2	
//#define RadtoDeg    57.324841f				//弧度到角度 (弧度 * 180/3.1415)
//#define DegtoRad    0.0174533f				//角度到弧度 (角度 * 3.1415/180)
#define Acc_Gain  	0.0001220f				//加速度变成G (初始化加速度满量程-+4g LSBa = 2*4/65535.0)
#define Gyro_Gain 	0.0609756f				//角速度变成度 (初始化陀螺仪满量程+-2000 LSBg = 2*2000/65535.0)
#define Gyro_Gr	    0.0010641f			  //角速度变成弧度(3.1415/180 * LSBg)

FLOAT_ANGLE Att_Angle;                       //飞机姿态数据
FLOAT_XYZ 	Gyr_rad,Gyr_radold;	              //把陀螺仪的各通道读出的数据，转换成弧度制
FLOAT_XYZ 	Acc_filt,Gry_filt,Acc_filtold;	  //滤波后的各通道数据
//FLOAT_XYZ   Hmc_filt;
float   accb[3],DCMgb[3][3];                  //方向余弦阵（将 惯性坐标系 转化为 机体坐标系）
uint8_t AccbUpdate = 0;
/**************************实现函数*********************************************************************
函  数：static float invSqrt(float x) 
功　能: 快速计算 1/Sqrt(x) 	
参  数：要计算的值
返回值：结果
备  注：比普通Sqrt()函数要快四倍See: http://en.wikipedia.org/wiki/Fast_inverse_square_root
*********************************************************************************************************/
static float invSqrt(float x)
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}
/*********************************************************************************************************
*函  数：void Prepare_Data(void)
*功　能：对陀螺仪去零偏后的数据滤波及赋予物理意义
*参  数：无
*返回值：无
*备  注：此函数对原始数据进行校准（去零偏，滤波处理，赋予物理意义 为姿态解算做准备
**********************************************************************************************************/	
void Prepare_Data(void)
{
 
	static uint8_t IIR_mode = 1;
	
	MPU9250_Read();    //触发读取 ，立即返回
	
//	ICM_ReadAccelGyroData();

	MPU9250_Offset();  //对MPU6050进行处理，减去零偏。如果没有计算零偏就计算零偏
	
//	Aver_FilterXYZ(&MPU9250_ACC_RAW,&Acc_filt,12);//对加速度原始数据进行滑动窗口滤波
	SortAver_FilterXYZ(&MPU9250_ACC_RAW,&Acc_filt,12);//对加速度原始数据进行去极值滑动窗口滤波
	
	//加速度AD值 转换成 米/平方秒 
	Acc_filt.X = (float)Acc_filt.X * Acc_Gain * G;
	Acc_filt.Y = (float)Acc_filt.Y * Acc_Gain * G;
	Acc_filt.Z = (float)Acc_filt.Z * Acc_Gain * G;
  //printf("ax=%0.2f ay=%0.2f az=%0.2f\r\n",Acc_filt.X,Acc_filt.Y,Acc_filt.Z);

	//陀螺仪AD值 转换成 弧度/秒    
	Gyr_rad.X = (float) MPU9250_GYRO_RAW.X * Gyro_Gr;  
	Gyr_rad.Y = (float) MPU9250_GYRO_RAW.Y * Gyro_Gr;
	Gyr_rad.Z = (float) MPU9250_GYRO_RAW.Z * Gyro_Gr;
	
	if(IIR_mode)
	{
		Acc_filt.X = Acc_filt.X * Kp_New + Acc_filtold.X * Kp_Old;
		Acc_filt.Y = Acc_filt.Y * Kp_New + Acc_filtold.Y * Kp_Old;
		Acc_filt.Z = Acc_filt.Z * Kp_New + Acc_filtold.Z * Kp_Old;
//		Gyr_rad.X = Gyr_rad.X * Kp_New + Gyr_radold.X * Kp_Old;
//		Gyr_rad.Y = Gyr_rad.Y * Kp_New + Gyr_radold.Y * Kp_Old;
//		Gyr_rad.Z = Gyr_rad.Z * Kp_New + Gyr_radold.Z * Kp_Old;
		
		Acc_filtold.X =  Acc_filt.X;
		Acc_filtold.Y =  Acc_filt.Y;
		Acc_filtold.Z =  Acc_filt.Z;
//		Gyr_radold.X = Gyr_rad.X;
//		Gyr_radold.Y = Gyr_rad.Y;
//		Gyr_radold.Z = Gyr_rad.Z;
	}
	accb[0] = Acc_filt.X;
	accb[1] = Acc_filt.Y;
	accb[2] = Acc_filt.Z;
	
	if(accb[0]&&accb[1]&&accb[2])
	{
		AccbUpdate = 1;
	}
}



/*********************************************************************************************************
*函  数：void IMUupdate(FLOAT_XYZ *Gyr_rad,FLOAT_XYZ *Acc_filt,FLOAT_ANGLE *Att_Angle)
*功　能：获取姿态角
*参  数：Gyr_rad 指向角速度的指针（注意单位必须是弧度）
*        Acc_filt 指向加速度的指针
*        Att_Angle 指向姿态角的指针
*返回值：无
*备  注：求解四元数和欧拉角都在此函数中完成
**********************************************************************************************************/	
//Kp=Ki=0 就是完全相信陀螺仪
#define Kp	1.50f		// proportional gain governs rate of convergence to accelerometer/magnetometer
						//比例增益控制加速度计，磁力计的收敛速率
#define Ki	0.005f		// integral gain governs rate of convergence of gyroscope biases  
						//积分增益控制陀螺偏差的收敛速度
#define halfT 0.005f	//// half the sample period 采样周期的一半

float q0 = 1,q1 = 0,q2 = 0,q3 = 0;// quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;	// scaled integral error	误差积分变量

void IMUupdate(FLOAT_XYZ *Gyr_rad, FLOAT_XYZ *Acc_filt, FLOAT_ANGLE *Att_Angle)
{

	float matrix[9] = {1.f, 0.0f, 0.0f, 0.0f, 1.f, 0.0f, 0.0f, 0.0f, 1.f};	//初始化矩阵
	float ax = Acc_filt->X, ay = Acc_filt->Y, az = Acc_filt->Z;	//加速度数据
	float gx = Gyr_rad->X, gy = Gyr_rad->Y, gz = Gyr_rad->Z;	//陀螺仪数据
	float vx, vy, vz;	//从四元素推导出来的理论重力加速度变量
	float ex, ey, ez;	//通过差积求出来的理论和实际重力加速度向量的误差
	float norm;			//求向量模
	
	//加快四元数方向余弦矩阵
	float q0q0 = q0*q0;
	float q0q1 = q0*q1;
	float q0q2 = q0*q2;
	float q0q3 = q0*q3;
	float q1q1 = q1*q1;
	float q1q2 = q1*q2;
	float q1q3 = q1*q3;
	float q2q2 = q2*q2;
	float q2q3 = q2*q3;
	float q3q3 = q3*q3;
	
	if(ax*ay*az == 0) return; //如果重力加速度在x轴或y轴的分量为0，则不需要融合，直接返回
	
	//加速剂计测量的重力向量单位化
	norm = invSqrt(ax*ax+ay*ay+az*az);
	ax = ax*norm;
	ay = ay*norm;
	az = az*norm;
	
	//陀螺仪积分估计重力向量(机体坐标系)
	vx = 2*(q1q3-q0q2);
	vy = 2*(q0q1+q2q3);
	vz = q0q0-q1q1-q2q2+q3q3;
	
	//测量的重力向量与估算的重力向量差积求出向量间的误差
	ex = (ay*vz-az*vy);
	ey = (az*vx-ax*vz);
	ez = (ax*vy-ay*vx);
	
	//用上面求出的误差进行积分
	exInt = exInt + ex*Ki;
	eyInt = eyInt + ey*Ki;
	ezInt = ezInt + ez*Ki;
	
	//将误差PI后补偿到陀螺仪
	gx = gx + Kp*ex + exInt;
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;	//gz由于没有观测者进行校正会产生漂移，表现出来的就是积分自增或自减
	
	//四元素的微分方程
	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;
	
	//单位化四元素
	norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0*norm;
	q1 = q1*norm;
	q2 = q2*norm;
	q3 = q3*norm;
	
	//计算方向余弦矩阵 将惯性坐标系(n)转换到机体坐标系(b) 用于定高 单纯的姿态解算不需要
	matrix[0] = q0q0 + q1q1 - q2q2 - q3q3;// 11(前列后行)
	matrix[1] = 2.f * (q1q2 + q0q3);	    // 12
	matrix[2] = 2.f * (q1q3 - q0q2);	    // 13
	matrix[3] = 2.f * (q1q2 - q0q3);	    // 21
	matrix[4] = q0q0 - q1q1 + q2q2 - q3q3;// 22
	matrix[5] = 2.f * (q2q3 + q0q1);	    // 23
	matrix[6] = 2.f * (q1q3 + q0q2);	    // 31
	matrix[7] = 2.f * (q2q3 - q0q1);	    // 32
	matrix[8] = q0q0 - q1q1 - q2q2 + q3q3;// 33
	
	//四元数转换成欧拉角（Z->Y->X）
	Att_Angle->yaw += Gyr_rad->Z*RadtoDeg*0.01f;
	//	Att_Angle->yaw = atan2(2.f * (q1q2 + q0q3), q0q0 + q1q1 - q2q2 - q3q3)* 57.3f; // yaw
	Att_Angle->pit = -asin(2.f*(q1*q3-q0*q2))*57.3f;
	Att_Angle->rol = atan2(2.f*q2*q3+2.f*q0*q1, q0*q0-q1*q1-q2*q2+q3*q3)*57.3f;
	for(int i = 0; i < 9; i++)
	{
		*(&(DCMgb[0][0])+i) = matrix[i];
	}
	//失控保护 (调试时可注释掉)
//	Safety_Check(); 
}



