/*
 * Position_Calculation.c
 *
 *  Created on: 2025年1月9日
 *      Author: 20483
 */

#include "zf_common_headfile.h"


int Track_Points_NUM  =   0;       // 当前追踪第几个点
double Angle_Error    =   0;       // 方向角与航向角之差
int16  Speed          = 100;       // 速度

// 核心循迹程序
void Track_Follow()
{
    // 计算从第一个点到第二个点的方位角(单位：°)
    // 计算从第一个点到第二个点的距离(单位：m)
    // Distance 作为切换点位的依据
    // 默认对北发车  IMU规划为+180和-180
    // 如果点位在正北，那么方向角未经处理的值是0
    //Angle += 180;  // 原来默认对北发车，把这句话加上，表示对南发车

    if(Angle > 180)
    {
        Angle -= 360;
    }

    if((Angle - Z_360) > 180)// 当两角度之差大于180度时，则将差值减去360
    {
        Angle_Error = Angle - Z_360 - 360;// GPS的方向角-IMU经过PD处理的航向角
    }
    else if((Angle - Z_360) < -180)// 当两角度之差小于-180度时，则将差值加上360
    {
        Angle_Error = Angle - Z_360 + 360;// GPS的方向角-IMU经过PD处理的航向角
    }
    else
    {
        Angle_Error = Angle - Z_360;// GPS的方向角-IMU经过PD处理的航向角
    }
    // 改进点
    // 1.Angle是GPS的方向角，通过对GPS的滤波，可以得到更加准确的方向角
    // 2.Z_360是IMU的航向角，通过对IMU的滤波，可以得到更加准确的航向角（卡尔曼滤波和四元数，上面这两点是数据处理）
    // 3.加入舵机PD控制
    // 4.加入电机PID控制
    // 5.MPC控制和曲率前馈
    Servo_Set((int16)(SERVO_MOTOR_MID - Angle_Error));
    
    if(Track_Points_NUM == 1 || Track_Points_NUM == 2)
    {
        DRV8701_MOTOR_DRIVER(3000);
    }



}

// 切换点位
void Point_Switch()
{
    if(Track_Points_NUM == 0) // 当前点位
    {
        if(Distance < 2)
        {
            Track_Points_NUM++;   // 切换下一点位
        }
    }
    else if(Track_Points_NUM == 1 || Track_Points_NUM == 2 || Track_Points_NUM == 3) // 当前点位
    {
        if(Distance < 1)   // 特殊任务，适用于距离把握更精确的情境
        {
            Track_Points_NUM++;
        }
    }

}











