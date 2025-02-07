/*
 * Position_Calculation.c
 *
 *  Created on: 2025年1月9日
 *      Author: 20483
 */

#include "zf_common_headfile.h"


int Track_Points_NUM  =   0;       // 当前追踪第几个点
double Angle_Error    =   0;       // 方向角与航向角之差
float  Fusion_angle   =   0;       // GPS和IMU互补滤波后的角度
float  Fusion_alpha   = 0.9;       // GPS和IMU互补滤波的权重
int16  Speed          = 100;       // 速度

void Stright_Some_Distance()
{
    double distance;
    if(gnss_flag)
    {
        gnss_flag = 0;
        gnss_data_parse();           //开始解析数据
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
        gnss_data_parse();           //开始解析数据
        Straight_Lat = gnss.latitude;
        Straight_Lon = gnss.longitude;
    }
    Delta_Lat = Start_Lat - GPS_GET_LAT[0];
    Delta_Lon = Start_Lon - GPS_GET_LOT[0];
    Delta_Angle = get_two_points_azimuth(Start_Lat, Start_Lon, Straight_Lat, Straight_Lon);
}

/****************************************************************************************************
//  @brief      将积分的Z_360和逐飞GPS的direction进行互补融合
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

// 核心循迹程序
void Track_Follow()
{
    // 计算从第一个点到第二个点的方位角(单位：°)
    // 计算从第一个点到第二个点的距离(单位：m)
    // Distance 作为切换点位的依据
    // 默认对北发车  IMU规划为+180和-180
    // 如果点位在正北，那么方向角未经处理的值是0
    //Angle += 180;  // 原来默认对北发车，把这句话加上，表示对南发车

    // 调试用
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
    // 改进点
    // 1.Angle是GPS的方向角，通过对GPS的滤波，可以得到更加准确的方向角
    // 2.Z_360是IMU的航向角，通过对IMU的滤波，可以得到更加准确的航向角（卡尔曼滤波和四元数，上面这两点是数据处理）
    // 3.加入舵机PD控制
    // 4.加入电机PID控制
    // 5.MPC控制和曲率前馈
    
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











