/*
 * RemoteCtrl.c
 *
 *  Created on: 2025��1��12��
 *      Author: 20483
 */

#include "zf_common_headfile.h"

int   Control_Flag               = 0;  // ң�������Ʊ�־λ��ÿ����һ�η�תһ�Σ�����Ϊ1ʱ��ң��ģʽ��Ϊ0ʱ��ѭ��ģʽ��Ĭ��ѭ����Ϊ0��
int   Channal_3_Press_Flag       = 0;  // ͨ��3�����Ƿ���
int   last_Channal_3             = 0;  // ͨ��3��һ��״̬
int   RemoteCtrl_Speed;                // ң�����ٶȿ�����
int16 RemoteCtrl_Direction;            // ң�������������

void RemoteCtrl_Program()
{
    RemoteCtrl_Direction_Speed();
    Is_Channal_3_Press();
    CtrlMode_Switch();
}

void RemoteCtrl_Direction_Speed()
{
    if(Control_Flag == 1)
    {
        RemoteCtrl_Direction = (int16)((uart_receiver.channel[0] - 856) * 39 / 800);    // ��Ҫ�޸ģ�����ֵӳ�䵽SERVO_MOTOR_RMAX �� SERVO_MOTOR_LMAX
        Servo_Set(SERVO_MOTOR_MID - RemoteCtrl_Direction);                             // ����Ƕ�

        RemoteCtrl_Speed = (int)((uart_receiver.channel[1] - 1056) * 3000 / 800);                              // ��Ҫ�޸ģ�����ֵӳ�䵽-MAX_DUTY �� MAX_DUTY
        DRV8701_MOTOR_DRIVER(RemoteCtrl_Speed);
    }
    if(Control_Flag == 1)
    {
        pwm_set_duty(PWM_CH1, 0);                                  // ֹͣ
        Servo_Set(SERVO_MOTOR_MID);                               // ����Ƕ�
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
