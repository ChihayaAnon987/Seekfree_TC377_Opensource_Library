/*
 * Common_peripherals.h
 * ͨ�����裬������������������������������������LED������
 *  Created on: 2025��1��7��
 *      Author: 20483
 */

#ifndef CODE_COMMON_PERIPHERALS_H_
#define CODE_COMMON_PERIPHERALS_H_

//===================================================�궨��BEG===================================================
#define IPS200_TYPE     (IPS200_TYPE_PARALLEL8)              // ˫������ ���������� ����궨����д IPS200_TYPE_PARALLEL8
                                                             // �������� SPI ������ ����궨����д IPS200_TYPE_SPI

#define BUZZER_PIN              (P33_10)                     // ��������������

#define LED1                    (P20_9)                      // LED1 ��������
#define LED2                    (P20_8)                      // LED2 ��������
#define LED3                    (P21_5)                      // LED3 ��������
#define LED4                    (P21_4)                      // LED4 ��������

#define KEY1                    (P20_6)                      // ����1 ��������
#define KEY2                    (P20_7)                      // ����2 ��������
#define KEY3                    (P11_2)                      // ����3 ��������
#define KEY4                    (P11_3)                      // ����4 ��������

#define SWITCH1                 (P33_11)                     // ����1 ��������
#define SWITCH2                 (P33_12)                     // ����2 ��������

#define MAX_DUTY            50                               // ���ռ�ձ��������
#define PWM_CH1             ATOM0_CH5_P02_5                  // PWM ����˿�
#define DIR_CH1             P02_4                            // �����������˿�

#define ENCODER1_TIM        TIM5_ENCODER                     // ��������ʱ��
#define ENCODER1_PLUS       TIM5_ENCODER_CH1_P10_3           // �����������˿�
#define ENCODER1_DIR        TIM5_ENCODER_CH2_P10_1           // �����������ֵ�˿�


#define SERVO_MOTOR_PWM             (ATOM1_CH1_P33_9)        // ���������϶����Ӧ����
#define SERVO_MOTOR_FREQ            (300)                    // ���������϶��Ƶ��  �����ע�ⷶΧ 50-300
#define SERVO_MOTOR_MID             (90)                     // ��ֵ
#define SERVO_MOTOR_LMAX            (129)                    // �󣬶��������϶�����Χ �Ƕ�
#define SERVO_MOTOR_RMAX            (51)                     // �ң����������϶�����Χ �Ƕ�
#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))

//===================================================�궨��END===================================================

//===================================================ȫ�ֱ���BEG===================================================
extern int16 Encoder;                                          // ����������ֵ
extern int16 Servo_Angle;                                      // ����Ƕ�
//===================================================ȫ�ֱ���END===================================================



//===================================================��������BEG===================================================
void CPU0_Init(void);                                          // CPU0 ��ʼ��
void CPU1_Init(void);                                          // CPU1 ��ʼ��
void Wireless_Init(uint8 Channel_Num);                         // ���ߴ��ڳ�ʼ��
void DRV8701_Init(void);                                       // �����ʼ��
void DRV8701_MOTOR_DRIVER(int Speed);                          // �������
void Encoder_Get(void);                                        // ��������ȡ
void SERVO_Init(void);                                         // �����ʼ��
void SERVO_SET(int16 *angle);                                  // �������1�����ڵ���
void Servo_Set2(int16 angle);                                  // �������2
void Buzzer_Init(void);                                        // ��������ʼ��
void Buzzer_Check(int time);                                   // �������Լ캯��
void KEY_Init(void);                                           // ������ʼ��
//===================================================��������END===================================================




#endif /* CODE_COMMON_PERIPHERALS_H_ */