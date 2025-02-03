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

//由于GPS会有漂移，所以一种方案是在踩点用发车位置的零点减去踩点的零点，从而得到漂移值
//后续点位均减去漂移值，就可以得到目标行驶路线
//但是由于踩点过程中也存在漂移，可以加快踩点速度

/*在比赛的时候，一般我们的发车位置是固定的，如果我们先记录发车的坐标，
再依次采集需要的路径坐标，后面轮到发车时，通过GPS当前的坐标数据与之
前采集的发车坐标做差，那么我们是否就得到了这段时间的GPS静态漂移值了呢？
如果我们将再后面所有采集的GPS坐标都加上这个偏差值，那么岂不是就完全避免
了这段时间的GPS漂移呢？非常正确，这就是更进一步的GPS矫正导航算法，这样我
们就已经可以利用GPS实现比较准确的路径导航了。?

如果踩点的时候，GPS数据就已经在偏移(踩点过程越慢，偏移的距离越远)，
那么还有办法解决么？当然有的，既然已经将采集的GPS点位显示到屏幕上了，
那么我们直接根据路径图修改部分已经偏移的点位，再结合GPS矫正导航，
即可实现非常精准的导航效果。

如果小车跑的时候的实时数据也飘了的话，可以结合惯导的方法
*/


uint32 Point_NUM = 0;               // 已采集点数
float K_Gps      = 0.5;             // 衔接部分的权重
double FilterPoint_Lat = 0;         // 滤波后的纬度
double FilterPoint_Lon = 0;         // 滤波后的经度
// double Now_Lat      = 0;            // 自身相对原点的经度
// double Now_Lon      = 0;            // 自身相对原点的纬度
double Start_Lat   = 0;             // 发车的经度
double Start_Lon   = 0;             // 发车的纬度
double Delta_Lat;                   // 漂移经度
double Delta_Lon;                   // 漂移纬度   
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

        // Now_Lat = gnss.latitude;
        // Now_Lon = gnss.longitude;

        Angle = get_two_points_azimuth(gnss.latitude, gnss.longitude, GPS_GET_LAT[Track_Points_NUM] - Delta_Lat, GPS_GET_LOT[Track_Points_NUM] - Delta_Lon);
        if(Angle > 180)
        {
            Angle -= 360;
        }
        Distance = get_two_points_distance(gnss.latitude, gnss.longitude, GPS_GET_LAT[Track_Points_NUM] - Delta_Lat, GPS_GET_LOT[Track_Points_NUM] - Delta_Lon);

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
