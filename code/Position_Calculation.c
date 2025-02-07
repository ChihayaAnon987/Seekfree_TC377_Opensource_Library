/*
 * Position_Calculation.c
 *
 *  Created on: 2025��1��9��
 *      Author: 20483
 */

#include "zf_common_headfile.h"


int Track_Points_NUM  =   0;       // ��ǰ׷�ٵڼ�����
double Angle_Error    =   0;       // ������뺽���֮��
float  Fusion_angle   =   0;       // GPS��IMU�����˲���ĽǶ�
float  Fusion_alpha   = 0.9;       // GPS��IMU�����˲���Ȩ��
int16  Speed          = 100;       // �ٶ�

void Stright_Some_Distance()
{
    double distance;
    if(gnss_flag)
    {
        gnss_flag = 0;
        gnss_data_parse();           //��ʼ��������
        Start_Lat = gnss.latitude;
        Start_Lon = gnss.longitude;
    }
    Servo_Set(SERVO_MOTOR_MID);
    DRV8701_MOTOR_DRIVER(3000);
    distance = get_two_points_distance(Start_Lat, Start_Lon, gnss.latitude, gnss.longitude);
    while(distance < 10);
    if(gnss_flag)
    {
        gnss_flag = 0;
        gnss_data_parse();           //��ʼ��������
        Straight_Lat = gnss.latitude;
        Straight_Lon = gnss.longitude;
    }
    Delta_Lat = Start_Lat - GPS_GET_LAT[0];
    Delta_Lon = Start_Lon - GPS_GET_LOT[0];
    Delta_Angle = get_two_points_azimuth(Start_Lat, Start_Lon, Straight_Lat, Straight_Lon);
}

/****************************************************************************************************
//  @brief      �����ֵ�Z_360�����GPS��direction���л����ں�
//  @param      void
//  @return     void
//  @since
//  Sample usage:
****************************************************************************************************/
void GPS_IMU_Complementary_Filtering()
{
    if(Z_360 > 180)
    {
        Z_360 -= 360;
    }
    if(gnss.direction > 180)
    {
        gnss.direction -= 360;
    }
    Fusion_angle = Fusion_alpha * Z_360 + (1 - Fusion_alpha) * gnss.direction;
    if(Fusion_angle > 180)
    {
        Fusion_angle -= 360;
    }
    if(Fusion_angle < -180)
    {
        Fusion_angle += 360;
    }
}

// ����ѭ������
void Track_Follow()
{
    // ����ӵ�һ���㵽�ڶ�����ķ�λ��(��λ����)
    // ����ӵ�һ���㵽�ڶ�����ľ���(��λ��m)
    // Distance ��Ϊ�л���λ������
    // Ĭ�϶Ա�����  IMU�滮Ϊ+180��-180
    // �����λ����������ô�����δ�������ֵ��0
    //Angle += 180;  // ԭ��Ĭ�϶Ա�����������仰���ϣ���ʾ���Ϸ���

    // ������
    // Angle = Test_Angle;
    
    if((Angle - Z_360) > 180)
    {
        Angle_Error = Angle - Z_360 - 360;
    }
    else if((Angle - Z_360) < -180)
    {
        Angle_Error = Angle - Z_360 + 360;
    }
    else
    {
        Angle_Error = Angle - Z_360;
    }
    // �Ľ���
    // 1.Angle��GPS�ķ���ǣ�ͨ����GPS���˲������Եõ�����׼ȷ�ķ����
    // 2.Z_360��IMU�ĺ���ǣ�ͨ����IMU���˲������Եõ�����׼ȷ�ĺ���ǣ��������˲�����Ԫ�������������������ݴ���
    // 3.������PD����
    // 4.������PID����
    // 5.MPC���ƺ�����ǰ��
    
    if(Track_Points_NUM == 1 || Track_Points_NUM == 2)
    {
        DRV8701_MOTOR_DRIVER(3000);
    }



}

// �л���λ
void Point_Switch()
{
    if(Track_Points_NUM == 0) // ��ǰ��λ
    {
        if(Distance < 2)
        {
            Track_Points_NUM++;   // �л���һ��λ
        }
    }
    else if(Track_Points_NUM == 1 || Track_Points_NUM == 2 || Track_Points_NUM == 3) // ��ǰ��λ
    {
        if(Distance < 1)   // �������������ھ�����ո���ȷ���龳
        {
            Track_Points_NUM++;
        }
    }

}











