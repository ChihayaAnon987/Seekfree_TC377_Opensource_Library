/*
 * IMU.c
 *
 *  Created on: 2025��1��7��
 *      Author: 20483
 */

#include "zf_common_headfile.h"

gyro_param_t Gyro_Offset;       // ��Ʈ����
IMU_param_t  IMU_Data;          // ���ٶ� ���ٶ�
float MAX_IMU_Data_gyro_z;      // Z�ᶶ�����ֵ
float gyro_Offset_flag = 0;     // ��־λ
float Z_360 = 0;                // Z��滮

// ��Ʈ��ʼ��
float IMU_gyro_Offset_Init()
{
    Gyro_Offset.Xdata = 0;
    Gyro_Offset.Ydata = 0;
    Gyro_Offset.Zdata = 0;
    // ѭ��1000�Σ��Դ�������Ϊ��������ƽ��ֵ����߾���
    for(uint16_t i = 0; i < 1000; i++)
    {
        imu963ra_get_gyro();
        Gyro_Offset.Xdata += imu963ra_gyro_x;
        Gyro_Offset.Ydata += imu963ra_gyro_y;
        Gyro_Offset.Zdata += imu963ra_gyro_z;
        system_delay_ms(5);
    }
    Gyro_Offset.Xdata /= 1000;
    Gyro_Offset.Ydata /= 1000;
    Gyro_Offset.Zdata /= 1000;

    return gyro_Offset_flag = 1;
}

// ���ֳ��Ƕ�ֵ
void IMU_YAW_integral()
{
    // �����ǽ��ٶ�ת��Ϊ�����ƽ��ٶ� : deg/s --> rad/s
    // ���ɼ�������ֵת��Ϊʵ������ֵ�����������ǽ���ȥ��Ʈ����
    // ������
    if(IMU_Data.gyro_z > MAX_IMU_Data_gyro_z)
    {
        MAX_IMU_Data_gyro_z = IMU_Data.gyro_z;
    }
    
    IMU_Data.gyro_z = imu963ra_gyro_transition(imu963ra_gyro_z - Gyro_Offset.Zdata);   // ��Ҫ�޸ģ�Ӧ�ò���Ҫ * PI / 180
    if(fabs(IMU_Data.gyro_z) >= 0.25)
    {
        Z_360 -= IMU_Data.gyro_z * 0.005;
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

// IMU��ʼ��
void IMU_Init()
{
    imu963ra_init();
//    IMU_gyro_Offset_Init();         // ��Ʈ��ʼ��
}







