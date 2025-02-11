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

/*
    �ο����ģ�
    - 17�����ģ�[����](https://mp.weixin.qq.com/s/vwganbbwu0eX2j-ZmxWk_A)
    - 18�����ģ�[����](https://mp.weixin.qq.com/s/7Ezc6coE8QljQRkr4Ab2sg)
    - 20�����ģ�[����](https://mp.weixin.qq.com/s/dY3w1BQd_STeICCmykcyXg)

    �ؼ����ܽ����£�

    1. **17�췽��**��
       - ʹ��GPS��ȡ��ǰ��ͷָ��ķ�λ�ǣ�`gnss.direction`����
       - ����GPS����Ƶ�������޷���ɷ���ջ������ͨ�������Ǽ���ת��ǡ�
       - �Ѿ���18�췽�������

    2. **18�췽��**��
       - ͨ�������Ǽ��㷽λ�ǣ�`Z_360`����
       - ��ʻ�����У���ģ�����ᵼ�·�λ��Ư�ƣ���˲��û����˲����GPS����Ǻ������ǽǶ�ֵ���õ����ȶ��ķ���ǡ�
       - ͬ��GPS�������ǵ�������ȷ�����߷���һ�¡�

    3. **����ʵ��**��
       - ������ɷ����ĵ�һ�ַ������������ǳ�ʼ����Ϊ�����򣬷���ָ��ͷ������ʵ���ں���`Stright_Some_Distance`�С�

    4. **20�췽��**��
       - ���GPS�����еĵ�λƯ�����⡣
       - ��¼����λ�õ����꣨`GPS_GET_LAT[0]`, `GPS_GET_LON[0]`���������βɼ�·�����꣨`GPS_GET_LAT[i]`, `GPS_GET_LON[i]`����
       - ����ʱ����ȡ��ǰ�������꣨`Start_Lat`, `Start_Lon`�������㾲̬Ư��ֵ��`Delta_Lat`, `Delta_Lon`����
       - ���������вɼ���GPS����������ƫ��ֵ������GPSƯ�ơ�
       - ����ȵ�ʱGPS�����Ѿ�ƫ�ƣ�����ͨ����ʾGPS��λ����Ļ������·��ͼ�ֶ���������ƫ�Ƶ�λ�����GPS����������ʵ�־�׼����Ч����
*/

/*
    Lat 0.000001 = 0.111319m
    Lon 0.000001 = 0.061010m
    Lat+0.000001 �� 0��
    Lat-0.000001 �� 180��
    Lon+0.000001 �� 270��
    Lon-0.000001 �� 90��
*/


uint32 Point_NUM = 0;               // �Ѳɼ�����
float K_Gps      = 0.5;             // �νӲ��ֵ�Ȩ��
double FilterPoint_Lat = 0;         // �˲����γ��
double FilterPoint_Lon = 0;         // �˲���ľ���
// double Now_Lat      = 0;            // �������ԭ��ľ���
// double Now_Lon      = 0;            // �������ԭ���γ��
double Start_Lat;                   // �����ľ���
double Start_Lon;                   // ������γ��
double Straight_Lat;                // ֱ��10-20m�ľ���
double Straight_Lon;                // ֱ��10-20m��γ��
double Delta_Lat    = 0;            // Ư�ƾ���
double Delta_Lon    = 0;            // Ư��γ��
double Angle        = 0;            // ��λ��
double Delta_Angle  = 0;            // GPS�������ǵ�������ƫ���
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
double GPS_GET_LAT[NUM_GPS_DATA];   // γ��
double GPS_GET_LOT[NUM_GPS_DATA];   // ����



// �����ɵ㺯��
void GL_CRC()
{
    // KEY1����ͨ��3���¶����Լ�¼��λ
    if(key_get_state(KEY_1) == KEY_SHORT_PRESS || Channal_3_Press_Flag)
    {
        if(gnss.state == 1)
        {
            lat_union[Point_NUM].double_type = gnss.latitude; // ż������γ��latitude
            lon_union[Point_NUM].double_type = gnss.longitude;// �������澭��longitude
            Point_NUM++;
        }
    }

    if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
    {
        FLASH_SAV_GPS();
    }

    if(key_get_state(KEY_3) == KEY_SHORT_PRESS)           // KEY2��KEY3�����ʵ�ֶ�ĳ��Ŀ�����زɼ�
    {

        if(Point_NUM >= 0)
        {
            Point_NUM += 1;
        }
    }

    if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
    {
        if(Point_NUM > 0)
        {
            Point_NUM -= 1;
        }
    }


}


void Get_Gps()
{
    // gps���ݽ������������ͨ�������жϵ���gps_uart_callback��������ʵ�ֵ�
    // ���ݽ������֮��gnss_flag��־λ����1
    if(gnss_flag)
    {
        gnss_flag = 0;
        gnss_data_parse();           //��ʼ��������
        FilterPoint_Lat = K_Gps * FilterPoint_Lat + (1 - K_Gps) * gnss.latitude;
        FilterPoint_Lon = K_Gps * FilterPoint_Lon + (1 - K_Gps) * gnss.longitude;
        Angle = get_two_points_azimuth(gnss.latitude, gnss.longitude, GPS_GET_LAT[Track_Points_NUM] - Delta_Lat, GPS_GET_LOT[Track_Points_NUM] - Delta_Lon);
        Angle -= Delta_Angle;
        if(Angle > 180)
        {
            Angle -= 360;
        }
        if(Angle < -180)
        {
            Angle += 360;
        }
        Distance = get_two_points_distance(gnss.latitude, gnss.longitude, GPS_GET_LAT[Track_Points_NUM] - Delta_Lat, GPS_GET_LOT[Track_Points_NUM] - Delta_Lon);

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
        Gps_Yaw = Gps_Yaw2 / 10;
        Gps_Yaw2 = 0;
        Yaw_Times = 0;
    }
}
