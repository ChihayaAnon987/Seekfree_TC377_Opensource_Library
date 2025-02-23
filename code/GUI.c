/*
 * GUI.c
 *
 *  Created on: 2025��1��10��
 *      Author: 20483
 */

#include "zf_common_headfile.h"
Parameter_set Parameter_set0=
{
    {1.34, 0.0, 0.7},            // ���PID
    {1.0, 0.0, 0.0},            // �ٶ�PID
    3000,                       // ���Ե��ٶ�
    1.5,                        // �������
    SERVO_MOTOR_MID             // �����е�ɵ���ֵ
};

int    func_index     = 0;
int    key_value;
int    Point          = 0;    // �˵���λ
int    Point1         = 0;
int    Point2         = 0;
int    Point3         = 0;
int    Task_Point_Set = 1;
int16  Task_Flag      = 1;
int    CameraPoint    = 0;
double Test_Angle     = 0;    // ������
int16  Test_Encoder   = 0;    // ������
uint8  Start_Flag     = 0;    // ������־
seekfree_assistant_oscilloscope_struct oscilloscope_data;


menu_table table[33]=
{
    // current, up, down, back, enter

    // �˵�0
    { 0, 31,  2,  0,  1, main_menu0},       // �ȵ�GPSһ��
    { 1,  1,  1,  0,  1, CaiDian_menu},     // �ȵ�GPS����

    // �˵�1
    { 2,  0, 11,  2,  3, main_menu1},       // PIDһ��
    { 3,  4,  4,  2,  5, ServoPID},         // ServoPID����
    { 4,  3,  3,  2,  8, MotorPID},         // MotorPID����
    { 5,  7,  6,  3,  5, ServoP_menu},      // ServoP
    { 6,  5,  7,  3,  6, ServoI_menu},      // ServoI
    { 7,  6,  5,  3,  7, ServoD_menu},      // ServoD
    { 8, 10,  9,  4,  8, MotorP_menu},      // MotorP
    { 9,  8, 10,  4,  9, MotorI_menu},      // MotorI
    {10,  9,  8,  4, 10, MotorD_menu},      // MotorD

    // �˵�2
    {11,  2, 13, 11, 12, main_menu2},        // ����GPS��ʾһ��
    {12, 12, 12, 11, 12, GPS_menu},          // ����GPS��ʾ����

    // �˵�3
    {13, 11, 17, 13, 14, main_menu3},        // ����,�����е��ֵ���������
    {14, 16, 15, 13, 14, spd_menu},          // ����
    {15, 14, 16, 13, 15, Distance_menu},     // �������
    {16, 15, 14, 13, 16, TaskPoint},         // �����

    // �˵�4
    {17, 13, 19, 17, 18, main_menu4},        // ң��ģʽһ��
    {18, 18, 18, 17, 18, RemoteCtrl_menu},   // ң��ģʽ����

    // �˵�5
    {19, 17, 21, 19, 20, main_menu5},        // GPS��λ�鿴һ��
    {20, 20, 20, 19, 20, Points_menu},       // GPS��λ�鿴����

    // �˵�6
    {21, 19, 23, 21, 22, main_menu6},        // ����ͷͼ��һ��
    {22, 22, 22, 21, 22, ZongZuanF} ,        // ����ͷͼ�����

    // �˵�7
    {23, 21, 25, 23, 24, main_menu7},        // ������һ��
    {24, 24, 24, 23, 24, Imu963_menu},       // �����Ƕ���

    // �˵�8
    {25, 23, 27, 25, 26, main_menu8},        // Flashһ��
    {26, 26, 26, 25, 26, Flash_menu},        // Flash����

    // �˵�9
    {27, 25, 29, 27, 28, main_menu9},        // �������һ��
    {28, 28, 28, 27, 28, Servo_menu},        // ������Զ���

    // �˵�10
    {29, 27, 31, 29, 30, main_menu10},       // ��������һ��
    {30, 30, 30, 29, 30, Param_Set},         // �������ö���
    
    // �˵�11
    {31, 29,  0, 31, 32, main_menu11},       // ����ѡ��һ��
    {32, 32, 32, 31, 32, Task_Select}        // ����ѡ�����  

};

/////////////////////////////////һ��˵�-------------------------------------------------
void main_menu0(void)
{
    ips200_show_string(  0, 16 * 0, "-->CaiDian   ");
    ips200_show_string(  0, 16 * 1, "   PID       ");
    ips200_show_string(  0, 16 * 2, "   GPS Show  ");
    ips200_show_string(  0, 16 * 3, "   Duty      ");
    ips200_show_string(  0, 16 * 4, "   RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "   Points    ");
    ips200_show_string(  0, 16 * 6, "   Camera    ");
    ips200_show_string(  0, 16 * 7, "   Imu963    ");
    ips200_show_string(136, 16 * 0, "   Flash     ");
    ips200_show_string(136, 16 * 1, "   SevroTest ");
    ips200_show_string(136, 16 * 2, "   ParamSet  ");
    ips200_show_string(136, 16 * 3, "   TaskSelect");
    ips200_draw_line  (108, 0, 108, 184, RGB565_PURPLE);
    ips200_draw_line  (132, 0, 132, 184, RGB565_PURPLE);
    ips200_show_chinese(112,  0, 16, TeamName1[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 16, 16, TeamName2[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 32, 16, TeamName3[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 48, 16, TeamName4[0], 1, RGB565_PURPLE);
}

void main_menu1(void)
{
    ips200_show_string(  0, 16 * 0, "   CaiDian   ");
    ips200_show_string(  0, 16 * 1, "-->PID       ");
    ips200_show_string(  0, 16 * 2, "   GPS Show  ");
    ips200_show_string(  0, 16 * 3, "   Duty      ");
    ips200_show_string(  0, 16 * 4, "   RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "   Points    ");
    ips200_show_string(  0, 16 * 6, "   Camera    ");
    ips200_show_string(  0, 16 * 7, "   Imu963    ");
    ips200_show_string(136, 16 * 0, "   Flash     ");
    ips200_show_string(136, 16 * 1, "   SevroTest ");
    ips200_show_string(136, 16 * 2, "   ParamSet  ");
    ips200_show_string(136, 16 * 3, "   TaskSelect");
    ips200_draw_line  (108, 0, 108, 184, RGB565_PURPLE);
    ips200_draw_line  (132, 0, 132, 184, RGB565_PURPLE);
    ips200_show_chinese(112,  0, 16, TeamName1[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 16, 16, TeamName2[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 32, 16, TeamName3[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 48, 16, TeamName4[0], 1, RGB565_PURPLE);
}

void main_menu2(void)
{
    ips200_show_string(  0, 16 * 0, "   CaiDian   ");
    ips200_show_string(  0, 16 * 1, "   PID       ");
    ips200_show_string(  0, 16 * 2, "-->GPS Show  ");
    ips200_show_string(  0, 16 * 3, "   Duty      ");
    ips200_show_string(  0, 16 * 4, "   RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "   Points    ");
    ips200_show_string(  0, 16 * 6, "   Camera    ");
    ips200_show_string(  0, 16 * 7, "   Imu963    ");
    ips200_show_string(136, 16 * 0, "   Flash     ");
    ips200_show_string(136, 16 * 1, "   SevroTest ");
    ips200_show_string(136, 16 * 2, "   ParamSet  ");
    ips200_show_string(136, 16 * 3, "   TaskSelect");
    ips200_draw_line  (108, 0, 108, 184, RGB565_PURPLE);
    ips200_draw_line  (132, 0, 132, 184, RGB565_PURPLE);
    ips200_show_chinese(112,  0, 16, TeamName1[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 16, 16, TeamName2[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 32, 16, TeamName3[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 48, 16, TeamName4[0], 1, RGB565_PURPLE);
}

void main_menu3(void)
{
    ips200_show_string(  0, 16 * 0, "   CaiDian   ");
    ips200_show_string(  0, 16 * 1, "   PID       ");
    ips200_show_string(  0, 16 * 2, "   GPS Show  ");
    ips200_show_string(  0, 16 * 3, "-->Duty      ");
    ips200_show_string(  0, 16 * 4, "   RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "   Points    ");
    ips200_show_string(  0, 16 * 6, "   Camera    ");
    ips200_show_string(  0, 16 * 7, "   Imu963    ");
    ips200_show_string(136, 16 * 0, "   Flash     ");
    ips200_show_string(136, 16 * 1, "   SevroTest ");
    ips200_show_string(136, 16 * 2, "   ParamSet  ");
    ips200_show_string(136, 16 * 3, "   TaskSelect");
    ips200_draw_line  (108, 0, 108, 184, RGB565_PURPLE);
    ips200_draw_line  (132, 0, 132, 184, RGB565_PURPLE);
    ips200_show_chinese(112,  0, 16, TeamName1[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 16, 16, TeamName2[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 32, 16, TeamName3[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 48, 16, TeamName4[0], 1, RGB565_PURPLE);
}

void main_menu4(void)
{
    ips200_show_string(  0, 16 * 0, "   CaiDian   ");
    ips200_show_string(  0, 16 * 1, "   PID       ");
    ips200_show_string(  0, 16 * 2, "   GPS Show  ");
    ips200_show_string(  0, 16 * 3, "   Duty      ");
    ips200_show_string(  0, 16 * 4, "-->RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "   Points    ");
    ips200_show_string(  0, 16 * 6, "   Camera    ");
    ips200_show_string(  0, 16 * 7, "   Imu963    ");
    ips200_show_string(136, 16 * 0, "   Flash     ");
    ips200_show_string(136, 16 * 1, "   SevroTest ");
    ips200_show_string(136, 16 * 2, "   ParamSet  ");
    ips200_show_string(136, 16 * 3, "   TaskSelect");
    ips200_draw_line  (108, 0, 108, 184, RGB565_PURPLE);
    ips200_draw_line  (132, 0, 132, 184, RGB565_PURPLE);
    ips200_show_chinese(112,  0, 16, TeamName1[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 16, 16, TeamName2[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 32, 16, TeamName3[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 48, 16, TeamName4[0], 1, RGB565_PURPLE);
}

void main_menu5(void)
{
    ips200_show_string(  0, 16 * 0, "   CaiDian   ");
    ips200_show_string(  0, 16 * 1, "   PID       ");
    ips200_show_string(  0, 16 * 2, "   GPS Show  ");
    ips200_show_string(  0, 16 * 3, "   Duty      ");
    ips200_show_string(  0, 16 * 4, "   RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "-->Points    ");
    ips200_show_string(  0, 16 * 6, "   Camera    ");
    ips200_show_string(  0, 16 * 7, "   Imu963    ");
    ips200_show_string(136, 16 * 0, "   Flash     ");
    ips200_show_string(136, 16 * 1, "   SevroTest ");
    ips200_show_string(136, 16 * 2, "   ParamSet  ");
    ips200_show_string(136, 16 * 3, "   TaskSelect");
    ips200_draw_line  (108, 0, 108, 184, RGB565_PURPLE);
    ips200_draw_line  (132, 0, 132, 184, RGB565_PURPLE);
    ips200_show_chinese(112,  0, 16, TeamName1[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 16, 16, TeamName2[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 32, 16, TeamName3[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 48, 16, TeamName4[0], 1, RGB565_PURPLE);
}

void main_menu6(void)
{
    ips200_show_string(  0, 16 * 0, "   CaiDian   ");
    ips200_show_string(  0, 16 * 1, "   PID       ");
    ips200_show_string(  0, 16 * 2, "   GPS Show  ");
    ips200_show_string(  0, 16 * 3, "   Duty      ");
    ips200_show_string(  0, 16 * 4, "   RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "   Points    ");
    ips200_show_string(  0, 16 * 6, "-->Camera    ");
    ips200_show_string(  0, 16 * 7, "   Imu963    ");
    ips200_show_string(136, 16 * 0, "   Flash     ");
    ips200_show_string(136, 16 * 1, "   SevroTest ");
    ips200_show_string(136, 16 * 2, "   ParamSet  ");
    ips200_show_string(136, 16 * 3, "   TaskSelect");
    ips200_draw_line  (108, 0, 108, 184, RGB565_PURPLE);
    ips200_draw_line  (132, 0, 132, 184, RGB565_PURPLE);
    ips200_show_chinese(112,  0, 16, TeamName1[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 16, 16, TeamName2[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 32, 16, TeamName3[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 48, 16, TeamName4[0], 1, RGB565_PURPLE);
}

void main_menu7(void)
{
    ips200_show_string(  0, 16 * 0, "   CaiDian   ");
    ips200_show_string(  0, 16 * 1, "   PID       ");
    ips200_show_string(  0, 16 * 2, "   GPS Show  ");
    ips200_show_string(  0, 16 * 3, "   Duty      ");
    ips200_show_string(  0, 16 * 4, "   RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "   Points    ");
    ips200_show_string(  0, 16 * 6, "   Camera    ");
    ips200_show_string(  0, 16 * 7, "-->Imu963    ");
    ips200_show_string(136, 16 * 0, "   Flash     ");
    ips200_show_string(136, 16 * 1, "   SevroTest ");
    ips200_show_string(136, 16 * 2, "   ParamSet  ");
    ips200_show_string(136, 16 * 3, "   TaskSelect");
    ips200_draw_line  (108, 0, 108, 184, RGB565_PURPLE);
    ips200_draw_line  (132, 0, 132, 184, RGB565_PURPLE);
    ips200_show_chinese(112,  0, 16, TeamName1[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 16, 16, TeamName2[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 32, 16, TeamName3[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 48, 16, TeamName4[0], 1, RGB565_PURPLE);
}

void main_menu8(void)
{
    ips200_show_string(  0, 16 * 0, "   CaiDian   ");
    ips200_show_string(  0, 16 * 1, "   PID       ");
    ips200_show_string(  0, 16 * 2, "   GPS Show  ");
    ips200_show_string(  0, 16 * 3, "   Duty      ");
    ips200_show_string(  0, 16 * 4, "   RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "   Points    ");
    ips200_show_string(  0, 16 * 6, "   Camera    ");
    ips200_show_string(  0, 16 * 7, "   Imu963    ");
    ips200_show_string(136, 16 * 0, "-->Flash     ");
    ips200_show_string(136, 16 * 1, "   SevroTest ");
    ips200_show_string(136, 16 * 2, "   ParamSet  ");
    ips200_show_string(136, 16 * 3, "   TaskSelect");
    ips200_draw_line  (108, 0, 108, 184, RGB565_PURPLE);
    ips200_draw_line  (132, 0, 132, 184, RGB565_PURPLE);
    ips200_show_chinese(112,  0, 16, TeamName1[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 16, 16, TeamName2[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 32, 16, TeamName3[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 48, 16, TeamName4[0], 1, RGB565_PURPLE);
}

void main_menu9(void)
{
    ips200_show_string(  0, 16 * 0, "   CaiDian   ");
    ips200_show_string(  0, 16 * 1, "   PID       ");
    ips200_show_string(  0, 16 * 2, "   GPS Show  ");
    ips200_show_string(  0, 16 * 3, "   Duty      ");
    ips200_show_string(  0, 16 * 4, "   RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "   Points    ");
    ips200_show_string(  0, 16 * 6, "   Camera    ");
    ips200_show_string(  0, 16 * 7, "   Imu963    ");
    ips200_show_string(136, 16 * 0, "   Flash     ");
    ips200_show_string(136, 16 * 1, "-->SevroTest ");
    ips200_show_string(136, 16 * 2, "   ParamSet  ");
    ips200_show_string(136, 16 * 3, "   TaskSelect");
    ips200_draw_line  (108, 0, 108, 184, RGB565_PURPLE);
    ips200_draw_line  (132, 0, 132, 184, RGB565_PURPLE);
    ips200_show_chinese(112,  0, 16, TeamName1[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 16, 16, TeamName2[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 32, 16, TeamName3[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 48, 16, TeamName4[0], 1, RGB565_PURPLE);
}

void main_menu10(void)
{
    ips200_show_string(  0, 16 * 0, "   CaiDian   ");
    ips200_show_string(  0, 16 * 1, "   PID       ");
    ips200_show_string(  0, 16 * 2, "   GPS Show  ");
    ips200_show_string(  0, 16 * 3, "   Duty      ");
    ips200_show_string(  0, 16 * 4, "   RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "   Points    ");
    ips200_show_string(  0, 16 * 6, "   Camera    ");
    ips200_show_string(  0, 16 * 7, "   Imu963    ");
    ips200_show_string(136, 16 * 0, "   Flash     ");
    ips200_show_string(136, 16 * 1, "   SevroTest ");
    ips200_show_string(136, 16 * 2, "-->ParamSet  ");
    ips200_show_string(136, 16 * 3, "   TaskSelect");
    ips200_draw_line  (108, 0, 108, 184, RGB565_PURPLE);
    ips200_draw_line  (132, 0, 132, 184, RGB565_PURPLE);
    ips200_show_chinese(112,  0, 16, TeamName1[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 16, 16, TeamName2[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 32, 16, TeamName3[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 48, 16, TeamName4[0], 1, RGB565_PURPLE);
}

void main_menu11(void)
{
    ips200_show_string(  0, 16 * 0, "   CaiDian   ");
    ips200_show_string(  0, 16 * 1, "   PID       ");
    ips200_show_string(  0, 16 * 2, "   GPS Show  ");
    ips200_show_string(  0, 16 * 3, "   Duty      ");
    ips200_show_string(  0, 16 * 4, "   RemoteCtrl");
    ips200_show_string(  0, 16 * 5, "   Points    ");
    ips200_show_string(  0, 16 * 6, "   Camera    ");
    ips200_show_string(  0, 16 * 7, "   Imu963    ");
    ips200_show_string(136, 16 * 0, "   Flash     ");
    ips200_show_string(136, 16 * 1, "   SevroTest ");
    ips200_show_string(136, 16 * 2, "   ParamSet  ");
    ips200_show_string(136, 16 * 3, "-->TaskSelect");
    ips200_draw_line  (108, 0, 108, 184, RGB565_PURPLE);
    ips200_draw_line  (132, 0, 132, 184, RGB565_PURPLE);
    ips200_show_chinese(112,  0, 16, TeamName1[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 16, 16, TeamName2[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 32, 16, TeamName3[0], 1, RGB565_PURPLE);
    ips200_show_chinese(112, 48, 16, TeamName4[0], 1, RGB565_PURPLE);
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

        seekfree_assistant_oscilloscope_send(&oscilloscope_data);
        oscilloscope_data.data[0] = Distance;
    }
    ips200_show_string(  0, 16 *  7, "KEY1:Get  Point");
    ips200_show_string(  0, 16 *  8, "KEY2:Save Point");
    ips200_show_string(  0, 16 *  9, "KEY3:Point+1");
    ips200_show_string(  0, 16 * 10, "KEY4:Point-1");

}

void ServoPID(void)
{
    ips200_show_string(0, 16 * 0, "-->ServePID");
    ips200_show_string(0, 16 * 1, "   MotorPID");
}

void MotorPID(void)
{
    ips200_show_string(0, 16 * 0, "   ServePID");
    ips200_show_string(0, 16 * 1, "-->MotorPID");
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
    ips200_show_string(  0, 16 * 8, "Angle:");
    ips200_show_string(  0, 16 * 9, "KEY1:K+0.05");
    ips200_show_string(120, 16 * 9, "KEY2:K-0.05");
    ips200_show_float ( 64, 16 * 1, gnss.latitude  , 4, 6);
    ips200_show_float ( 64, 16 * 2, gnss.longitude , 4, 6);
    ips200_show_uint  (216, 16 * 1, gnss.state     , 1);
    ips200_show_float (200, 16 * 2, K_Gps          , 1, 2);
    ips200_show_float ( 88, 16 * 3, FilterPoint_Lat, 4, 6);
    ips200_show_float ( 88, 16 * 4, FilterPoint_Lon, 4, 6);
    ips200_show_float ( 72, 16 * 5, Gps_Yaw        , 4, 6);
    ips200_show_float ( 72, 16 * 6, Gps_Yaw2       , 4, 6);
    ips200_show_float ( 72, 16 * 7, Yaw            , 4, 6);
    ips200_show_float ( 48, 16 * 8, Angle          , 4, 6);

   seekfree_assistant_oscilloscope_send(&oscilloscope_data);
   oscilloscope_data.data[0] = gnss.latitude;
   oscilloscope_data.data[1] = gnss.longitude;
   oscilloscope_data.data[2] = Angle;
   oscilloscope_data.data[3] = gnss.direction;
}

void spd_menu(void)
{
    ips200_show_string( 0, 16 * 0, "-->Duty    :");
    ips200_show_string( 0, 16 * 1, "   Distance:");
    ips200_show_string( 0, 16 * 2, "   TasPoint:");    

    ips200_show_uint  (184, 16 * 0, Point1, 3);
    ips200_show_string(208, 16 * 0, "/");
    ips200_show_uint  (216, 16 * 0, NUM_GPS_DATA - 1, 3);

    int Page = Point1 / Page_Point_Num;
    int RightArrow = Point1 % Page_Point_Num + 1;
    ips200_show_string(176, 16 * RightArrow, "->");
    for(int i = 1; i <= Page_Point_Num; i++)
    {
        ips200_show_uint (192, 16 * i, GpsTgtEncod[i - 1 + Page * Page_Point_Num], 5);
    }

    ips200_show_string(  0, 16 *  9, "KEY1:Point-1");
    ips200_show_string(  0, 16 * 10, "KEY2:Point+1");
    ips200_show_string(120, 16 *  9, "KEY3:Duty+100");
    ips200_show_string(120, 16 * 10, "KEY4:Duty-100");


}

void Distance_menu(void)
{

    ips200_show_string( 0, 16 * 0, "   Duty    :");
    ips200_show_string( 0, 16 * 1, "-->Distance:");
    ips200_show_string( 0, 16 * 2, "   TasPoint:");

    ips200_show_uint  (184, 16 * 0, Point1, 3);
    ips200_show_string(208, 16 * 0, "/");
    ips200_show_uint  (216, 16 * 0, NUM_GPS_DATA - 1, 3);

    int Page = Point1 / Page_Point_Num;
    int RightArrow = Point1 % Page_Point_Num + 1;
    ips200_show_string(176, 16 * RightArrow, "->");
    for(int i = 1; i <= Page_Point_Num; i++)
    {
        ips200_show_float (192, 16 * i, GpsDistance[i - 1 + Page * Page_Point_Num], 1, 1);
    }

    ips200_show_string(  0, 16 *  9, "KEY1:Point-1");
    ips200_show_string(  0, 16 * 10, "KEY2:Point+1");
    ips200_show_string(120, 16 *  9, "KEY3:Dist+0.5");
    ips200_show_string(120, 16 * 10, "KEY4:Dist-0.5");

}

void TaskPoint(void)
{
    ips200_show_string(0, 16 * 0, "   Duty    :");
    ips200_show_string(0, 16 * 1, "   Distance:");
    ips200_show_string(0, 16 * 2, "-->TasPoint:");


    if(Task_Point_Set == 1)
    {
        ips200_show_string(0, 16 * 3, "->Task1Point:");
        ips200_show_string(0, 16 * 4, "  Task2Point:");
        ips200_show_string(0, 16 * 5, "  Task3Point:");
        ips200_show_int (104, 16 * 3, Task1_Points, 3);
        ips200_show_int (104, 16 * 4, Task2_Points, 3);
        ips200_show_int (104, 16 * 5, Task3_Points, 3);
    }
    if(Task_Point_Set == 2)
    {
        ips200_show_string(0, 16 * 3, "  Task1Point:");
        ips200_show_string(0, 16 * 4, "->Task2Point:");
        ips200_show_string(0, 16 * 5, "  Task3Point:");
        ips200_show_int (104, 16 * 3, Task1_Points, 3);
        ips200_show_int (104, 16 * 4, Task2_Points, 3);
        ips200_show_int (104, 16 * 5, Task3_Points, 3);
    }
    if(Task_Point_Set == 3)
    {
        ips200_show_string(0, 16 * 3, "  Task1Point:");
        ips200_show_string(0, 16 * 4, "  Task2Point:");
        ips200_show_string(0, 16 * 5, "->Task3Point:");
        ips200_show_int (104, 16 * 3, Task1_Points, 3);
        ips200_show_int (104, 16 * 4, Task2_Points, 3);
        ips200_show_int (104, 16 * 5, Task3_Points, 3);
    }

    ips200_show_string(  0, 16 *  9, "KEY1:Task-1");
    ips200_show_string(  0, 16 * 10, "KEY2:Task+1");
    ips200_show_string(120, 16 *  9, "KEY3:Point+1");
    ips200_show_string(120, 16 * 10, "KEY4:Point-1");

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
    ips200_show_string( 64, 16 * 5, "finsh_flag:");
    ips200_show_uint(  152, 16 * 5, uart_receiver.finsh_flag, 1);
    ips200_show_string(  0, 16 * 6, "Control_Flag:");
    ips200_show_uint(  104, 16 * 6, Control_Flag, 1);
    ips200_show_string(  0, 16 * 7, "Motor_PWM:");
    ips200_show_int (   80, 16 * 7 , RemoteCtrl_Speed, 4);
    ips200_show_string(  0, 16 * 8, "Encoder:");
    ips200_show_int   ( 64, 16 * 8, Encoder, 5);
    
}

void Points_menu(void)
{
    ips200_show_string( 16, 16 * 0, "Lat:");
    ips200_show_string(128, 16 * 0, "Lon:");
    ips200_show_uint  (184, 16 * 0, Point, 3);
    ips200_show_string(208, 16 * 0, "/");
    ips200_show_uint  (216, 16 * 0, Point_NUM - 1, 3);

    int Page = Point / Page_Point_Num;
    int RightArrow = Point % Page_Point_Num + 1;
    if(Point_NUM > 0)
    {
        ips200_show_string(0, 16 * RightArrow, "->");
        for(int i = 1; i <= Page_Point_Num; i++)
        {
            ips200_show_float ( 16, 16 * i, GPS_GET_LAT[i - 1 + Page * Page_Point_Num], 3, 6);
            ips200_show_float (128, 16 * i, GPS_GET_LOT[i - 1 + Page * Page_Point_Num], 3, 6);
            if(i  + Page * 10 == Point_NUM)
            {
                break;
            }
        }
    }
    ips200_show_string(  0, 16 *  9, "1:Point-1/Lat+");
    ips200_show_string(  0, 16 * 10, "2:Point+1/Lat-");
    ips200_show_string(120, 16 *  9, "3:Save   /Lot+");
    ips200_show_string(120, 16 * 10, "4:Print  /Lot-");

    // double Direction = get_two_points_azimuth(GPS_GET_LAT[0], GPS_GET_LOT[0], GPS_GET_LAT[1], GPS_GET_LOT[1]);
    // double Distance = get_two_points_distance(GPS_GET_LAT[0], GPS_GET_LOT[0], GPS_GET_LAT[1], GPS_GET_LOT[1]);
    // ips200_show_string(0, 16 * 4, "Direction:");
    // ips200_show_string(0, 16 * 5, "Distance:");
    // ips200_show_float(96, 16 * 4, Direction, 3, 3);
    // ips200_show_float(96, 16 * 5, Distance , 5, 6);


}

void ZongZuanF(void)
{
    Process_Image();
    if(mt9v03x_finish_flag)
    {
        mt9v03x_finish_flag = 0;
        // memcpy(image_copy[0], mt9v03x_image[0], MT9V03X_IMAGE_SIZE);
        // seekfree_assistant_camera_send();
        ips200_show_gray_image(0, 0, mt9v03x_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
    }

    ips200_show_string(0, 16 * 8, "Left Line:");
    ips200_show_string(0, 16 * 9, "RightLine:");
    ips200_show_int( 216, 16 * 8, clip_value, 2);
    if(LeftLineNum > 0)
    {
        for(int i = 0; i < MT9V03X_H; i++)
        {
            ips200_draw_point(IntClip(LeftLine_x[i], 0, ips200_width_max - 1), IntClip(LeftLine_y[i], 0, ips200_height_max - 1), RGB565_RED);
        }
        ips200_show_string(80, 16 * 8, "FoundLine");
    }
    else
    {
        ips200_show_string(80, 16  * 8, "Not Found");
    }
    if(RightLineNum > 0)
    {
        for(int i = 0; i < MT9V03X_H; i++)
        {
            ips200_draw_point(IntClip(RightLine_x[i], 0, ips200_width_max - 1), IntClip(RightLine_y[i], 0, ips200_height_max - 1), RGB565_RED);
        }
        ips200_show_string(80, 16 * 9, "FoundLine");
    }
    else
    {
        ips200_show_string(80, 16  * 9, "Not Found");
    }
    ips200_show_string(  0, 16 * 10, "Angle_Error");
    ips200_show_float(  88, 16 * 10, CalculateAngleError(LeftLine), 3, 3);
//    ips200_show_string(  0, 16 *  9, "1:Point-1");
//    ips200_show_string(  0, 16 * 10, "2:Point+1");
//    ips200_show_string(120, 16 *  9, "3:");
//    ips200_show_string(120, 16 * 10, "4:");

}
void Imu963_menu()
{
    ips200_show_string( 0, 16 * 0, "IMU_Angle:");
    ips200_show_string( 0, 16 * 1, "IMU_Data.gyro_z:");
    ips200_show_string( 0, 16 * 2, "MAX_IMU_Data_gyro_z:");
    ips200_show_string( 0, 16 * 3, "angle[0]:");
    ips200_show_string( 0, 16 * 4, "angle[1]:");
    ips200_show_string( 0, 16 * 5, "angle[2]:");
    ips200_show_string( 0, 16 * 6, "Kp_Ah:");
    ips200_show_string( 0, 16 * 7, "Ki_Ah:");

    ips200_show_float( 80, 16 * 0, Z_360 > 180 ? Z_360 - 360 : Z_360, 3, 3);
    ips200_show_float(128, 16 * 1, IMU_Data.gyro_z, 3, 3);
    ips200_show_float(160, 16 * 2, MAX_IMU_Data_gyro_z, 3, 3);
    ips200_show_float( 72, 16 * 3, angle[0], 3, 3);
    ips200_show_float( 72, 16 * 4, angle[1], 3, 3);
    ips200_show_float( 72, 16 * 5, angle[2], 3, 3);
    ips200_show_float( 48, 16 * 6, Kp_Ah, 3, 3);
    ips200_show_float( 48, 16 * 7, Ki_Ah, 3, 3);

    seekfree_assistant_oscilloscope_send(&oscilloscope_data);
    oscilloscope_data.data[0] = IMU_Data.gyro_z;
    oscilloscope_data.data[1] = Z_360 > 180 ? Z_360 - 360 : Z_360;
    oscilloscope_data.data[2] = imu963ra_mag_x;
    oscilloscope_data.data[3] = imu963ra_mag_y;
    oscilloscope_data.data[4] = imu963ra_mag_z;
    oscilloscope_data.data[5] = angle[0];
    oscilloscope_data.data[6] = angle[1];
    oscilloscope_data.data[7] = angle[2];

    system_delay_ms(10);
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

void Param_Set()
{
    int Page = Point2 / Page_Point_Num;
    int RightArrow = Point2 % Page_Point_Num;
    ips200_show_string(0, 16 * RightArrow, "-->");
    if(Page == 0)
    {
        ips200_show_string( 24, 16 * 0, "Fly_Slope_Alpha:");
        ips200_show_string( 24, 16 * 1, "K_Straight     :");
        ips200_show_string( 24, 16 * 2, "2000DutyServoKp:");
        ips200_show_string( 24, 16 * 3, "2000DutyServoKd:");
        ips200_show_string( 24, 16 * 4, "3000DutyServoKp:");
        ips200_show_string( 24, 16 * 5, "3000DutyServoKd:");
        ips200_show_string( 24, 16 * 6, "4500DutyServoKp:");
        ips200_show_string( 24, 16 * 7, "4500DutyServoKd:");
        ips200_show_int(   152, 16 * 0, Fly_Slope_Alpha, 4);
        ips200_show_float( 152, 16 * 1, K_Straight, 1, 3);
        ips200_show_float( 152, 16 * 2, From_0000_To_2000_ServoPD.Kp, 1, 2);
        ips200_show_float( 152, 16 * 3, From_0000_To_2000_ServoPD.Kd, 1, 2);
        ips200_show_float( 152, 16 * 4, From_2000_To_4000_ServoPD.Kp, 1, 2);
        ips200_show_float( 152, 16 * 5, From_2000_To_4000_ServoPD.Kd, 1, 2);
        ips200_show_float( 152, 16 * 6, From_4000_To_5000_ServoPD.Kp, 1, 2);
        ips200_show_float( 152, 16 * 7, From_4000_To_5000_ServoPD.Kd, 1, 2);
    }
    if(Page == 1)
    {
        ips200_show_string( 24, 16 * 0, "4500DutyServoKp:");
        ips200_show_string( 24, 16 * 1, "4500DutyServoKd:");
        ips200_show_string( 24, 16 * 2, "6500DutyServoKp:");
        ips200_show_string( 24, 16 * 3, "6500DutyServoKd:");
        ips200_show_string( 24, 16 * 4, "7500DutyServoKp:");
        ips200_show_string( 24, 16 * 5, "7500DutyServoKd:");
        ips200_show_string( 24, 16 * 6, "8500DutyServoKp:");
        ips200_show_string( 24, 16 * 7, "8500DutyServoKd:");
        ips200_show_float( 152, 16 * 0, From_5000_To_6000_ServoPD.Kp, 1, 2);
        ips200_show_float( 152, 16 * 1, From_5000_To_6000_ServoPD.Kd, 1, 2);
        ips200_show_float( 152, 16 * 2, From_6000_To_7000_ServoPD.Kp, 1, 2);
        ips200_show_float( 152, 16 * 3, From_6000_To_7000_ServoPD.Kd, 1, 2);
        ips200_show_float( 152, 16 * 4, From_7000_To_8000_ServoPD.Kp, 1, 2);
        ips200_show_float( 152, 16 * 5, From_7000_To_8000_ServoPD.Kd, 1, 2);
        ips200_show_float( 152, 16 * 6, From_8000_To_9000_ServoPD.Kp, 1, 2);
        ips200_show_float( 152, 16 * 7, From_8000_To_9000_ServoPD.Kd, 1, 2);
    }
    if(Page == 2)
    {
        ips200_show_string( 24, 16 * 0, "9500DutyServoKp:");
        ips200_show_string( 24, 16 * 1, "9500DutyServoKd:");
        ips200_show_float( 152, 16 * 0, From_9000_To_9900_ServoPD.Kp, 1, 2);
        ips200_show_float( 152, 16 * 1, From_9000_To_9900_ServoPD.Kd, 1, 2);
    }

    if(Point2 == 0)
    {
        ips200_show_string(  0, 16 *  9, "KEY1:Up");
        ips200_show_string(  0, 16 * 10, "KEY2:Down");
        ips200_show_string(120, 16 *  9, "KEY3:K+100");
        ips200_show_string(120, 16 * 10, "KEY4:K-100");
    }
    else
    {
        ips200_show_string(  0, 16 *  9, "KEY1:Up");
        ips200_show_string(  0, 16 * 10, "KEY2:Down");
        ips200_show_string(120, 16 *  9, "KEY3:K+0.01");
        ips200_show_string(120, 16 * 10, "KEY4:K-0.01");
    }


}


void Task_Select()
{
    ips200_show_string(  0, 16 *  0, "Track_Point:");
    ips200_show_uint(   96, 16 *  0, Track_Points_NUM, 3);
    ips200_show_string(  0, 16 *  1, "Distance:");
    ips200_show_float(  72, 16 *  1, Distance, 3, 3);
    ips200_show_string(  0, 16 *  2, "GPS_Angle:");
    ips200_show_float(  80, 16 *  2, Angle, 3, 3);
    ips200_show_string(  0, 16 *  3, "Yaw:");
    ips200_show_float(  32, 16 *  3, angle[2], 3, 3);
    ips200_show_string(  0, 16 *  4, "Angle_Error:");
    ips200_show_float(  96, 16 *  4, Angle_Error, 3, 6);
    ips200_show_string(  0, 16 *  5, "Delta_Angle:");
    ips200_show_float(  96, 16 *  5, Delta_Angle, 3, 6);
    ips200_show_string(  0, 16 *  6, "Delta_Lat:");
    ips200_show_float(  80, 16 *  6, Delta_Lat, 3, 6);
    ips200_show_string(  0, 16 *  7, "Delta_Lon:");
    ips200_show_float(  80, 16 *  7, Delta_Lon, 3, 6);
    ips200_show_float(  0,  16 *  8, gnss.latitude, 3, 6);
    ips200_show_float( 96,  16 *  8, gnss.longitude, 3, 6);
    ips200_show_string(  0, 16 *  9, "KEY1:Task1");
    ips200_show_string(120, 16 *  9, "KEY2:Task2");
    ips200_show_string(  0, 16 * 10, "KEY3:Task3");
    ips200_show_string(120, 16 * 10, "KEY4:Start");
}


/////////////////////////////////����˵�-------------------------------------------------

void ServoP_menu(void)
{
    ips200_show_string(  0, 16 * 0, "-->ServoP:");
    ips200_show_string(  0, 16 * 1, "   ServoI:");
    ips200_show_string(  0, 16 * 2, "   ServoD:");
    ips200_show_string(  0, 16 * 3, "Angle:");
    ips200_show_string(  0, 16 * 4, "angle[2]:");
    ips200_show_string(  0, 16 * 5, "Angle_Error:");
    ips200_show_string(  0, 16 * 6, "Servo_Angle:");
    ips200_show_string(  0, 16 * 7, "PID.output:");
    ips200_show_string(  0, 16 * 8, "KEY1:P+0.01");
    ips200_show_string(120, 16 * 8, "KEY2:P-0.01");
    ips200_show_string(  0, 16 * 9, "KEY3:SavePar");
    ips200_show_string(120, 16 * 9, "KEY4:Get Par");
    ips200_show_float ( 80, 16 * 0, Parameter_set0.ServePID[0], 2, 3);
    ips200_show_float ( 80, 16 * 1, Parameter_set0.ServePID[1], 2, 3);
    ips200_show_float ( 80, 16 * 2, Parameter_set0.ServePID[2], 2, 3);
    ips200_show_float ( 48, 16 * 3, Angle, 4, 6);
    ips200_show_float ( 72, 16 * 4, angle[2], 3, 3);
    ips200_show_float ( 96, 16 * 5, Angle_Error, 3, 6);
    ips200_show_float ( 96, 16 * 6, Servo_Angle, 3, 3);
    ips200_show_float ( 88, 16 * 7, PID_SERVO.output, 3, 6);

    seekfree_assistant_oscilloscope_send(&oscilloscope_data);
    oscilloscope_data.data[0] = PID_SERVO.output;
    oscilloscope_data.data[1] = Servo_Angle;
    oscilloscope_data.data[2] = Angle_Error;
    oscilloscope_data.data[3] = Angle;

}

void ServoI_menu(void)
{
    ips200_show_string(  0, 16 * 0, "   ServoP:");
    ips200_show_string(  0, 16 * 1, "-->ServoI:");
    ips200_show_string(  0, 16 * 2, "   ServoD:");
    ips200_show_string(  0, 16 * 3, "Angle:");
    ips200_show_string(  0, 16 * 4, "angle[2]:");
    ips200_show_string(  0, 16 * 5, "Angle_Error:");
    ips200_show_string(  0, 16 * 6, "Servo_Angle:");
    ips200_show_string(  0, 16 * 7, "PID.output:");
    ips200_show_string(  0, 16 * 8, "KEY1:I+0.01");
    ips200_show_string(120, 16 * 8, "KEY2:I-0.01");
    ips200_show_string(  0, 16 * 9, "KEY3:SavePar");
    ips200_show_string(120, 16 * 9, "KEY4:Get Par");
    ips200_show_float ( 80, 16 * 0, Parameter_set0.ServePID[0], 2, 3);
    ips200_show_float ( 80, 16 * 1, Parameter_set0.ServePID[1], 2, 3);
    ips200_show_float ( 80, 16 * 2, Parameter_set0.ServePID[2], 2, 3);
    ips200_show_float ( 48, 16 * 3, Angle, 4, 6);
    ips200_show_float ( 72, 16 * 4, angle[2], 3, 3);
    ips200_show_float ( 96, 16 * 5, Angle_Error, 3, 6);
    ips200_show_float ( 96, 16 * 6, Servo_Angle, 3, 3);
    ips200_show_float ( 88, 16 * 7, PID_SERVO.output, 3, 6);

    seekfree_assistant_oscilloscope_send(&oscilloscope_data);
    oscilloscope_data.data[0] = PID_SERVO.output;
    oscilloscope_data.data[1] = Servo_Angle;
    oscilloscope_data.data[2] = Angle_Error;
    oscilloscope_data.data[3] = Angle;
}
void ServoD_menu(void)
{
    ips200_show_string(  0, 16 * 0, "   ServoP:");
    ips200_show_string(  0, 16 * 1, "   ServoI:");
    ips200_show_string(  0, 16 * 2, "-->ServoD:");
    ips200_show_string(  0, 16 * 3, "Angle:");
    ips200_show_string(  0, 16 * 4, "angle[2]:");
    ips200_show_string(  0, 16 * 5, "Angle_Error:");
    ips200_show_string(  0, 16 * 6, "Servo_Angle:");
    ips200_show_string(  0, 16 * 7, "PID.output:");
    ips200_show_string(  0, 16 * 8, "KEY1:D+0.01");
    ips200_show_string(120, 16 * 8, "KEY2:D-0.01");
    ips200_show_string(  0, 16 * 9, "KEY3:SavePar");
    ips200_show_string(120, 16 * 9, "KEY4:Get Par");
    ips200_show_float ( 80, 16 * 0, Parameter_set0.ServePID[0], 2, 3);
    ips200_show_float ( 80, 16 * 1, Parameter_set0.ServePID[1], 2, 3);
    ips200_show_float ( 80, 16 * 2, Parameter_set0.ServePID[2], 2, 3);
    ips200_show_float ( 48, 16 * 3, Angle, 4, 6);
    ips200_show_float ( 72, 16 * 4, angle[2], 3, 3);
    ips200_show_float ( 96, 16 * 5, Angle_Error, 3, 6);
    ips200_show_float ( 96, 16 * 6, Servo_Angle, 3, 3);
    ips200_show_float ( 88, 16 * 7, PID_SERVO.output, 3, 6);

    seekfree_assistant_oscilloscope_send(&oscilloscope_data);
    oscilloscope_data.data[0] = PID_SERVO.output;
    oscilloscope_data.data[1] = Servo_Angle;
    oscilloscope_data.data[2] = Angle_Error;
    oscilloscope_data.data[3] = Angle;
}

void MotorP_menu(void)
{
    ips200_show_string(  0, 16 * 0, "-->MotorP:");
    ips200_show_string(  0, 16 * 1, "   MotorI:");
    ips200_show_string(  0, 16 * 2, "   MotorD:");
    ips200_show_string(  0, 16 * 3, "Test_Encoder:");
    ips200_show_string(  0, 16 * 4, "Encoder:");
    ips200_show_string(  0, 16 * 5, "PID.error:");
    ips200_show_string(  0, 16 * 6, "PID.output:");
    ips200_show_string(  0, 16 * 7, "KEY1:P+0.1");
    ips200_show_string(120, 16 * 7, "KEY2:P-0.1");
    ips200_show_string(  0, 16 * 8, "KEY3:Enco+100");
    ips200_show_string(120, 16 * 8, "KEY4:Enco-100");
    ips200_show_float ( 80, 16 * 0, Parameter_set0.SpeedPID[0], 2, 3);
    ips200_show_float ( 80, 16 * 1, Parameter_set0.SpeedPID[1], 2, 3);
    ips200_show_float ( 80, 16 * 2, Parameter_set0.SpeedPID[2], 2, 3);
    ips200_show_uint  (104, 16 * 3, Test_Encoder, 5);
    ips200_show_int   ( 64, 16 * 4, Encoder, 5);
    ips200_show_float ( 80, 16 * 5, PID_MOTOR.current_error, 3, 3);
    ips200_show_float ( 88, 16 * 6, PID_MOTOR.output, 3, 6);

    seekfree_assistant_oscilloscope_send(&oscilloscope_data);
    oscilloscope_data.data[0] = PID_MOTOR.output;
    oscilloscope_data.data[1] = Encoder;
    oscilloscope_data.data[2] = Test_Encoder;
}

void MotorI_menu(void)
{
    ips200_show_string(  0, 16 * 0, "   MotorP:");
    ips200_show_string(  0, 16 * 1, "-->MotorI:");
    ips200_show_string(  0, 16 * 2, "   MotorD:");
    ips200_show_string(  0, 16 * 3, "Test_Encoder:");
    ips200_show_string(  0, 16 * 4, "Encoder:");
    ips200_show_string(  0, 16 * 5, "PID.error:");
    ips200_show_string(  0, 16 * 6, "PID.output:");
    ips200_show_string(  0, 16 * 7, "KEY1:I+0.1");
    ips200_show_string(120, 16 * 7, "KEY2:I-0.1");
    ips200_show_string(  0, 16 * 8, "KEY3:Enco+100");
    ips200_show_string(120, 16 * 8, "KEY4:Enco-100");
    ips200_show_float ( 80, 16 * 0, Parameter_set0.SpeedPID[0], 2, 3);
    ips200_show_float ( 80, 16 * 1, Parameter_set0.SpeedPID[1], 2, 3);
    ips200_show_float ( 80, 16 * 2, Parameter_set0.SpeedPID[2], 2, 3);
    ips200_show_uint  (104, 16 * 3, Test_Encoder, 5);
    ips200_show_int   ( 64, 16 * 4, Encoder, 5);
    ips200_show_float ( 80, 16 * 5, PID_MOTOR.current_error, 3, 3);
    ips200_show_float ( 88, 16 * 6, PID_MOTOR.output, 3, 6);

    seekfree_assistant_oscilloscope_send(&oscilloscope_data);
    oscilloscope_data.data[0] = PID_MOTOR.output;
    oscilloscope_data.data[1] = Encoder;
    oscilloscope_data.data[2] = Test_Encoder;
}


void MotorD_menu(void)
{
    ips200_show_string(  0, 16 * 0, "   MotorP:");
    ips200_show_string(  0, 16 * 1, "   MotorI:");
    ips200_show_string(  0, 16 * 2, "-->MotorD:");
    ips200_show_string(  0, 16 * 3, "Test_Encoder:");
    ips200_show_string(  0, 16 * 4, "Encoder:");
    ips200_show_string(  0, 16 * 5, "PID.error:");
    ips200_show_string(  0, 16 * 6, "PID.output:");
    ips200_show_string(  0, 16 * 7, "KEY1:D+0.1");
    ips200_show_string(120, 16 * 7, "KEY2:D-0.1");
    ips200_show_string(  0, 16 * 8, "KEY3:Enco+100");
    ips200_show_string(120, 16 * 8, "KEY4:Enco-100");
    ips200_show_float ( 80, 16 * 0,Parameter_set0.SpeedPID[0], 2, 3);
    ips200_show_float ( 80, 16 * 1,Parameter_set0.SpeedPID[1], 2, 3);
    ips200_show_float ( 80, 16 * 2,Parameter_set0.SpeedPID[2], 2, 3);
    ips200_show_uint  (104, 16 * 3, Test_Encoder, 5);
    ips200_show_int   ( 64, 16 * 4, Encoder, 5);
    ips200_show_float ( 80, 16 * 5, PID_MOTOR.current_error, 3, 3);
    ips200_show_float ( 88, 16 * 6, PID_MOTOR.output, 3, 6);

    seekfree_assistant_oscilloscope_send(&oscilloscope_data);
    oscilloscope_data.data[0] = PID_MOTOR.output;
    oscilloscope_data.data[1] = Encoder;
    oscilloscope_data.data[2] = Test_Encoder;
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
            // KEY1����ͨ��3���¶����Լ�¼��λ
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS || Channal_3_Press_Flag)
            {
                if(gnss.state == 1)
                {
                    lat_union[Point_NUM].double_type = gnss.latitude; // ż������γ��latitude
                    lon_union[Point_NUM].double_type = gnss.longitude;// �������澭��longitude
                    GPS_GET_LAT[Point_NUM] = gnss.latitude;
                    GPS_GET_LOT[Point_NUM] = gnss.longitude;
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
            if(key_get_state(KEY_1) == KEY_LONG_PRESS)
            {
                Point_NUM = Task1_Start_Point;
            }
            if(key_get_state(KEY_2) == KEY_LONG_PRESS)
            {
                Point_NUM = Task2_Start_Point;
            }
            if(key_get_state(KEY_3) == KEY_LONG_PRESS)
            {
                Point_NUM = Task3_Start_Point;
            }
        }

        // ServoP����
        if(func_index == 5)
        {
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                Parameter_set0.ServePID[0] += 0.01;
            }
            if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                Parameter_set0.ServePID[0] -= 0.01;
            }
            if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
            {
                FLASH_SAV_PAR();
            }
            if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
            {
                FLASH_GET_PAR();
            }
        }

        // ServoI����
        if(func_index == 6)
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
            if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
            {
                FLASH_GET_PAR();
            }
        }

        // ServoD����
        if(func_index == 7)
        {
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                Parameter_set0.ServePID[2] += 0.01;
            }
            if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                Parameter_set0.ServePID[2] -= 0.01;
            }
            if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
            {
                FLASH_SAV_PAR();
            }
            if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
            {
                FLASH_GET_PAR();
            }
        }

        // MotorP����
        if(func_index == 8)
        {
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                Parameter_set0.SpeedPID[0] += 0.1;
            }
            if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                Parameter_set0.SpeedPID[0] -= 0.1;
            }
            if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
            {
                if(Test_Encoder < PWM_DUTY_MAX)
                {
                    Test_Encoder += 100;
                }
            }
            if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
            {
                if(Test_Encoder > -PWM_DUTY_MAX)
                {
                    Test_Encoder -= 100;
                }
            }
        }

        // MotorI����
        if(func_index == 9)
        {
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                Parameter_set0.SpeedPID[1] += 0.1;
            }
            if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                Parameter_set0.SpeedPID[1] -= 0.1;
            }
            if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
            {
                if(Test_Encoder < PWM_DUTY_MAX)
                {
                    Test_Encoder += 100;
                }
            }
            if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
            {
                if(Test_Encoder > -PWM_DUTY_MAX)
                {
                    Test_Encoder -= 100;
                }
            }
        }

        //MotorD����
        if(func_index == 10)
        {
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                Parameter_set0.SpeedPID[2] += 0.1;
            }
            if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                Parameter_set0.SpeedPID[2] -= 0.1;
            }
            if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
            {
                if(Test_Encoder < PWM_DUTY_MAX)
                {
                    Test_Encoder += 100;
                }
            }
            if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
            {
                if(Test_Encoder > -PWM_DUTY_MAX)
                {
                    Test_Encoder -= 100;
                }
            }
        }

        if(func_index == 12)
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
        if(func_index == 14)
        {
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                if(Point1 > 0)
                {
                    Point1 = Point1 - 1;
                    ips200_clear();
                }
            }
            if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                if(Point1 < NUM_GPS_DATA - 1)
                {
                    Point1 = Point1 + 1;
                    ips200_clear();
                }
            }
            if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
            {
                if(GpsTgtEncod[Point1] < PWM_DUTY_MAX)
                {
                    GpsTgtEncod[Point1] += 100;
                }
            }
            if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
            {
                if(GpsTgtEncod[Point1] > 0)
                {
                    GpsTgtEncod[Point1] -= 100;
                }
            }
            if(key_get_state(KEY_1) == KEY_LONG_PRESS)
            {
                FLASH_SAV_PAR();
            }
        }

        // �������
        if(func_index == 15)
        {
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                if(Point1 > 0)
                {
                    Point1 = Point1 - 1;
                    ips200_clear();
                }
            }
            if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                if(Point1 < NUM_GPS_DATA - 1)
                {
                    Point1 = Point1 + 1;
                    ips200_clear();
                }
            }
            if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
            {
                GpsDistance[Point1] += 0.5;
            }
            if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
            {
                if(GpsDistance[Point1] > 0)
                {
                    GpsDistance[Point1] -= 0.5;
                }
            }
            if(key_get_state(KEY_1) == KEY_LONG_PRESS)
            {
                FLASH_SAV_PAR();
            }
        }

        // �����λ����
        if(func_index == 16)
        {
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                if(Task_Point_Set > 1)
                {
                    Task_Point_Set -= 1;
                }
            }
            if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                if(Task_Point_Set < 3)
                {
                    Task_Point_Set += 1;
                }
            }
            if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
            {
                if(Task_Point_Set == 1)
                {
                    Task1_Points += 1;
                }
                if(Task_Point_Set == 2)
                {
                    Task2_Points += 1;
                }
                if(Task_Point_Set == 3)
                {
                    Task3_Points += 1;
                }
            }
            if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
            {
                if(Task_Point_Set == 1)
                {
                    Task1_Points -= 1;
                }
                if(Task_Point_Set == 2)
                {
                    Task2_Points -= 1;
                }
                if(Task_Point_Set == 3)
                {
                    Task3_Points -= 1;
                }
            }
            if(key_get_state(KEY_1) == KEY_LONG_PRESS)
            {
                FLASH_SAV_PAR();
            }
        }

        //��λ�鿴����
        if(func_index == 20)
        {
            // ���뿪�����ϱ�ʾ��λ�л�
            if(!gpio_get_level(SWITCH1))
            {
                if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
                {
                    if(Point_NUM > 0)
                    {
                        if(Point > 0)
                        {
                            Point = Point - 1;
                            ips200_clear();
                        }
                    }
                }
                if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
                {
                    if(Point_NUM > 0)
                    {
                        if(Point < Point_NUM - 1)
                        {
                            Point = Point + 1;
                            ips200_clear();
                        }
                    }
                }
                if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
                {
                    FLASH_FIX_GPS();
                }
                if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
                {
                    for(int i = 0; i < Point_NUM; i++)
                    {
                        printf("{ lat: %lf, lng: %lf, name: \"%d\"}\r\n",GPS_GET_LAT[i], GPS_GET_LOT[i], i);
                    }
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
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                clip_value += 1;
            }
            if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                clip_value -= 1;
            }
        }

        if(func_index == 24)
        {
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                Kp_Ah += 0.1;
            }
            if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                Kp_Ah -= 0.1;
            }
            if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
            {
                Ki_Ah += 0.01;
            }
            if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
            {
                Ki_Ah -= 0.01;
            }
        }

        if(func_index == 26)
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
                func_index = 20;
                table[func_index].current_operation();
            }
        }

        if(func_index == 28)
        {
            // Ҫ����ʱ��ע�͵�PDLocServoCtrl()�е�Servo_Set
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

        if(func_index == 30)
        {
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                if(Point2 > 0)
                {
                    Point2 = Point2 - 1;
                    ips200_clear();
                }
            }
            if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                if(Point2 < 100)
                {
                    Point2 = Point2 + 1;
                    ips200_clear();
                }
            }
            if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
            {
                if(Point2 == 0)
                {
                    Fly_Slope_Alpha += 100;
                }
                if(Point2 == 1)
                {
                    K_Straight += 0.01;
                }
                if(Point2 == 2)
                {
                    From_0000_To_2000_ServoPD.Kp += 0.01;
                }
                if(Point2 == 3)
                {
                    From_0000_To_2000_ServoPD.Kd += 0.01;
                }
                if(Point2 == 4)
                {
                    From_2000_To_4000_ServoPD.Kp += 0.01;
                }
                if(Point2 == 5)
                {
                    From_2000_To_4000_ServoPD.Kd += 0.01;
                }
                if(Point2 == 6)
                {
                    From_4000_To_5000_ServoPD.Kp += 0.01;
                }
                if(Point2 == 7)
                {
                    From_4000_To_5000_ServoPD.Kd += 0.01;
                }
                if(Point2 == 8)
                {
                    From_5000_To_6000_ServoPD.Kp += 0.01;
                }
                if(Point2 == 9)
                {
                    From_5000_To_6000_ServoPD.Kd += 0.01;
                }
                if(Point2 == 10)
                {
                    From_6000_To_7000_ServoPD.Kp += 0.01;
                }
                if(Point2 == 11)
                {
                    From_6000_To_7000_ServoPD.Kd += 0.01;
                }
                if(Point2 == 12)
                {
                    From_7000_To_8000_ServoPD.Kp += 0.01;
                }
                if(Point2 == 13)
                {
                    From_7000_To_8000_ServoPD.Kd += 0.01;
                }
                if(Point2 == 14)
                {
                    From_8000_To_9000_ServoPD.Kp += 0.01;
                }
                if(Point2 == 15)
                {
                    From_8000_To_9000_ServoPD.Kd += 0.01;
                }
                if(Point2 == 16)
                {
                    From_9000_To_9900_ServoPD.Kp += 0.01;
                }
                if(Point2 == 17)
                {
                    From_9000_To_9900_ServoPD.Kd += 0.01;
                }
            }
            if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
            {
                if(Point2 == 0)
                {
                    Fly_Slope_Alpha -= 100;
                }
                if(Point2 == 1)
                {
                    K_Straight -= 0.01;
                }
                if(Point2 == 2)
                {
                    From_0000_To_2000_ServoPD.Kp -= 0.01;
                }
                if(Point2 == 3)
                {
                    From_0000_To_2000_ServoPD.Kd -= 0.01;
                }
                if(Point2 == 4)
                {
                    From_2000_To_4000_ServoPD.Kp -= 0.01;
                }
                if(Point2 == 5)
                {
                    From_2000_To_4000_ServoPD.Kd -= 0.01;
                }
                if(Point2 == 6)
                {
                    From_4000_To_5000_ServoPD.Kp -= 0.01;
                }
                if(Point2 == 7)
                {
                    From_4000_To_5000_ServoPD.Kd -= 0.01;
                }
                if(Point2 == 8)
                {
                    From_5000_To_6000_ServoPD.Kp -= 0.01;
                }
                if(Point2 == 9)
                {
                    From_5000_To_6000_ServoPD.Kd -= 0.01;
                }
                if(Point2 == 10)
                {
                    From_6000_To_7000_ServoPD.Kp -= 0.01;
                }
                if(Point2 == 11)
                {
                    From_6000_To_7000_ServoPD.Kd -= 0.01;
                }
                if(Point2 == 12)
                {
                    From_7000_To_8000_ServoPD.Kp -= 0.01;
                }
                if(Point2 == 13)
                {
                    From_7000_To_8000_ServoPD.Kd -= 0.01;
                }
                if(Point2 == 14)
                {
                    From_8000_To_9000_ServoPD.Kp -= 0.01;
                }
                if(Point2 == 15)
                {
                    From_8000_To_9000_ServoPD.Kd -= 0.01;
                }
                if(Point2 == 16)
                {
                    From_9000_To_9900_ServoPD.Kp -= 0.01;
                }
                if(Point2 == 17)
                {
                    From_9000_To_9900_ServoPD.Kd -= 0.01;
                }
            }
            if(key_get_state(KEY_1) == KEY_LONG_PRESS)
            {
                FLASH_SAV_PAR();
            }
        }

        if(func_index == 32)
        {
            if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                Track_Points_NUM = 0;
                Task_Flag = 1;
            }
            if(key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                Track_Points_NUM = 10;
                Task_Flag = 2;
            }
            if(key_get_state(KEY_3) == KEY_SHORT_PRESS)
            {
                Track_Points_NUM = 50;
                Task_Flag = 3;
            }
            if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
            {
                Start_Flag = 1;
                LED_Buzzer_Flag_Ctrl(LED3);
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





