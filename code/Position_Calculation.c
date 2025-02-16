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
int16  Target_Encoder =   0;       // ת��

void Stright_Some_Distance()
{
    Start_Lat = gnss.latitude;
    Start_Lon = gnss.longitude;

    Delta_Lat = Start_Lat - GPS_GET_LAT[0];
    Delta_Lon = Start_Lon - GPS_GET_LOT[0];

    double distance = 0;
    Angle_Error = 0;
    Target_Encoder = 1500;
    while(distance < 5)
    {
        distance = get_two_points_distance(Start_Lat, Start_Lon, gnss.latitude, gnss.longitude);
    }

    Straight_Lat = gnss.latitude;
    Straight_Lon = gnss.longitude;
    Delta_Angle = get_two_points_azimuth(Start_Lat, Start_Lon, Straight_Lat, Straight_Lon);
    Track_Points_NUM = 1;
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
    // Ĭ�϶Ա�����  IMU�滮Ϊ+180��-180
    // �����λ����������ô�����δ�������ֵ��0
    //Angle += 180;  // ԭ��Ĭ�϶Ա�����������仰���ϣ���ʾ���Ϸ���

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

    // ���ڿ�Ŀһ�������л������Խ����˵�ʵ�֣���Servo_Test����Task_Select��������һ��һ���˵����ǿ��Ե�
    // KEY1��KEY2��KEY3�ֱ��Ӧ��Ŀһ����Ŀ������Ŀ����Ԥ�����ĵ�λ��ĳ����Ŀ�����°�����ת��ĳ��Ŀ��Ӧ�ĵ�λ��
    // KEY4����������һ����־λ��KEY4������Ϊ�ߵ�ƽ���˳����������һ��������whileѭ��������ѭ��ѭ����
    // ˼·�����ˣ�ʵ��Ҳ�ܼ򵥣����������ˣ��ѿ�Ŀһ������ʵ������߼�

    switch(Track_Points_NUM)
    {
        case 0:
            // Stright_Some_Distance();
            break;
        case 1:
            Target_Encoder = 1500;
            break;
        case 2:
            Target_Encoder = 1500;
            break;
        case 3:
            Target_Encoder = 1500;
            break;
        case 4:
            Target_Encoder = 1500;
            break;
        case 5:
            Target_Encoder = 0;
            break;
        case 6:
            Target_Encoder = 0;
            break;
        case 7:
            Target_Encoder = 0;
            break;



        case 20:
            break;

        case 50:
            break;
        
        default:
            break;
    }



}

// �л���λ
void Point_Switch()
{

    switch(Track_Points_NUM)
    {
        // ��ʱ�ٶ���Ŀһ��5���㣬�����±�0�Ƿ�����
        case 0:
            break;
        case 1:
            if(Distance < 2)
            {
                Track_Points_NUM = 2;
                LED_Buzzer_Flag_Ctrl(BUZZER_PIN);
            }
            break;
        case 2:
            if(Distance < 2)
            {
                Track_Points_NUM = 3;
                LED_Buzzer_Flag_Ctrl(BUZZER_PIN);
            }
            break;
        case 3:
            if(Distance < 2)
            {
                Track_Points_NUM = 4;
                LED_Buzzer_Flag_Ctrl(BUZZER_PIN);
            }
            break;
        case 4:
            if(Distance < 2)
            {
                Track_Points_NUM = 5;
                LED_Buzzer_Flag_Ctrl(BUZZER_PIN);
            }
            break;
        case 5:
            if(Distance <2)
            {
                Track_Points_NUM = 5;
                LED_Buzzer_Flag_Ctrl(LED3);
            }
            break;
        
        default:
            break;

        // ��Ŀ������ΪGPSѭ�������ߵ���Ȼ�ȶ��������ٶ�����ʵ���ѣ���ʱ������

        // ��Ŀ���Ŷ���׼������ͨ��GPS�������Ŷ���������ͨ������ͷʶ���Ŷ��м��ɫPCV���ϡ�
        // 
    }

}











