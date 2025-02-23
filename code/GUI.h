/*
 * GUI.h
 *
 *  Created on: 2025��1��10��
 *      Author: 20483
 */

#ifndef CODE_GUI_H_
#define CODE_GUI_H_


//===================================================�궨��BEG===================================================
#define Page_Point_Num          (     8     )                // ÿҳ��ʾ��ĸ�����1-8��
//===================================================�궨��END===================================================


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
extern seekfree_assistant_oscilloscope_struct oscilloscope_data;// ��ʼ���������ʾ�����Ľṹ��
extern double Test_Angle;                                       // ������
extern int16  Test_Encoder;                                     // ������
extern int16  Task_Flag;                                        // �����־
extern uint8  Start_Flag;                                       // ������־
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
void main_menu10(void);                                        // ���˵�10
void main_menu11(void);                                        // ���˵�11
//===================================================һ���˵�END===================================================


//===================================================�����˵�BEG===================================================
void CaiDian_menu(void);                                       // �˵����ȵ�
void ServoPID(void);                                           // �˵���ServoPID
void MotorPID(void);                                           // �˵���MotorPID
void GPS_menu(void);                                           // �˵���GPS
void spd_menu(void);                                           // �˵����ٶ�
void Distance_menu(void);                                      // �˵����������
void TaskPoint(void);                                          // �˵������������
void RemoteCtrl_menu(void);                                    // �˵���ң��
void Points_menu(void);                                        // �˵�����
void ZongZuanF(void);                                          // �˵��������
void Imu963_menu(void);                                        // �˵���IMU963
void Flash_menu(void);                                         // �˵���Flash
void Servo_menu(void);                                         // �˵������
void Param_Set(void);                                          // �˵�����������
void Task_Select(void);                                        // �˵�������ѡ��
//===================================================�����˵�END===================================================


//===================================================�����˵�BEG===================================================
void ServoP_menu(void);                                        // �˵���ServoP
void ServoI_menu(void);                                        // �˵���ServoI
void ServoD_menu(void);                                        // �˵���ServoD
void MotorP_menu(void);                                        // �˵���MotorP
void MotorI_menu(void);                                        // �˵���MotorI
void MotorD_menu(void);                                        // �˵���MotorD
//===================================================�����˵�END===================================================


//===================================================�˵�����BEG===================================================
void Key_Ctrl_Menu(void);                                      // �������Ʋ˵�
void Menu_Control(void);                                       // �˵�����
//===================================================�˵�����END===================================================


//===================================================��������END===================================================

#endif /* CODE_GUI_H_ */
