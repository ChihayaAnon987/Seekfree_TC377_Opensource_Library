/*
 * PID.c
 *
 *  Created on: 2025��1��12��
 *      Author: 20483
 */

#include "zf_common_headfile.h"

/*
    ����͵����PID�㷨�ο���[����](https://mp.weixin.qq.com/s/vwganbbwu0eX2j-ZmxWk_A)
*/

PIDController PID_Init;   // ���� PID ��ʼ��
PIDController PID_GPS;    // ���� GPS ����
PIDController PID_IMU;    // ���� IMU ����
PIDController PID_SERVO;  // ��� PID������
PIDController PID_MOTOR;  // ��� PID������


/****************************************************************************************************
//  @brief      ���Ʒ�Χ
//  @param      x              int�ͱ���
//  @param      low            ����
//  @param      up             ����
//  @return     int            �����޷�Χ�ڱ���
//  @since
//  Sample usage:
****************************************************************************************************/
int IntClip(int x, int low, int up)
{
    return x > up ? up : x < low ? low : x;
}

/****************************************************************************************************
//  @brief      ���Ʒ�Χ
//  @param      x              float�ͱ���
//  @param      low            ����
//  @param      up             ����
//  @return     float          �����޷�Χ�ڱ���
//  @since
//  Sample usage:
****************************************************************************************************/
float FloatClip(float x, float low, float up)
{
    return x > up ? up : x < low ? low : x;
}

/****************************************************************************************************
//  @brief      λ��ʽPID����
//  @param      *pid                   PID�ṹ��
//  @param      error                  ���
//  @return     output                 PID���
//  @since
//  Sample usage:
****************************************************************************************************/
float PidLocCtrl(PIDController * pid, float error)
{
    pid->integrator += error;
    FloatClip(pid->integrator, -pid->imax, pid->imax);

    pid->out_p = pid->Kp * error;
    pid->out_i = pid->Ki * pid->integrator;
    pid->out_d = pid->Kd * (error - pid->last_error);
    pid->last_error = error;
    pid->output = pid->out_p + pid->out_i + pid->out_d;

    return pid->output;
}

/****************************************************************************************************
//  @brief      ����ʽPID�㷨
//  @param      *pid                   PID�ṹ��
//  @param      error                  ���
//  @return     output                 PID���
//  @since
//  Sample usage:
****************************************************************************************************/
float PidIncCtrl(PIDController * pid, float error)
{
    pid->out_p = pid->Kp * (error - pid->last_error);
    pid->out_i = pid->Ki * error;
    pid->out_d = pid->Kd * ((error - pid->last_error) - pid->last_derivative);

    pid->last_derivative = error - pid->last_error;
    pid->last_error = error;

    pid->output += pid->out_p + pid->out_i + pid->out_d;
    return pid->output;
}

void PdGpsCtrl()
{
    int32 pwm = 0;
    PID_GPS.last_error = PID_GPS.current_error;
    PID_GPS.current_error = Yaw - Angle;
    if(PID_GPS.current_error > 180)
    {
        PID_GPS.current_error -= 360;
    }
    else if(PID_GPS.current_error < -180)
    {
        PID_GPS.current_error += 360;
    }
    pwm = (int32)(PID_GPS.current_error * PID_GPS.Kp + (PID_GPS.current_error - PID_GPS.last_error) * PID_GPS.Kd);
    if(pwm > 1300)
    {
        pwm = 1300;
    }
    else if(pwm < -1300)
    {
        pwm = -1300;
    }
    //pwm_set_duty(SERVO_MOTOR_PWM, (uint32)(SERVO_MOTOR_MID_DUTY - pwm));
    //DRV8701_MOTOR_DRIVER(3000);
}

/****************************************************************************************************
//  @brief      ��� PDλ��ʽ������
//  @param      void
//  @return     void
//  @since
//  Sample usage:PiLocServoCtrl();
****************************************************************************************************/
void PDLocServoCtrl()
{
    PID_SERVO.last_error    = PID_SERVO.current_error;
    PID_SERVO.current_error = Angle_Error;
    PID_SERVO.derivative    = PID_SERVO.current_error - PID_SERVO.last_error;
    PID_SERVO.output = Parameter_set0.ServePID[0] * PID_SERVO.current_error + 
                       Parameter_set0.ServePID[2] * PID_SERVO.derivative;

    Servo_Angle = Parameter_set0.Serve_Mid - PID_SERVO.output;
    if(Servo_Angle > SERVO_MOTOR_LMAX) {Servo_Angle = SERVO_MOTOR_LMAX;}
    if(Servo_Angle < SERVO_MOTOR_RMAX) {Servo_Angle = SERVO_MOTOR_RMAX;}
    pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(Servo_Angle));
}

/****************************************************************************************************
//  @brief      ��� PID����ʽ������
//  @param      TARGET_MOTOR_DUTY      Ŀ����ռ�ձ�
//  @return     void
//  @since
//  Sample usage:PIDIncMotorCtrl(3000);
****************************************************************************************************/
void PIDIncMotorCtrl(int16 TARGET_MOTOR_DUTY)
{
    PID_MOTOR.lastlast_error = PID_MOTOR.last_error;
    PID_MOTOR.last_error     = PID_MOTOR.current_error;
    PID_MOTOR.current_error  = TARGET_MOTOR_DUTY - Encoder;

    PID_MOTOR.output += Parameter_set0.SpeedPID[0] * (PID_MOTOR.current_error - PID_MOTOR.last_error) +
                        Parameter_set0.SpeedPID[1] * PID_MOTOR.current_error +
                        Parameter_set0.SpeedPID[2] * (PID_MOTOR.current_error - 2 * PID_MOTOR.last_error + PID_MOTOR.lastlast_error);
    FloatClip(PID_MOTOR.output, -PWM_DUTY_MAX, PWM_DUTY_MAX);
    int32 MOTOR_DUTY = (int32)PID_MOTOR.output;
    if(MOTOR_DUTY >= 0)
    {
        gpio_set_level(DIR_CH1, 1);
        pwm_set_duty  (PWM_CH1, MOTOR_DUTY);
    }
    else
    {
        gpio_set_level(DIR_CH1, 0);
        pwm_set_duty  (PWM_CH1, -MOTOR_DUTY);
    }
}


// /****************************************************************************************************
// //  @brief      λ��ʽPID����
// //  @param      *pid                   PID�ṹ��
// //  @param      error                  ���
// //  @return     output                 PID���
// //  @since
// //  Sample usage:
// ****************************************************************************************************/
// void PidLocCtrl(PIDController *pid, float measuredVal)
// {
//     pid->preError = pid->currentError;
//     pid->currentError = pid->targetVal- measuredVal;
//     pid->derivative = pid->currentError- pid->preError;
//     pid->integral += pid->currentError;
//     // ���ֱ��ʹ���
//     FloatClip(pid->integral, -pid->imax, pid->imax);

//     pid->output =
//     pid->Kp * pid->currentError +
//     pid->Ki * pid->integral +
//     pid->Kd * pid->derivative;
// }


// /****************************************************************************************************
// //  @brief      ����ʽPID�㷨
// //  @param      *pid                   PID�ṹ��
// //  @param      measuredVal            ����ֵ
// //  @return     void
// //  @since
// //  Sample usage:
// ****************************************************************************************************/
// void PidIncCtrl(PIDController *pid, float measuredVal)
// {
//     pid->prepreError = pid->preError;
//     pid->preError = pid->currentError;
//     pid->currentError = pid->targetVal- measuredVal;

//     pid->output +=
//     pid->Kp * (pid->currentError- pid->preError) +
//     pid->Ki * pid->currentError +
//     pid->Kd * (pid->currentError- 2 * pid->preError + pid->prepreError);
//  }


void GPS_IMU_COM_filtering()
{
    float K = 0.9;          // ����ϵ��
    float Fusion_angle;     // �ںϺ�ĺ����
    // �����ֵ�YAW�����GPS��direction���л����ں�
    Fusion_angle = K * Z_360 + ( 1 - K ) * gnss.direction;

}

/****************************************************************************************************
//  @brief      ����ƽ���˲���ʼ��
//  @param      move_filter            �ṹ�崫��
//  @return     void
//  @since
//  Sample usage:move_filter_init(&speed_filter);
****************************************************************************************************/
void move_filter_init(move_filter_struct *move_filter)
{
    move_filter->data_average   = 0;
    move_filter->data_sum       = 0;
    move_filter->index          = 0;
    //���û�������С
    move_filter->buffer_size    = MOVE_AVERAGE_SIZE;// ���建������С

    uint8 i;
    for(i = 0; i < move_filter->buffer_size; i ++)
    {
        move_filter->data_buffer[i] = 0;
    }
}

/****************************************************************************************************
//  @brief      ����ƽ���˲�����
//  @param      move_filter            �ṹ�崫��
//  @param      new_data               �����������
//  @return     void
//  @since
//  Sample usage:move_filter_calc(&speed_filter, 400);
****************************************************************************************************/
void move_filter_calc(move_filter_struct *move_filter, int32 new_data)//�ṹ�崫�Σ������������
{
    // �����µ���ֵ ��ȥ��ĩβ����ֵ ������µĺ�
    move_filter->data_sum = move_filter->data_sum + new_data - move_filter->data_buffer[move_filter->index];
    // ������ƽ��ֵ
    move_filter->data_average = move_filter->data_sum / move_filter->buffer_size;

    // ������д�뻺����
    move_filter->data_buffer[move_filter->index] = new_data;
    move_filter->index ++;
    if(move_filter->buffer_size <= move_filter->index)
    {
        move_filter->index = 0;
    }
}
