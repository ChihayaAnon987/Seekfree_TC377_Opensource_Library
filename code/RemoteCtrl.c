/*
 * RemoteCtrl.c
 *
 *  Created on: 2025��1��12��
 *      Author: 20483
 */

#include "zf_common_headfile.h"

int   Control_Flag               = 0;  // ң�������Ʊ�־λ��ÿ����һ�η�תһ�Σ�����Ϊ1ʱ��ң��ģʽ��Ϊ0ʱ��ѭ��ģʽ��Ĭ��ѭ����Ϊ0��
int   Center_Flag                = 0;
int   Channal_3_Press_Flag       = 0;  // ͨ��3�����Ƿ���
int   Channal_5_Press_Flag       = 0;  // ͨ��5�����Ƿ���
int   Channal_6_Press_Flag       = 0;  // ͨ��6�����Ƿ���
int   last_Channal_3             = 0;  // ͨ��3��һ��״̬
int   last_Channal_5             = 0;  // ͨ��5��һ��״̬
int   last_Channal_6             = 0;  // ͨ��6��һ��״̬
int   RemoteCtrl_Speed;                // ң�����ٶȿ�����
int16 RemoteCtrl_Direction;            // ң�������������

void RemoteCtrl_Program()
{
    RemoteCtrl_Direction_Speed();
    // Is_Channal_3_Press();
    // Is_Channal_5_Press();
    // Is_Channal_6_Press();
    CtrlMode_Switch();
}

void RemoteCtrl_Direction_Speed()
{
    if(Control_Flag == 1)
    {
        RemoteCtrl_Direction = (int16)((856 - uart_receiver.channel[0]) * 24 / 800);    // ����ֵӳ�䵽SERVO_MOTOR_RMAX �� SERVO_MOTOR_LMAX
        static float CenterAngle;
        if(RemoteCtrl_Direction == 0)
        {
            if(Center_Flag == 0)
            {
                CenterAngle = angle[2];
                Center_Flag = 1;
            }
            if(Center_Flag == 1)
            {
                if(RemoteCtrl_Speed != 0)
                {
                    if(RemoteCtrl_Speed > 0)
                    {
                        Angle_Error = -(angle[2] - CenterAngle);
                    }
                    else
                    {
                        Angle_Error =  (angle[2] - CenterAngle);
                    }
                }
                else
                {
                    Angle_Error = 0;
                }

            }
        }
        if(RemoteCtrl_Direction != 0)
        {
            Center_Flag = 0;
            Servo_Set(SERVO_MOTOR_MID - RemoteCtrl_Direction);                          // ����Ƕ�
        }

        RemoteCtrl_Speed = (int)((uart_receiver.channel[1] - 1056) * 7000 / 800);       // ����ֵӳ�䵽-MAX_DUTY �� MAX_DUTY
        DRV8701_MOTOR_DRIVER(RemoteCtrl_Speed);

        // �Զ���λ
        if(Channal_5_Press_Flag == 1)
        {
            Start_Flag = 1;
            if(Task_Flag == 1)
            {
                GPS_GET_LAT[8] = GPS_GET_LAT[Task1_Start_Point];
                GPS_GET_LOT[8] = GPS_GET_LOT[Task1_Start_Point];
            }
            else if(Task_Flag == 2)
            {
                GPS_GET_LAT[8] = GPS_GET_LAT[Task2_Start_Point];
                GPS_GET_LOT[8] = GPS_GET_LOT[Task2_Start_Point];
            }
            else if(Task_Flag == 3)
            {
                GPS_GET_LAT[8] = GPS_GET_LAT[Task3_Start_Point];
                GPS_GET_LOT[8] = GPS_GET_LOT[Task3_Start_Point];
            }
            else
            {
                GPS_GET_LAT[8] = GPS_GET_LAT[Task1_Start_Point];
                GPS_GET_LOT[8] = GPS_GET_LOT[Task1_Start_Point];
            }
            Track_Points_NUM = 8;
        }
    }
}



void CtrlMode_Switch()
{
    if(uart_receiver.channel[3] == CHANNAL_LOW_LEVEL)
    {
        Control_Flag = 0;
    }
    if(uart_receiver.channel[3] == 992 || uart_receiver.channel[3] == CHANNAL_HIGH_LEVEL)
    {
        Control_Flag = 1;
    }
}


void Is_Channal_3_Press()
{
    if(Channal_3_Press_Flag == 1)
    {
        Channal_3_Press_Flag = 0;
    }
    if(uart_receiver.channel[2] == CHANNAL_HIGH_LEVEL && last_Channal_3 == CHANNAL_LOW_LEVEL)
    {
        Channal_3_Press_Flag = 1;
    }
    if(uart_receiver.channel[2] == CHANNAL_LOW_LEVEL && last_Channal_3 == CHANNAL_HIGH_LEVEL)
    {
        Channal_3_Press_Flag = 1;
    }
    last_Channal_3 = uart_receiver.channel[2];
}

void Is_Channal_5_Press()
{
    if(Channal_5_Press_Flag == 1)
    {
        Channal_5_Press_Flag = 0;
    }
    if(uart_receiver.channel[4] == CHANNAL_HIGH_LEVEL && last_Channal_5 == CHANNAL_LOW_LEVEL)
    {
        Channal_5_Press_Flag = 1;
    }
    if(uart_receiver.channel[4] == CHANNAL_LOW_LEVEL && last_Channal_5 == CHANNAL_HIGH_LEVEL)
    {
        Channal_5_Press_Flag = 1;
    }
    last_Channal_5 = uart_receiver.channel[4];
}

void Is_Channal_6_Press()
{
    if(Channal_6_Press_Flag == 1)
    {
        Channal_6_Press_Flag = 0;
    }
    if(uart_receiver.channel[5] == CHANNAL_HIGH_LEVEL && last_Channal_6 == CHANNAL_LOW_LEVEL)
    {
        Channal_6_Press_Flag = 1;
    }
    if(uart_receiver.channel[5] == CHANNAL_LOW_LEVEL && last_Channal_6 == CHANNAL_HIGH_LEVEL)
    {
        Channal_6_Press_Flag = 1;
    }
    last_Channal_6 = uart_receiver.channel[5];
}
