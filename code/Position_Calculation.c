/*
 * Position_Calculation.c
 *
 *  Created on: 2025��1��9��
 *      Author: 20483
 */

#include "zf_common_headfile.h"


int Track_Points_NUM  =   0;       // ��ǰ׷�ٵڼ�����
double Angle_Error    =   0;       // ������뺽���֮��
int16  Speed          = 100;       // �ٶ�

// ����ѭ������
void Track_Follow()
{
    // ����ӵ�һ���㵽�ڶ�����ķ�λ��(��λ����)
    // ����ӵ�һ���㵽�ڶ�����ľ���(��λ��m)
    // Distance ��Ϊ�л���λ������
    // Ĭ�϶Ա�����  IMU�滮Ϊ+180��-180
    // �����λ����������ô�����δ�������ֵ��0
    //Angle += 180;  // ԭ��Ĭ�϶Ա�����������仰���ϣ���ʾ���Ϸ���

    if(Angle > 180)
    {
        Angle -= 360;
    }

    if((Angle - Z_360) > 180)// �����Ƕ�֮�����180��ʱ���򽫲�ֵ��ȥ360
    {
        Angle_Error = Angle - Z_360 - 360;// GPS�ķ����-IMU����PD����ĺ����
    }
    else if((Angle - Z_360) < -180)// �����Ƕ�֮��С��-180��ʱ���򽫲�ֵ����360
    {
        Angle_Error = Angle - Z_360 + 360;// GPS�ķ����-IMU����PD����ĺ����
    }
    else
    {
        Angle_Error = Angle - Z_360;// GPS�ķ����-IMU����PD����ĺ����
    }
    // �Ľ���
    // 1.Angle��GPS�ķ���ǣ�ͨ����GPS���˲������Եõ�����׼ȷ�ķ����
    // 2.Z_360��IMU�ĺ���ǣ�ͨ����IMU���˲������Եõ�����׼ȷ�ĺ���ǣ��������˲�����Ԫ�������������������ݴ���
    // 3.������PD����
    // 4.������PID����
    // 5.MPC���ƺ�����ǰ��
    Servo_Set((int16)(SERVO_MOTOR_MID - Angle_Error));
    
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











