/*
 * IMU.c
 *
 *  Created on: 2025年1月7日
 *      Author: 20483
 */

#include "zf_common_headfile.h"

gyro_param_t Gyro_Offset;// 零飘参数
IMU_param_t  IMU_Data;   // 加速度 角速度

float gyro_Offset_flag = 0;// 标志位
float Z_360 = 0;// Z轴规划

// 零飘初始化
float IMU_gyro_Offset_Init()
{
    // 初始化陀螺仪Z轴偏移量为0，准备开始数据采集
    Gyro_Offset.Zdata = 0;

    // 循环1000次，以大量样本为基础计算平均值，提高精度
    for(uint16_t i=0; i<1000;i++)
    {
        // 累加每次读取的陀螺仪XYZ轴数据
        Gyro_Offset.Xdata += imu963ra_gyro_x;
        Gyro_Offset.Ydata += imu963ra_gyro_y;
        Gyro_Offset.Zdata += imu963ra_gyro_z;

        // 延时5毫秒，10毫秒也可以
        system_delay_ms(5);
    }

    // 计算平均值，作为最终的XYZ轴偏移量
    Gyro_Offset.Xdata /= 1000;
    Gyro_Offset.Ydata /= 1000;
    Gyro_Offset.Zdata /= 1000;

    // 标志陀螺仪Z轴偏移量初始化完成，返回状态标志1
    return gyro_Offset_flag = 1;
}

// 积分出角度值
void IMU_YAW_integral()
{
    //陀螺仪角速度转换为弧度制角速度 : deg/s --> rad/s
    // 将采集到的数值转化为实际物理值，并对陀螺仪进行去零飘处理
    IMU_Data.gyro_z = imu963ra_gyro_transition((float)imu963ra_gyro_z - Gyro_Offset.Zdata);   // 需要修改，应该不需要 * PI / 180
    if(fabs(IMU_Data.gyro_z) < 0.35)
    {
        Z_360 -= 0;
    }
    else
    {
        Z_360 += IMU_Data.gyro_z * 0.01;              // (积分过程)本来是逆时针为正，现在改为顺时针为正
    }

    while(Z_360 < 0)
    {
        Z_360 += 360;
    }
    while(Z_360 > 360)
    {
        Z_360 -= 360;
    }

}

// IMU初始化
void IMU_Init()
{
    imu963ra_init();
    IMU_gyro_Offset_Init();         // 零飘初始化
}







