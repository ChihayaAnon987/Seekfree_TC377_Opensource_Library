/*
 * GPS.c
 *
 *  Created on: 2025年1月8日
 *      Author: 20483
 */

#include "zf_common_headfile.h"

//1.确定坐标点
//2.利用坐标点代入角度计算函数----方向角
//3.IMU---------------------------航向角
//4.方向角-航向角=角度误差
//存坐标点-----1.用FLASH（可以断电）      2.数组


uint32 Point_NUM = 0;               // 已采集点数
float K_Gps      = 0.5;             // 衔接部分的权重
double FilterPoint_Lat = 0;         // 滤波后的纬度
double FilterPoint_Lon = 0;         // 滤波后的经度
// double Now_Lat      = 0;            // 自身相对原点的经度
// double Now_Lon      = 0;            // 自身相对原点的纬度
// double Origin_Lat   = 0;            // 新建坐标系的原点对应的经度
// double Origin_Lon   = 0;            // 新建坐标系的原点对应的纬度
double Angle        = 0;            // 方位角
float  Gps_Yaw      = 0;            // GPS直接得到的偏航角
uint8  Gps_Yaw_Flag = 0;            // GPS直接得到的偏航角标志位
float  Gps_Yaw2     = 0;            // GPS得到的偏航角（累加和）
float  Yaw          = 0;            // 偏航角
uint8  Yaw_Times    = 0;            // 偏航角计数
float  Lat_Fix    = 1.0;            // 纬度修正系数
float  Lon_Fix    = 1.0;            // 经度修正系数
double Delta_x    = 0;              // 位移
double Delta_y    = 0;              // 位移
double Distance     = 0;            // 自身距下一个点的距离
double GPS_GET_LAT[NUM_GPS_DATA];// 纬度
double GPS_GET_LOT[NUM_GPS_DATA];// 经度



// 按键采点函数
void GL_CRC()
{
    if(key_get_state(KEY_1) == KEY_SHORT_PRESS)                                               //key1 按下
    {
        if(gnss.state == 1)
        {
            lat_union[Point_NUM].double_type = gnss.latitude; // 偶数储存纬度latitude
            lon_union[Point_NUM].double_type = gnss.longitude;// 奇数储存经度longitude
            Point_NUM++;
        }
    }

    if(key_get_state(KEY_2) == KEY_SHORT_PRESS)           // KEY2和KEY3配合以实现对某个目标点的重采集
    {

        if(Point_NUM >= 0)
        {
            Point_NUM += 1;
        }
    }

    if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
    {
        if(Point_NUM > 0)
        {
            Point_NUM -= 1;
        }
    }

    if(key_get_state(KEY_4)==KEY_SHORT_PRESS)           //记录特殊点
    {


    }


}


void Get_Gps()
{
    //gps数据接收与解析都是通过串口中断调用gps_uart_callback函数进行实现的
    //数据解析完毕之后gps_tau1201_flag标志位会置1
    if(gnss_flag)
    {
        gnss_flag = 0;
        gnss_data_parse();           //开始解析数据
        FilterPoint_Lat = K_Gps * FilterPoint_Lat + (1 - K_Gps) * gnss.latitude;
        FilterPoint_Lon = K_Gps * FilterPoint_Lon + (1 - K_Gps) * gnss.longitude;

        // Now_Lat = gnss.latitude - Origin_Lat;
        // Now_Lon = gnss.longitude - Origin_Lon;

        Angle = get_two_points_azimuth(gnss.latitude, gnss.longitude, GPS_GET_LAT[Track_Points_NUM], GPS_GET_LOT[Track_Points_NUM]);
        if(Angle > 180)
        {
            Angle -= 360;
        }
        Distance = get_two_points_distance(gnss.latitude, gnss.longitude, GPS_GET_LAT[Track_Points_NUM], GPS_GET_LOT[Track_Points_NUM]);

        if(gnss.direction < 180)
        {
            Gps_Yaw = gnss.direction;
        }
        if(gnss.direction > 180)
        {
            Gps_Yaw = gnss.direction - 360;
        }
        Gps_Yaw_Flag = 1;
    }
    FilterPoint_Lat += (Delta_y * QS * 0.000000001 * Lat_Fix);
    FilterPoint_Lon += (Delta_x * QS * 0.000000001 * Lon_Fix) / (cos(FilterPoint_Lat * PI / 180));
}

void Get_Gps_Yaw()
{
    if(Gps_Yaw_Flag == 1)
    {
        Yaw_Times++;
        Gps_Yaw2 += Gps_Yaw;
        Gps_Yaw_Flag = 0;
    }
    if(Yaw_Times == 10)
    {
        Gps_Yaw = Gps_Yaw2 / 10;
        Gps_Yaw2 = 0;
        Yaw_Times = 0;
    }
}
