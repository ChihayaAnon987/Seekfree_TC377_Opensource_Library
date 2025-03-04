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

/*
    参考推文：
    - 17届推文：[链接](https://mp.weixin.qq.com/s/vwganbbwu0eX2j-ZmxWk_A)
    - 18届推文：[链接](https://mp.weixin.qq.com/s/7Ezc6coE8QljQRkr4Ab2sg)
    - 20届推文：[链接](https://mp.weixin.qq.com/s/dY3w1BQd_STeICCmykcyXg)

    关键点总结如下：

    1. **17届方案**：
       - 使用GPS获取当前车头指向的方位角（`gnss.direction`）。
       - 由于GPS更新频率慢，无法完成方向闭环，因此通过陀螺仪计算转向角。
       - 已经被18届方案替代。

    2. **18届方案**：
       - 通过陀螺仪计算方位角（`Z_360`）。
       - 行驶过程中，车模抖动会导致方位角漂移，因此采用互补滤波结合GPS方向角和陀螺仪角度值，得到更稳定的方向角。
       - 同步GPS和陀螺仪的正方向，确保两者方向一致。

    3. **具体实现**：
       - 采用逐飞方案的第一种方法：以陀螺仪初始方向为正方向，方向指向车头。具体实现在函数`Stright_Some_Distance`中。

    4. **20届方案**：
       - 解决GPS导航中的点位漂移问题。
       - 记录发车位置的坐标（`GPS_GET_LAT[0]`, `GPS_GET_LON[0]`），并依次采集路径坐标（`GPS_GET_LAT[i]`, `GPS_GET_LON[i]`）。
       - 发车时，获取当前车的坐标（`Start_Lat`, `Start_Lon`），计算静态漂移值（`Delta_Lat`, `Delta_Lon`）。
       - 将后续所有采集的GPS坐标加上这个偏差值，避免GPS漂移。
       - 如果踩点时GPS数据已经偏移，可以通过显示GPS点位到屏幕，根据路径图手动修正部分偏移点位，结合GPS矫正导航，实现精准导航效果。
*/

/*
    Lat 0.000001 = 0.111319m
    Lon 0.000001 = 0.061010m
    Lat+0.000001 是 0°  (北)
    Lat-0.000001 是 180°(南)
    Lon+0.000001 是 90° (东)
    Lon-0.000001 是 270°(西)
*/


uint32 Point_NUM = 0;               // 已采集点数
float K_Gps      = 0.5;             // 衔接部分的权重
double FilterPoint_Lat = 0;         // 滤波后的纬度
double FilterPoint_Lon = 0;         // 滤波后的经度
double Start_Lat;                   // 发车的经度
double Start_Lon;                   // 发车的纬度
double Straight_Lat;                // 直行10-20m的经度
double Straight_Lon;                // 直行10-20m的纬度
double Delta_Lat    = 0;            // 漂移经度
double Delta_Lon    = 0;            // 漂移纬度
double Angle        = 0;            // 方位角
double Delta_Angle  = 0;            // GPS与陀螺仪的正方向偏差角
float  Gps_Yaw      = 0;            // GPS直接得到的偏航角
uint8  Gps_Yaw_Flag = 0;            // GPS直接得到的偏航角标志位
float  Gps_Yaw2     = 0;            // GPS得到的偏航角（累加和）
float  Yaw          = 0;            // 偏航角
uint8  Yaw_Times    = 0;            // 偏航角计数
float  Lat_Fix    = 1.0;            // 纬度修正系数
float  Lon_Fix    = 1.0;            // 经度修正系数
double Delta_x      = 0;            // 位移
double Delta_y      = 0;            // 位移
double Distance     = 0;            // 自身距下一个点的距离
double GPS_GET_LAT[NUM_GPS_DATA];   // 纬度
double GPS_GET_LOT[NUM_GPS_DATA];   // 经度
int8   Task1_Points = 5;            // 科目一所用点位数量
int8   Task2_Points = 10;           // 科目二所用点位数量（4个桶, 每多一个桶增加两个点位）
int8   Task3_Points = 9;           // 科目三所用点位数量
float  GpsDistance[NUM_GPS_DATA] = 
{
    5.0, 3.0, 2.5, 4.0,   0,   0,   0,   0, 1.5,   0,  // 0 - 9

    5.0, 1.5, 1.5, 1.5, 1.5, 1.0, 1.5, 1.5, 1.5, 4.0,  // 10 - 19
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,  // 20 - 29
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,  // 30 - 39
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,  // 40 - 49

    4.0, 1.0, 1.0, 1.0, 1.5, 1.5, 1.5, 1.5, 4.5, 0.0,  // 50 - 59
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,  // 60 - 69
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,  // 70 - 79
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,  // 80 - 89
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0   // 90 - 99
};  // 存储换点距离的数组
int16  GpsTgtEncod[NUM_GPS_DATA] = 
{
    6000, 7000, 3000, 4000, 7000,    0,    0,    0, 3000,    0,  // 0 - 9

    3500, 1700, 1700, 1700, 1700, 1700, 1700, 1700, 2000, 5500,  // 10 - 19
       0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  // 20 - 29
       0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  // 30 - 39
       0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  // 40 - 49

    3000, 2000, 2000, 3000, 3000, 3000, 2000, 2000, 5000,    0,  // 50 - 59
       0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  // 60 - 69
       0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  // 70 - 79
       0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  // 80 - 89
       0,    0,    0,    0,    0,    0,    0,    0,    0,    0   // 90 - 99
};  // 存储点位速度的数组


void Get_Gps()
{
    // gps数据接收与解析都是通过串口中断调用gps_uart_callback函数进行实现的
    // 数据解析完毕之后gnss_flag标志位会置1
    if(gnss_flag)
    {
        gnss_flag = 0;
        gnss_data_parse();           //开始解析数据
        FilterPoint_Lat = K_Gps * FilterPoint_Lat + (1 - K_Gps) * gnss.latitude;
        FilterPoint_Lon = K_Gps * FilterPoint_Lon + (1 - K_Gps) * gnss.longitude;
        Angle = get_two_points_azimuth(gnss.latitude - Delta_Lat, gnss.longitude - Delta_Lon, GPS_GET_LAT[Track_Points_NUM], GPS_GET_LOT[Track_Points_NUM]);
        Angle -= Delta_Angle;
        if(Angle > 180)
        {
            Angle -= 360;
        }
        if(Angle < -180)
        {
            Angle += 360;
        }
        Distance = get_two_points_distance(gnss.latitude - Delta_Lat, gnss.longitude - Delta_Lon, GPS_GET_LAT[Track_Points_NUM], GPS_GET_LOT[Track_Points_NUM]);
        // if(gnss.direction < 180)
        // {
        //     Gps_Yaw = gnss.direction;
        // }
        // if(gnss.direction > 180)
        // {
        //     Gps_Yaw = gnss.direction - 360;
        // }
        // Gps_Yaw_Flag = 1;
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
        Yaw = Gps_Yaw2 / 10;
        Gps_Yaw2 = 0;
        Yaw_Times = 0;
    }
}