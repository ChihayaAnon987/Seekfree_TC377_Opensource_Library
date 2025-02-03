/*
 * RemoteCtrl.c
 *
 *  Created on: 2025��1��12��
 *      Author: 20483
 */

#include "zf_common_headfile.h"

int   Control_Flag               = 0;  // ң�������Ʊ�־λ��ÿ����һ�η�תһ�Σ�����Ϊ1ʱ��ң��ģʽ��Ϊ0ʱ��ѭ��ģʽ��Ĭ��ѭ����Ϊ0��
int   RemoteCtrl_Start_Stop_Flag = 0;  // ң������ͣ��־λ��ÿ����һ�η�תһ�Σ�����Ϊ1ʱ������״̬��Ϊ0ʱ��ֹͣ״̬��Ĭ��ֹͣ��Ϊ0��
int   RemoteCtrl_Speed;                // ң�����ٶȿ�����
int16 RemoteCtrl_Direction;            // ң�������������

void RemoteCtrl_Program()
{
    RemoteCtrl_Direction_Speed();
    RemoteCtrl_Start_Stop();
    CtrlMode_Switch();
}

void RemoteCtrl_Direction_Speed()
{
    if(Control_Flag == 1 && RemoteCtrl_Start_Stop_Flag == 1)
    {
        RemoteCtrl_Direction = (int16)((uart_receiver.channel[0] - 856) * 39 / 800);    // ��Ҫ�޸ģ�����ֵӳ�䵽SERVO_MOTOR_RMAX �� SERVO_MOTOR_LMAX
        Servo_Set(SERVO_MOTOR_MID - RemoteCtrl_Direction);                             // ����Ƕ�

        RemoteCtrl_Speed = (int)((uart_receiver.channel[1] - 1056) * 3000 / 800);                              // ��Ҫ�޸ģ�����ֵӳ�䵽-MAX_DUTY �� MAX_DUTY
        DRV8701_MOTOR_DRIVER(RemoteCtrl_Speed);
    }
    if(Control_Flag == 1 && RemoteCtrl_Start_Stop_Flag == 0)
    {
        pwm_set_duty(PWM_CH1, 0);                                  // ֹͣ
        Servo_Set(SERVO_MOTOR_MID);                               // ����Ƕ�
    }
}

void RemoteCtrl_Start_Stop()
{
    // if(uart_receiver.channel[2] == 1792)     // �ߵ�ƽֵ
    // {
    //     RemoteCtrl_Start_Stop_Flag = 1;
    // }
    // else
    // {
    //     RemoteCtrl_Start_Stop_Flag = 0;
    // }
    RemoteCtrl_Start_Stop_Flag = (uart_receiver.channel[2] == 1792);
}

void CtrlMode_Switch()
{
    // // ������յ���ͨ��4��ֵΪ�ߵ�ƽֵ���򽫿��Ʊ�־��Ϊ1
    // if(uart_receiver.channel[4] == 1792)     // �ߵ�ƽֵ
    // {
    //     Control_Flag = 1;
    // }
    // // ������յ���ͨ��4��ֵΪ�͵�ƽֵ���򽫿��Ʊ�־��Ϊ0
    // if(uart_receiver.channel[4] == 192)     // �͵�ƽֵ
    // {
    //     Control_Flag = 0;
    // }
    Control_Flag = (uart_receiver.channel[4] == 1792);
}
