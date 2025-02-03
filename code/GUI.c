/*
 * GUI.c
 *
 *  Created on: 2025��1��10��
 *      Author: 20483
 */

#include "zf_common_headfile.h"

Parameter_set Parameter_set0=
{
    {2.0, 0.0, 1.0},            // ���PID
    {0.0, 0.0, 0.0},            // �ٶ�PID
    3000,                       // ���Ե��ٶ�
    2.5,                        // �������
    SERVO_MOTOR_MID             // �����е�ɵ���ֵ
};

int func_index = 0;
int key_value;
int Point = 0;   // �˵���λ

menu_table table[25]=
{
    // current, up, down, back, enter

    // �˵�0
    { 0, 23,  2,  0,  1, main_menu0},       // �ȵ�GPSһ��
    { 1,  1,  1,  0,  1, CaiDian_menu},     // �ȵ�GPS����

    // �˵�1
    { 2,  0,  7,  2,  3, main_menu1},       // PIDһ��
    { 3,  3,  3,  2,  4, PID_menu},         // PID����
    { 4,  4,  5,  3,  4, P_menu},           // P
    { 5,  4,  6,  3,  5, I_menu},           // I
    { 6,  5,  6,  3,  6, D_menu},           // D

    // �˵�2
    { 7,  2,  9,  7,  8, main_menu2},        // ����GPS��ʾһ��
    { 8,  8,  8,  7,  8, GPS_menu},          // ����GPS��ʾ����

    // �˵�3
    { 9,  7, 13,  9, 10, main_menu3},        // ����,�����е��ֵ���������
    {10, 10, 11,  9, 10, spd_menu},          // ����
    {11, 10, 12,  9, 11, Distance_menu},     // �������
    {12, 11, 12,  9, 12, serve_mid_menu},    // �����ֵ

    // �˵�4
    {13,  9, 15, 13, 14, main_menu4},        // ң��ģʽһ��
    {14, 14, 14, 13, 14, RemoteCtrl_menu},   // ң��ģʽ����

    // �˵�5
    {15, 13, 17, 15, 16, main_menu5},        // GPS��λ�鿴һ��
    {16, 16, 16, 15, 16, Points_menu},       // GPS��λ�鿴����

    // �˵�6
    {17, 15, 19, 17, 18, main_menu6},        // ����ͷͼ��һ��
    {18, 18, 18, 17, 18, ZongZuanF} ,        // ����ͷͼ�����

    // �˵�7
    {19, 17, 21, 19, 20, main_menu7},        // ������һ��
    {20, 20, 20, 19, 20, Imu963_menu},       // �����Ƕ���

    // �˵�8
    {21, 19, 23, 22, 22, main_menu8},        // Flashһ��
    {22, 22, 22, 21, 22, Flash_menu},        // Flash����

    // �˵�9
    {23, 21,  0, 23, 24, main_menu9},        // �������һ��
    {24, 24, 24, 23, 24, Servo_menu}         // ������Զ���



};

/////////////////////////////////һ��˵�-------------------------------------------------
void main_menu0(void)
{
    ips200_show_string(  0, 16 * 0, "-->CaiDian   ");
    ips200_show_string(  0, 16 * 1, "   PID       ");
    ips200_show_string(  0, 16 * 2, "   GPS Show  ");
    ips200_show_string(  0, 16 * 3, "   Speed     ");
    ips200_show_string(  0, 16 * 4, "   RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "   Points    ");
    ips200_show_string(  0, 16 * 6, "   Camera    ");
    ips200_show_string(  0, 16 * 7, "   Imu963    ");
    ips200_show_string(110, 16 * 0, "   Flash     ");
    ips200_show_string(110, 16 * 1, "   SevroTest ");
}

void main_menu1(void)
{
    ips200_show_string(  0, 16 * 0, "   CaiDian   ");
    ips200_show_string(  0, 16 * 1, "-->PID       ");
    ips200_show_string(  0, 16 * 2, "   GPS Show  ");
    ips200_show_string(  0, 16 * 3, "   Speed     ");
    ips200_show_string(  0, 16 * 4, "   RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "   Points    ");
    ips200_show_string(  0, 16 * 6, "   Camera    ");
    ips200_show_string(  0, 16 * 7, "   Imu963    ");
    ips200_show_string(110, 16 * 0, "   Flash     ");
    ips200_show_string(110, 16 * 1, "   SevroTest ");
}

void main_menu2(void)
{
    ips200_show_string(  0, 16 * 0, "   CaiDian   ");
    ips200_show_string(  0, 16 * 1, "   PID       ");
    ips200_show_string(  0, 16 * 2, "-->GPS Show  ");
    ips200_show_string(  0, 16 * 3, "   Speed     ");
    ips200_show_string(  0, 16 * 4, "   RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "   Points    ");
    ips200_show_string(  0, 16 * 6, "   Camera    ");
    ips200_show_string(  0, 16 * 7, "   Imu963    ");
    ips200_show_string(110, 16 * 0, "   Flash     ");
    ips200_show_string(110, 16 * 1, "   SevroTest ");
}

void main_menu3(void)
{
    ips200_show_string(  0, 16 * 0, "   CaiDian   ");
    ips200_show_string(  0, 16 * 1, "   PID       ");
    ips200_show_string(  0, 16 * 2, "   GPS Show  ");
    ips200_show_string(  0, 16 * 3, "-->Speed     ");
    ips200_show_string(  0, 16 * 4, "   RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "   Points    ");
    ips200_show_string(  0, 16 * 6, "   Camera    ");
    ips200_show_string(  0, 16 * 7, "   Imu963    ");
    ips200_show_string(110, 16 * 0, "   Flash     ");
    ips200_show_string(110, 16 * 1, "   SevroTest ");
}

void main_menu4(void)
{
    ips200_show_string(  0, 16 * 0, "   CaiDian   ");
    ips200_show_string(  0, 16 * 1, "   PID       ");
    ips200_show_string(  0, 16 * 2, "   GPS Show  ");
    ips200_show_string(  0, 16 * 3, "   Speed     ");
    ips200_show_string(  0, 16 * 4, "-->RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "   Points    ");
    ips200_show_string(  0, 16 * 6, "   Camera    ");
    ips200_show_string(  0, 16 * 7, "   Imu963    ");
    ips200_show_string(110, 16 * 0, "   Flash     ");
    ips200_show_string(110, 16 * 1, "   SevroTest ");
}

void main_menu5(void)
{
    ips200_show_string(  0, 16 * 0, "   CaiDian   ");
    ips200_show_string(  0, 16 * 1, "   PID       ");
    ips200_show_string(  0, 16 * 2, "   GPS Show  ");
    ips200_show_string(  0, 16 * 3, "   Speed     ");
    ips200_show_string(  0, 16 * 4, "   RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "-->Points    ");
    ips200_show_string(  0, 16 * 6, "   Camera    ");
    ips200_show_string(  0, 16 * 7, "   Imu963    ");
    ips200_show_string(110, 16 * 0, "   Flash     ");
    ips200_show_string(110, 16 * 1, "   SevroTest ");
}

void main_menu6(void)
{
    ips200_show_string(  0, 16 * 0, "   CaiDian   ");
    ips200_show_string(  0, 16 * 1, "   PID       ");
    ips200_show_string(  0, 16 * 2, "   GPS Show  ");
    ips200_show_string(  0, 16 * 3, "   Speed     ");
    ips200_show_string(  0, 16 * 4, "   RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "   Points    ");
    ips200_show_string(  0, 16 * 6, "-->Camera    ");
    ips200_show_string(  0, 16 * 7, "   Imu963    ");
    ips200_show_string(110, 16 * 0, "   Flash     ");
    ips200_show_string(110, 16 * 1, "   SevroTest ");
}

void main_menu7(void)
{
    ips200_show_string(  0, 16 * 0, "   CaiDian   ");
    ips200_show_string(  0, 16 * 1, "   PID       ");
    ips200_show_string(  0, 16 * 2, "   GPS Show  ");
    ips200_show_string(  0, 16 * 3, "   Speed     ");
    ips200_show_string(  0, 16 * 4, "   RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "   Points    ");
    ips200_show_string(  0, 16 * 6, "   Camera    ");
    ips200_show_string(  0, 16 * 7, "-->Imu963    ");
    ips200_show_string(110, 16 * 0, "   Flash     ");
    ips200_show_string(110, 16 * 1, "   SevroTest ");
}

void main_menu8(void)
{
    ips200_show_string(  0, 16 * 0, "   CaiDian   ");
    ips200_show_string(  0, 16 * 1, "   PID       ");
    ips200_show_string(  0, 16 * 2, "   GPS Show  ");
    ips200_show_string(  0, 16 * 3, "   Speed     ");
    ips200_show_string(  0, 16 * 4, "   RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "   Points    ");
    ips200_show_string(  0, 16 * 6, "   Camera    ");
    ips200_show_string(  0, 16 * 7, "   Imu963    ");
    ips200_show_string(110, 16 * 0, "-->Flash     ");
    ips200_show_string(110, 16 * 1, "   SevroTest ");
}

void main_menu9(void)
{
    ips200_show_string(  0, 16 * 0, "   CaiDian   ");
    ips200_show_string(  0, 16 * 1, "   PID       ");
    ips200_show_string(  0, 16 * 2, "   GPS Show  ");
    ips200_show_string(  0, 16 * 3, "   Speed     ");
    ips200_show_string(  0, 16 * 4, "   RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "   Points    ");
    ips200_show_string(  0, 16 * 6, "   Camera    ");
    ips200_show_string(  0, 16 * 7, "   Imu963    ");
    ips200_show_string(110, 16 * 0, "   Flash     ");
    ips200_show_string(110, 16 * 1, "-->SevroTest ");
}

/////////////////////////////////����˵�-------------------------------------------------
void CaiDian_menu(void)
{
    float Distance;
    ips200_show_string(0, 16 * 0, "Now_Lat:");
    ips200_show_string(0, 16 * 1, "Now_Lon:");

    ips200_show_float(96, 16 * 0, gnss.latitude, 4, 6);
    ips200_show_float(96, 16 * 1, gnss.longitude, 4, 6);

    ips200_show_string(  0, 16 * 2, "Point_NUM:");
    ips200_show_uint  ( 96, 16 * 2, Point_NUM, 3);
    ips200_show_string(  0, 16 * 3, "Last Saved Point:");

    if(Point_NUM >= 0)
    {
        ips200_show_float( 0, 16 * 4, lat_union[Point_NUM - 1].double_type, 4, 6);
        ips200_show_float( 0, 16 * 5, lon_union[Point_NUM - 1].double_type, 4, 6);

        Distance = get_two_points_distance (gnss.latitude, gnss.longitude, lat_union[Point_NUM - 1].double_type, lon_union[Point_NUM - 1].double_type);
        ips200_show_string(0, 16 * 6, "Distance:");
        ips200_show_float(96, 16 * 6, Distance, 4, 6);
    }


//    ips200_show_uint(  100, 16*3, TeDian[0], 3);          //�����λ�洢��ʾ
//    ips200_show_uint(  100, 16*4, TeDian[1], 3);
//    ips200_show_uint(  100, 16*5, TeDian[2], 3);
//    ips200_show_uint(  100, 16*6, TeDian[3], 3);          //�����λ�洢��ʾ
//    ips200_show_uint(  100, 16*7, TeDian[4], 3);
//    ips200_show_uint(  100, 16*8, TeDian[5], 3);
//    ips200_show_uint(  100, 16*9, TeDian[6], 3);          //�����λ�洢��ʾ
//    ips200_show_uint(  100, 16*10, TeDian[7], 3);
//    ips200_show_uint(  100, 16*5, TeDian[8], 3);

}

void PID_menu(void)
{
    ips200_show_string(0, 16 * 0, "P:");
    ips200_show_string(0, 16 * 1, "I:");
    ips200_show_string(0, 16 * 2, "D:");

    ips200_show_float(16, 16 * 0, Parameter_set0.ServePID[0], 2, 3);
    ips200_show_float(16, 16 * 1, Parameter_set0.ServePID[1], 2, 3);
    ips200_show_float(16, 16 * 2, Parameter_set0.ServePID[2], 2, 3);

    ips200_show_string(0, 16 * 3, "Enter to fix");
}

void GPS_menu(void)
{
    ips200_show_uint(    0, 16 * 0, gnss.time.year  , 4);
    ips200_show_string( 32, 16 * 0, "/");
    ips200_show_uint(   40, 16 * 0, gnss.time.month , 2);
    ips200_show_string( 56, 16 * 0, "/");
    ips200_show_uint(   64, 16 * 0, gnss.time.day   , 2);
    ips200_show_uint(  176, 16 * 0, gnss.time.hour  , 2);
    ips200_show_string(192, 16 * 0, ":");
    ips200_show_uint(  200, 16 * 0, gnss.time.minute, 2);
    ips200_show_string(216, 16 * 0, ":");
    ips200_show_uint(  224, 16 * 0, gnss.time.second, 2);

    ips200_show_string(  0, 16 * 1, "Now_Lat:");
    ips200_show_string(  0, 16 * 2, "Now_Lon:");
    ips200_show_string(168, 16 * 1, "state:");
    ips200_show_string(168, 16 * 2, "K_G:");
    ips200_show_string(  0, 16 * 3, "Filter_Lat:");
    ips200_show_string(  0, 16 * 4, "Filter_Lon:");
    ips200_show_string(  0, 16 * 5, "Gps_Yaw:");
    ips200_show_string(  0, 16 * 6, "Gps_Yaw2:");
    ips200_show_string(  0, 16 * 7, "Yaw:");
    ips200_show_string(  0, 16 * 8, "KEY1:K+0.05");
    ips200_show_string(120, 16 * 8, "KEY2:K-0.05");
    ips200_show_float( 64, 16 * 1, gnss.latitude , 4, 6);
    ips200_show_float( 64, 16 * 2, gnss.longitude, 4, 6);
    ips200_show_uint (216, 16 * 1, gnss.state    , 1);
    ips200_show_float(200, 16 * 2, K_Gps         , 1, 2);
    ips200_show_float( 88, 16 * 3, FilterPoint_Lat, 4, 6);
    ips200_show_float( 88, 16 * 4, FilterPoint_Lon, 4, 6);
    ips200_show_float( 72, 16 * 5, Gps_Yaw        , 4, 6);
    ips200_show_float( 72, 16 * 6, Gps_Yaw2       , 4, 6);
    ips200_show_float( 72, 16 * 7, Yaw            , 4, 6);
}

void spd_menu(void)
{
    ips200_show_string( 0, 16 * 0, "-->Speed   :");
    ips200_show_string( 0, 16 * 1, "   Distance:");
    ips200_show_string( 0, 16 * 2, "   ServeMid:");
    ips200_show_string( 0, 16 * 3, "KEY1:Duty+1000");
    ips200_show_string( 0, 16 * 4, "KEY2:Duty-1000");
    ips200_show_string( 0, 16 * 5, "KEY3:Save");
    ips200_show_int(  136, 16 * 0, Parameter_set0.Speed_Duty, 5);
    ips200_show_float(136, 16 * 1, Parameter_set0.Distance, 2, 2);
    ips200_show_int(  136, 16 * 2, Parameter_set0.Serve_Mid, 4);

}

void Distance_menu(void)
{

    ips200_show_string( 0, 16 * 0, "   Speed   :");
    ips200_show_string( 0, 16 * 1, "-->Distance:");
    ips200_show_string( 0, 16 * 2, "   ServeMid:");
    ips200_show_string( 0, 16 * 3, "KEY1:Distance+0.5");
    ips200_show_string( 0, 16 * 4, "KEY2:Distance-0.5");
    ips200_show_string( 0, 16 * 5, "KEY3:Save");
    ips200_show_int(  136, 16 * 0, Parameter_set0.Speed_Duty, 5);
    ips200_show_float(136, 16 * 1, Parameter_set0.Distance, 2, 2);
    ips200_show_int(  136, 16 * 2, Parameter_set0.Serve_Mid, 4);


}

void serve_mid_menu(void)
{
    ips200_show_string(0, 16 * 0, "   Speed   :");
    ips200_show_string(0, 16 * 1, "   Distance:");
    ips200_show_string(0, 16 * 2, "-->ServeMid:");
    ips200_show_int(  136, 16 * 0, Parameter_set0.Speed_Duty, 5);
    ips200_show_float(136, 16 * 1, Parameter_set0.Distance, 2, 2);
    ips200_show_int(  136, 16 * 2, Parameter_set0.Serve_Mid, 4);

}

void RemoteCtrl_menu(void)
{
    ips200_show_string(80, 16 * 0, "RemoteCtrl:");
    if(1 == uart_receiver.finsh_flag)                                   // ֡��ɱ�־�ж�
    {
        if(1 == uart_receiver.state)                                    // ң����ʧ��״̬�ж�
        {
            ips200_show_string(  0, 16 * 1, "CH1:");
            ips200_show_string(  0, 16 * 2, "CH2:");
            ips200_show_string(  0, 16 * 3, "CH3:");
            ips200_show_string(110, 16 * 1, "CH4:");
            ips200_show_string(110, 16 * 2, "CH5:");
            ips200_show_string(110, 16 * 3, "CH6:");
            ips200_show_uint(   40, 16 * 1, uart_receiver.channel[0], 4);
            ips200_show_uint(   40, 16 * 2, uart_receiver.channel[1], 4);
            ips200_show_uint(   40, 16 * 3, uart_receiver.channel[2], 4);
            ips200_show_uint(  150, 16 * 1, uart_receiver.channel[3], 4);
            ips200_show_uint(  150, 16 * 2, uart_receiver.channel[4], 4);
            ips200_show_uint(  150, 16 * 3, uart_receiver.channel[5], 4);
        }
        else
        {
            ips200_show_string(  0, 16 * 1, "RemoteCtrl disconnected"); // �������ʧ����ʾ
        }
        ips200_show_string(  0, 16 * 4, "finsh_flag:");
        ips200_show_uint(   88, 16 * 4, uart_receiver.finsh_flag, 1);
        uart_receiver.finsh_flag = 0;                                   // ֡��ɱ�־��λ
    }
    ips200_show_string(  0, 16 * 5, "state:");
    ips200_show_uint(   48, 16 * 5, uart_receiver.state, 1);
    ips200_show_string(  0, 16 * 6, "Control_Flag:");
    ips200_show_uint(  104, 16 * 6, Control_Flag, 1);
    ips200_show_string(  0, 16 * 7, "Sta_Stp_Flag:");
    ips200_show_uint(  104, 16 * 7, RemoteCtrl_Start_Stop_Flag, 1);
    ips200_show_string(  0, 16 * 8, "Motor_PWM:");
    ips200_show_int (   80, 16 * 8 , RemoteCtrl_Speed, 4);
}

void Points_menu(void)
{
    ips200_show_string(0, 16 * 0, "Lat:");
    ips200_show_string(0, 16 * 1, "Lot:");
    ips200_show_string(0, 16 * 2, "Point:");
    ips200_show_string(0, 16 * 3, "Point_NUM:");
    ips200_show_float(32, 16 * 0, GPS_GET_LAT[Point], 4, 6);
    ips200_show_float(32, 16 * 1, GPS_GET_LOT[Point], 4, 6);
    ips200_show_uint( 96, 16 * 2, Point + 1, 3);
    ips200_show_uint( 96, 16 * 3, Point_NUM, 3);
    
    double Direction = get_two_points_azimuth(gnss.latitude, gnss.longitude, GPS_GET_LAT[Point], GPS_GET_LOT[Point]);
    double Distance = get_two_points_distance(gnss.latitude, gnss.longitude, GPS_GET_LAT[Point], GPS_GET_LOT[Point]);
    ips200_show_string(0, 16 * 4, "Direction:");
    ips200_show_string(0, 16 * 5, "Distance:");
    ips200_show_float(96, 16 * 4, Direction, 3, 3);
    ips200_show_float(96, 16 * 5, Distance , 3, 6);

    ips200_show_string(0, 16 * 6, "KEY1:Point+1/Lat+");
    ips200_show_string(0, 16 * 7, "KEY2:Point-1/Lat-");
    ips200_show_string(0, 16 * 8, "KEY3:Save   /Lot+");
    ips200_show_string(0, 16 * 9, "KEY4:       /Lot-");
}

void Imu963_menu()
{
    ips200_show_string(0, 16 * 0, "Angle:");
    if(Z_360 >180)
    {
        Z_360 -= 360;
    }
    ips200_show_float( 0, 16 * 1, Z_360, 3, 3);               // ��ʾ������   ������ʾ3λ   С����ʾ3λ
    ips200_show_string(0, 16 * 2, "IMU_Data.gyro_z:");
    ips200_show_float( 0, 16 * 3, IMU_Data.gyro_z, 3, 3);
    system_delay_ms(100);
}

void Flash_menu()
{
    ips200_show_string( 80, 16 * 0, "Flash Menu");
    ips200_show_string(  0, 16 * 1, "KEY1:Obtain");
    ips200_show_string(  0, 16 * 2, "KEY2:Save");
    ips200_show_string(  0, 16 * 3, "KEY3:Delete");
    ips200_show_string(  0, 16 * 4, "KEY4:Check");

}

void Servo_menu()
{
    ips200_show_string( 80, 16 * 0, "ServoTest:");
    ips200_show_string(  0, 16 * 1, "KEY1:Servo Angle + 10");
    ips200_show_string(  0, 16 * 2, "KEY2:Servo Angle - 10");
    ips200_show_string(  0, 16 * 3, "KEY3:Servo Angle + 1");
    ips200_show_string(  0, 16 * 4, "KEY4:Servo Angle - 1");
    ips200_show_string(  0, 16 * 5, "Servo Angle:");
    ips200_show_uint(   96, 16 * 5, Servo_Angle, 3);
}

/////////////////////////////////����˵�-------------------------------------------------

void P_menu(void)
{
    ips200_show_string(  0, 16 * 0, "-->P:");
    ips200_show_string(  0, 16 * 1, "   I:");
    ips200_show_string(  0, 16 * 2, "   D:");
    ips200_show_string(  0, 16 * 3, "KEY1:P+0.1");
    ips200_show_string(120, 16 * 3, "KEY2:P-0.1");
    ips200_show_string(  0, 16 & 4, "KEY3:Save");
    ips200_show_string(120, 16 * 4, "KEY4:Get PID");
    ips200_show_float  (80, 16 * 0,Parameter_set0.ServePID[0], 2, 3);
    ips200_show_float  (80, 16 * 1,Parameter_set0.ServePID[1], 2, 3);
    ips200_show_float  (80, 16 * 2,Parameter_set0.ServePID[2], 2, 3);
}

void I_menu(void)
{
    ips200_show_string(0, 16 * 0, "   P:");
    ips200_show_string(0, 16 * 1, "-->I:");
    ips200_show_string(0, 16 * 2, "   D:");
    ips200_show_string(  0, 16 * 3, "KEY1:I+0.1");
    ips200_show_string(120, 16 * 3, "KEY2:I-0.1");
    ips200_show_string(  0, 16 & 4, "KEY3:Save");
    ips200_show_string(120, 16 * 4, "KEY4:Get PID");
    ips200_show_float  (80, 16 * 0,Parameter_set0.ServePID[0], 2, 3);
    ips200_show_float  (80, 16 * 1,Parameter_set0.ServePID[1], 2, 3);
    ips200_show_float  (80, 16 * 2,Parameter_set0.ServePID[2], 2, 3);
}
void D_menu(void)
{
    ips200_show_string(0, 16 * 0, "   P:");
    ips200_show_string(0, 16 * 1, "   I:");
    ips200_show_string(0, 16 * 2, "-->D:");
    ips200_show_string(  0, 16 * 3, "KEY1:D+0.1");
    ips200_show_string(120, 16 * 3, "KEY2:D-0.1");
    ips200_show_string(  0, 16 & 4, "KEY3:Save");
    ips200_show_string(120, 16 * 4, "KEY4:Get PID");
    ips200_show_float  (80, 16 * 0,Parameter_set0.ServePID[0], 2, 3);
    ips200_show_float  (80, 16 * 1,Parameter_set0.ServePID[1], 2, 3);
    ips200_show_float  (80, 16 * 2,Parameter_set0.ServePID[2], 2, 3);
}

void ZongZuanF(void)
{
//    if(mt9v03x_finish_flag)
//    {
//        ips200_displayimage03x(mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
//        mt9v03x_finish_flag = 0;
//    }
//    ips200_show_rgb565_image(0, 184, (const uint16 *)my_picture, 240, 135, 240, 135, 0);    // ��ʾһ��RGB565ɫ��ͼƬ ԭͼ240*80 ��ʾ240*80 ��λ��ǰ
}

void Key_Ctrl_Menu()
{
    if(!gpio_get_level(SWITCH2))           // �����뿪�����ϱ�ʾ�����л��˵�
    {
        if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
          {
              key_value = 1;
          }
        if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
          {
              key_value = 2;
          }
        if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
          {
              key_value = 3;
          }
        if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
          {
              key_value = 4;
          }
    }
    // flash�洢GPS��λ����
    if(gpio_get_level(SWITCH2))           // ���뿪�����±�ʾִ�д洢����
    {
        // GPS�ɵ�
        if(func_index == 1)
        {
            GL_CRC();

        }
        // P����
        if(func_index == 4)
        {
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                Parameter_set0.ServePID[0] += 0.1;
            }
            if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                Parameter_set0.ServePID[0] -= 0.1;
            }
            if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
            {
                FLASH_SAV_PAR();
            }
        }

        // I����
        if(func_index == 5)
        {
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                Parameter_set0.ServePID[1] += 0.1;
            }
            if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                Parameter_set0.ServePID[1] -= 0.1;
            }
            if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
            {
                FLASH_SAV_PAR();
            }
        }

        // D����
        if(func_index == 6)
        {
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                Parameter_set0.ServePID[2] += 0.1;
            }
            if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                Parameter_set0.ServePID[2] -= 0.1;
            }
            if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
            {
                FLASH_SAV_PAR();
            }
        }

        if(func_index == 8)
        {
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                if(K_Gps < 1)
                {
                    K_Gps += 0.05;
                }
            }
            if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                if(K_Gps > 0)
                {
                    K_Gps -= 0.05;
                }
            }
        }

        // �����ٶ�
        if(func_index == 10)
        {
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                Parameter_set0.Speed_Duty += 1000;
            }
            if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                Parameter_set0.Speed_Duty -= 1000;
            }
            if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
            {
                FLASH_SAV_PAR();
            }
        }

        // �������
        if(func_index == 11)
        {
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                Parameter_set0.Distance += 0.5;
            }
            if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                Parameter_set0.Distance -= 0.5;
            }
            if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
            {
                FLASH_SAV_PAR();
            }
        }

        if(func_index == 16)                                       //��λ�鿴����
        {
            // ���뿪�����ϱ�ʾ��λ�л�
            if(!gpio_get_level(SWITCH1))
            {
                if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
                {
                    if(Point < Point_NUM - 1)
                    {
                        Point = Point + 1;
                    }
                }
                if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
                {
                    if(Point > 0)
                    {
                        Point = Point - 1;
                    }
                }
                if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
                {
                    FLASH_FIX_GPS();
                }
            }
            // ���뿪�����±�ʾ��λ����
            if(gpio_get_level(SWITCH1))
            {
                if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
                {
                    GPS_GET_LAT[Point] += 0.000001;
                }
                if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
                {
                    GPS_GET_LAT[Point] -= 0.000001;
                }
                if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
                {
                    GPS_GET_LOT[Point] += 0.000001;
                }
                if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
                {
                    GPS_GET_LOT[Point] -= 0.000001;
                }
            }

        }

        if(func_index == 22)
        {

            // ����KEY1��Flash�л�ȡ��γ������
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                FLASH_GET_GPS();
            }
            // ����KEY2���澭γ�����ݵ�Flash��
            if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                FLASH_SAV_GPS();
            }
            // ����KEY3ɾ��Flash�еľ�γ������
            if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
            {
                FLASH_DEL_GPS();
            }
            // ����KEY4�鿴��γ����Ϣ
            if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
            {
                ips200_clear();
                func_index = 16;
                table[func_index].current_operation();
            }
        }

        if(func_index == 24)
        {
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                Servo_Angle += 10;
                Servo_Set(Servo_Angle);     // ����Ƕȿ���
            }
            if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                Servo_Angle -= 10;
                Servo_Set(Servo_Angle);     // ����Ƕȿ���
            }
            if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
            {
                Servo_Angle += 1;
                Servo_Set(Servo_Angle);     // ����Ƕȿ���
            }
            if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
            {
                Servo_Angle -= 1;
                Servo_Set(Servo_Angle);     // ����Ƕȿ���
            }
        }
    }
}













// GUI�˵��л�
void Menu_Control()
{
        switch(key_value)
        {
            case 1 : ips200_clear(); func_index = table[func_index].up;    key_value = 0; break;
            case 2 : ips200_clear(); func_index = table[func_index].down;  key_value = 0; break;
            case 3 : ips200_clear(); func_index = table[func_index].back;  key_value = 0; break;
            case 4 : ips200_clear(); func_index = table[func_index].enter; key_value = 0; break;
        }
        table[func_index].current_operation();
}





