/*
 * GPS.c
 *
 *  Created on: 2025��1��8��
 *      Author: 20483
 */

#include "zf_common_headfile.h"

//1.ȷ�������
//2.������������Ƕȼ��㺯��----�����
//3.IMU---------------------------�����
//4.�����-�����=�Ƕ����
//�������-----1.��FLASH�����Զϵ磩      2.����

//����GPS����Ư�ƣ�����һ�ַ������ڲȵ��÷���λ�õ�����ȥ�ȵ����㣬�Ӷ��õ�Ư��ֵ
//������λ����ȥƯ��ֵ���Ϳ��Եõ�Ŀ����ʻ·��
//�������ڲȵ������Ҳ����Ư�ƣ����Լӿ�ȵ��ٶ�

/*�ڱ�����ʱ��һ�����ǵķ���λ���ǹ̶��ģ���������ȼ�¼���������꣬
�����βɼ���Ҫ��·�����꣬�����ֵ�����ʱ��ͨ��GPS��ǰ������������֮
ǰ�ɼ��ķ������������ô�����Ƿ�͵õ������ʱ���GPS��̬Ư��ֵ���أ�
������ǽ��ٺ������вɼ���GPS���궼�������ƫ��ֵ����ô���Ǿ���ȫ����
�����ʱ���GPSƯ���أ��ǳ���ȷ������Ǹ���һ����GPS���������㷨��������
�Ǿ��Ѿ���������GPSʵ�ֱȽ�׼ȷ��·�������ˡ�?

����ȵ��ʱ��GPS���ݾ��Ѿ���ƫ��(�ȵ����Խ����ƫ�Ƶľ���ԽԶ)��
��ô���а취���ô����Ȼ�еģ���Ȼ�Ѿ����ɼ���GPS��λ��ʾ����Ļ���ˣ�
��ô����ֱ�Ӹ���·��ͼ�޸Ĳ����Ѿ�ƫ�Ƶĵ�λ���ٽ��GPS����������
����ʵ�ַǳ���׼�ĵ���Ч����

���С���ܵ�ʱ���ʵʱ����ҲƮ�˵Ļ������Խ�Ϲߵ��ķ���
*/


uint32 Point_NUM = 0;               // �Ѳɼ�����
float K_Gps      = 0.5;             // �νӲ��ֵ�Ȩ��
double FilterPoint_Lat = 0;         // �˲����γ��
double FilterPoint_Lon = 0;         // �˲���ľ���
// double Now_Lat      = 0;            // �������ԭ��ľ���
// double Now_Lon      = 0;            // �������ԭ���γ��
double Start_Lat   = 0;             // �����ľ���
double Start_Lon   = 0;             // ������γ��
double Delta_Lat;                   // Ư�ƾ���
double Delta_Lon;                   // Ư��γ��   
double Angle        = 0;            // ��λ��
float  Gps_Yaw      = 0;            // GPSֱ�ӵõ���ƫ����
uint8  Gps_Yaw_Flag = 0;            // GPSֱ�ӵõ���ƫ���Ǳ�־λ
float  Gps_Yaw2     = 0;            // GPS�õ���ƫ���ǣ��ۼӺͣ�
float  Yaw          = 0;            // ƫ����
uint8  Yaw_Times    = 0;            // ƫ���Ǽ���
float  Lat_Fix    = 1.0;            // γ������ϵ��
float  Lon_Fix    = 1.0;            // ��������ϵ��
double Delta_x    = 0;              // λ��
double Delta_y    = 0;              // λ��
double Distance     = 0;            // �������һ����ľ���
double GPS_GET_LAT[NUM_GPS_DATA];// γ��
double GPS_GET_LOT[NUM_GPS_DATA];// ����



// �����ɵ㺯��
void GL_CRC()
{
    if(key_get_state(KEY_1) == KEY_SHORT_PRESS)                                               //key1 ����
    {
        if(gnss.state == 1)
        {
            lat_union[Point_NUM].double_type = gnss.latitude; // ż������γ��latitude
            lon_union[Point_NUM].double_type = gnss.longitude;// �������澭��longitude
            Point_NUM++;
        }
    }

    if(key_get_state(KEY_2) == KEY_SHORT_PRESS)           // KEY2��KEY3�����ʵ�ֶ�ĳ��Ŀ�����زɼ�
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

    if(key_get_state(KEY_4)==KEY_SHORT_PRESS)           //��¼�����
    {


    }


}


void Get_Gps()
{
    //gps���ݽ������������ͨ�������жϵ���gps_uart_callback��������ʵ�ֵ�
    //���ݽ������֮��gps_tau1201_flag��־λ����1
    if(gnss_flag)
    {
        gnss_flag = 0;
        gnss_data_parse();           //��ʼ��������
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
