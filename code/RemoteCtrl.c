/*
 * RemoteCtrl.c
 *
 *  Created on: 2025年1月12日
 *      Author: 20483
 */

#include "zf_common_headfile.h"

int   Control_Flag               = 0;  // 遥控器控制标志位，每按下一次翻转一次，当它为1时是遥控模式，为0时是循迹模式，默认循迹（为0）
int   Channal_3_Press_Flag       = 0;  // 通道3按键是否按下
int   last_Channal_3             = 0;  // 通道3上一个状态
int   RemoteCtrl_Speed;                // 遥控器速度控制量
int16 RemoteCtrl_Direction;            // 遥控器方向控制量

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
        RemoteCtrl_Direction = (int16)((uart_receiver.channel[0] - 856) * 30 / 800);   // 需要修改，把其值映射到SERVO_MOTOR_RMAX 到 SERVO_MOTOR_LMAX
        Servo_Set(SERVO_MOTOR_MID - RemoteCtrl_Direction);                             // 舵机角度

        RemoteCtrl_Speed = (int)((uart_receiver.channel[1] - 1056) * 3000 / 800);                              // 需要修改，把其值映射到-MAX_DUTY 到 MAX_DUTY
        DRV8701_MOTOR_DRIVER(RemoteCtrl_Speed);
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
