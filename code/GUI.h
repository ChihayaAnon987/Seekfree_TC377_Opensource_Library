/*
 * GUI.h
 *
 *  Created on: 2025��1��10��
 *      Author: 20483
 */

#ifndef CODE_GUI_H_
#define CODE_GUI_H_
//===================================================���Ͷ���BEG===================================================
typedef struct
{
    char current;                                              // ��ǰ��ʾ����
    char up;                                                   // �Ϸ�
    char down;                                                 // �·�
    char back;                                                 // ����
    char enter;                                                // ȷ��
    void (*current_operation)(void);                           // ��ǰ��ʾ����
} menu_table;

typedef struct
{
    float       ServePID[3];                                    // ���PID
    float       SpeedPID[3];                                    // �ٶ�PID
    int         Speed_Duty;                                     // ���Ե��ٶ�
    float       Distance;                                       // �������
    int         Serve_Mid;                                      // �����е�ɵ���ֵ�������ϴ��ܳ�������������´��ܳ�����ֵ��
}Parameter_set;                                                 // ��������
//===================================================���Ͷ���END===================================================


//===================================================ȫ�ֱ���BEG===================================================
extern Parameter_set Parameter_set0;                            // ��������
//===================================================ȫ�ֱ���END===================================================


//===================================================��������BEG===================================================


//===================================================һ���˵�BEG===================================================
void main_menu0(void);                                         // ���˵�0
void main_menu1(void);                                         // ���˵�1
void main_menu2(void);                                         // ���˵�2
void main_menu3(void);                                         // ���˵�3
void main_menu4(void);                                         // ���˵�4
void main_menu5(void);                                         // ���˵�5
void main_menu6(void);                                         // ���˵�6
void main_menu7(void);                                         // ���˵�7
void main_menu8(void);                                         // ���˵�8
void main_menu9(void);                                         // ���˵�9
//===================================================һ���˵�END===================================================


//===================================================�����˵�BEG===================================================
void CaiDian_menu(void);                                       // �˵����ȵ�
void PID_menu(void);                                           // �˵���PID��������
void GPS_menu(void);                                           // �˵���GPS
void spd_menu(void);                                           // �˵����ٶ�
void Distance_menu(void);                                      // �˵����������
void serve_mid_menu(void);                                     // �˵��������ֵ����
void RemoteCtrl_menu(void);                                    // �˵���ң��
void Points_menu(void);                                        // �˵�����
void ZongZuanF(void);                                          // �˵��������
void Imu963_menu(void);                                        // �˵���IMU963
void Flash_menu(void);                                         // �˵���Flash
void Servo_menu(void);                                         // �˵������
//===================================================�����˵�END===================================================


//===================================================�����˵�BEG===================================================
void P_menu(void);                                             // �˵���P
void I_menu(void);                                             // �˵���I
void D_menu(void);                                             // �˵���D
//===================================================�����˵�END===================================================


//===================================================�˵�����BEG===================================================
void Key_Ctrl_Menu(void);                                      // �������Ʋ˵�
void Menu_Control(void);                                       // �˵�����
//===================================================�˵�����END===================================================


//===================================================��������END===================================================

#endif /* CODE_GUI_H_ */