/*
 * PID.h
 *
 *  Created on: 2025��1��12��
 *      Author: 20483
 */

#ifndef CODE_PID_H_
#define CODE_PID_H_

//===================================================�궨��BEG===================================================
#define MOVE_AVERAGE_SIZE           (32)                     // ���建������С
//===================================================�궨��END===================================================


//===================================================���Ͷ���BEG===================================================
typedef struct
{
    float Kp;                                                  // ����ϵ��P
    float Ki;                                                  // ����ϵ��I
    float Kd;                                                  // ΢��ϵ��D
    float imax;                                                // �����޷�

    float out_p;                                               // KP���
    float out_i;                                               // KI���
    float out_d;                                               // KD���
    float output;                                              // PID���

    float integrator;                                          // ����ֵ
    float current_error;                                       // ���
    float last_error;                                          // �ϴ����
    float lastlast_error;                                      // ���ϴ����
    float derivative;                                          // ���΢��
    float last_derivative;                                     // �ϴ���������ϴ����֮��
    // unsigned long last_t;                                      // �ϴ�ʱ��
} PIDController;

// typedef struct
// {
//     float Kp;                                                  // ����ϵ��P
//     float Ki;                                                  // ����ϵ��I
//     float Kd;                                                  // ΢��ϵ��D
//     float imax;                                                // �����޷�

//     float targetVal;                                           // Ŀ��ֵ
//     float currentError;                                        // ��ǰʱ�̵����
//     float preError;                                            // ǰһʱ�̵����
//     float prepreError;                                         // ǰ���β���ʱ�̵����
//     float derivative;                                          // ���΢��
//     float integral;                                            // ������
//     float output;                                              // ���
// } PIDController;

typedef struct                                                  // ����ƽ���˲��ṹ��
{
    uint8_t index;                                              // �±�
    uint8_t buffer_size;                                        // ��������С
    int32_t data_buffer[MOVE_AVERAGE_SIZE];                     // ������
    int32_t data_sum;                                           // ���ݺ�
    int32_t data_average;                                       // ����ƽ��ֵ
} move_filter_struct;
//===================================================���Ͷ���END===================================================


//===================================================ȫ�ֱ���BEG===================================================
extern PIDController PID_Init;         // PID��ʼ��������
extern PIDController PID_GPS;          // GPS PID������
extern PIDController PID_IMU;          // IMU PID������
extern PIDController PID_SERVO;        // ��� PID������
extern PIDController PID_MOTOR;        // ��� PID������
//===================================================ȫ�ֱ���END===================================================


//===================================================��������BEG===================================================
int   IntClip(int x, int low, int up);                                  // �޷�����
float FloatClip(float x, float low, float up);                          // �޷�����
float PidLocCtrl(PIDController *pid, float error);                      // PIDλ��ʽ���������
float PidIncCtrl(PIDController *pid, float error);                      // PID����ʽ���������
void  PdGpsCtrl(void);                                                  // GPS PD������
void  PDLocServoCtrl(void);                                             // ��� PDλ��ʽ������
void  PIDIncMotorCtrl(int16 TARGET_MOTOR_ENCODER);                      // ��� PID����ʽ������
// void PidLocCtrl(PIDConntroller *pid, float measuredVal);                 // PIDλ��ʽ���������
// void PidIncCtrl(PIDController *pid, float measuredVal);                 // PID����ʽ���������
void move_filter_init(move_filter_struct *move_filter);                 // ����ƽ���˲���ʼ��
void move_filter_calc(move_filter_struct *move_filter, int32_t new_data); // ����ƽ���˲�����
//===================================================��������END===================================================
#endif /* CODE_PID_H_ */
