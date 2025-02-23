/*
 * Position_Calculation.c
 *
 *  Created on: 2025��1��9��
 *      Author: 20483
 */

#include "zf_common_headfile.h"

int Track_Points_NUM   =   0;       // ��ǰ׷�ٵڼ�����
double Angle_Error     =   0;       // ������뺽���֮��
float  Fusion_angle    =   0;       // GPS��IMU�����˲���ĽǶ�
float  Fusion_alpha    = 0.9;       // GPS��IMU�����˲���Ȩ��
int16  Target_Encoder  =   0;       // ת��
int16  Fly_Slope_Alpha = 200;       // ����ϵ��
float  K_Straight      = 1.7;       // ��ֱ��ϵ��


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

/****************************************************************************************************
//  @brief      ����ѭ���߼�
//  @param      void
//  @return     void
//  @since
//  Sample usage:
****************************************************************************************************/
void Track_Follow()
{
    // ����ӵ�һ���㵽�ڶ�����ķ�λ��(��λ����)
    // ����ӵ�һ���㵽�ڶ�����ľ���(��λ��m)
    // Distance ��Ϊ�л���λ������

    // ������
    // Angle = Test_Angle;

    if((Angle - angle[2]) > 180)
    {
        Angle_Error = Angle - angle[2] - 360;
    }
    else if((Angle - angle[2]) < -180)
    {
        Angle_Error = Angle - angle[2] + 360;
    }
    else
    {
        Angle_Error = Angle - angle[2];
    }
    // �Ľ���
    // 1.Angle��GPS�ķ���ǣ�ͨ����GPS���˲������Եõ�����׼ȷ�ķ����
    // 2.Z_360��IMU�ĺ���ǣ�ͨ����IMU���˲������Եõ�����׼ȷ�ĺ���ǣ��������˲�����Ԫ�������������������ݴ���
    // 3.������PD����
    // 4.������PID����
    // 5.MPC���ƺ�����ǰ��
    // 1234����ʵ�ֵĲ�࣬�ȴ�ʵ�ʲ���

    if(Track_Points_NUM == Task1_Start_Point || Track_Points_NUM == Task2_Start_Point || Track_Points_NUM == Task3_Start_Point)
    {
        Angle_Error = -angle[2];
    }
    Target_Encoder = GpsTgtEncod[Track_Points_NUM];

    if(fabs(angle[0]) > 20)
    {
        Target_Encoder -= fabs(fabs(angle[0]) - 20) * Fly_Slope_Alpha;
        if(Target_Encoder <= 1500)
        {
            Target_Encoder = 1500;
        }
    }
}

// �л���λ
void Point_Switch()
{
    Distance = get_two_points_distance(gnss.latitude - Delta_Lat, gnss.longitude - Delta_Lon, GPS_GET_LAT[Track_Points_NUM], GPS_GET_LOT[Track_Points_NUM]);
    if(Track_Points_NUM != Task1_Start_Point && Track_Points_NUM != Task2_Start_Point && Track_Points_NUM != Task3_Start_Point)
    {
        if(Distance < GpsDistance[Track_Points_NUM])
        {
            Track_Points_NUM ++;
            LED_Buzzer_Flag_Ctrl(BUZZER_PIN);
        }
    }
    else
    {
        if (Distance > GpsDistance[Track_Points_NUM])
        {
            Track_Points_NUM ++;
            LED_Buzzer_Flag_Ctrl(BUZZER_PIN);
            Delta_Angle = get_two_points_azimuth(Start_Lat, Start_Lon, gnss.latitude, gnss.longitude);
        }
    }

}











