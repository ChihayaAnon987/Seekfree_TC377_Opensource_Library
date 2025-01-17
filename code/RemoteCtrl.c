/*
 * RemoteCtrl.c
 *
 *  Created on: 2025年1月12日
 *      Author: 20483
 */

#include "zf_common_headfile.h"

int   Control_Flag               = 0;  // 遥控器控制标志位，每按下一次翻转一次，当它为1时是遥控模式，为0时是循迹模式，默认循迹（为0）
int   RemoteCtrl_Start_Stop_Flag = 0;  // 遥控器启停标志位，每按下一次翻转一次，当它为1时是启动状态，为0时是停止状态，默认停止（为0）
int   RemoteCtrl_Speed;                // 遥控器速度控制量
int16 RemoteCtrl_Direction;            // 遥控器方向控制量

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
        RemoteCtrl_Direction = (int16)((uart_receiver.channel[0] - 856) * 39 / 800);    // 需要修改，把其值映射到SERVO_MOTOR_RMAX 到 SERVO_MOTOR_LMAX
        Servo_Set2(SERVO_MOTOR_MID + RemoteCtrl_Direction);                             // 舵机角度

        RemoteCtrl_Speed = uart_receiver.channel[1] - 192;                              // 需要修改，把其值映射到-MAX_DUTY 到 MAX_DUTY
        //DRV8701_MOTOR_DRIVER(RemoteCtrl_Speed);                                       // 电机速度
    }
    if(Control_Flag == 1 && RemoteCtrl_Start_Stop_Flag == 0)
    {
        pwm_set_duty(PWM_CH1, 0);                                  // 停止
        Servo_Set2(SERVO_MOTOR_MID);                               // 舵机角度
    }
}

void RemoteCtrl_Start_Stop()
{
    if(uart_receiver.channel[2] == 1792)     // 高电平值
    {
        RemoteCtrl_Start_Stop_Flag = 1;
    }
    else
    {
        RemoteCtrl_Start_Stop_Flag = 0;
    }
}

void CtrlMode_Switch()
{
    if(uart_receiver.channel[4] == 1792)     // 高电平值
    {
        Control_Flag = 1;
    }
    if(uart_receiver.channel[4] == 192)     // 低电平值
    {
        Control_Flag = 0;
    }
}
