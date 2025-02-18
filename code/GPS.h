/*
 * GPS.h
 *
 *  Created on: 2025年1月8日
 *      Author: 20483
 */

#ifndef CODE_GPS_H_
#define CODE_GPS_H_

//===================================================宏定义BEG===================================================
#define NUM_GPS_DATA        (     100     )                  // GPS 采集点数
#define QS                  (   8.99266   )                  // 坐标变换常数
//===================================================宏定义END===================================================


//===================================================全局变量BEG===================================================
extern uint32 Point_NUM;                                       // 当前采集的 GPS 点数量
extern float  K_Gps;                                           // 衔接部分的权重
extern double FilterPoint_Lat;                                 // 滤波后的纬度
extern double FilterPoint_Lon;                                 // 滤波的经度
// extern double Now_Lat;                                         // 自身相对原点的纬度
// extern double Now_Lon;                                         // 自身相对原点的经度
extern double Start_Lat;                                       // 发车的经度
extern double Start_Lon;                                       // 发车的纬度
extern double Straight_Lat;                                    // 直行10-20m的经度
extern double Straight_Lon;                                    // 直行10-20m的纬度
extern double Delta_Lat;                                       // 漂移经度
extern double Delta_Lon;                                       // 漂移纬度
extern double Angle;                                           // 方位角
extern double Delta_Angle;                                     // GPS与陀螺仪的正方向偏差角
extern double Distance;                                        // 自身距下一个点的距离
extern float  Yaw;                                             // 偏航角
extern float  Gps_Yaw;                                         // GPS直接得到的偏航角
extern float  Gps_Yaw2;                                        // GPS得到的偏航角（累加和）
extern float  Lat_Fix;                                         // 纬度修正系数
extern float  Lon_Fix;                                         // 经度修正系数
extern double Delta_x;                                         // 位移
extern double Delta_y;                                         // 位移
extern double GPS_GET_LAT[NUM_GPS_DATA];                       // 存储纬度数据的数组
extern double GPS_GET_LOT[NUM_GPS_DATA];                       // 存储经度数据的数组
//===================================================全局变量END===================================================


//===================================================函数声明BEG===================================================
void GL_CRC(void);                                             // 按键采点函数
void Get_Gps(void);                                            // 获取坐标等信息
void Get_Gps_Yaw(void);                                        // 获取GPS偏航角
//===================================================函数声明END===================================================


#endif /* CODE_GPS_H_ */
