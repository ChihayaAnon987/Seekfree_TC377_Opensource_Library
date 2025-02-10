/*
 * Common_peripherals.c
 *
 *  Created on: 2025��1��7��
 *      Author: 20483
 */
#include "zf_common_headfile.h"

int16 Servo_Angle = SERVO_MOTOR_MID;
int16 Encoder;
float System_Time = 0;
void CPU0_Init()
{
    AHRS_init();                                                    // AHRS��ʼ��
    IMU_Init();                                                     // IMU��ʼ��
    gnss_init(TAU1201);                                             // GPS��ʼ��
    SERVO_Init();                                                   // �����ʼ��
    DRV8701_Init();                                                 // �����ʼ��
    pit_ms_init(CCU60_CH0, 5);                                      // �жϣ�IMU���ݲɼ�
    pit_ms_init(CCU60_CH1, 5);                                      // �ж� ��������������PID
    pit_ms_init(CCU61_CH0, 100);                                    // �ж� GPS���ݽ���
    encoder_dir_init(ENCODER1_TIM, ENCODER1_PLUS, ENCODER1_DIR);    // ��������ʼ��
    wireless_uart_init();                                           // ��ʼ�����ߴ���
    Oscilloscope_Init(8);                                           // ���ʾ������ʼ��
}

void CPU1_Init()
{
    Buzzer_Init();                                                  // ��������ʼ��
//    uart_receiver_init();                                           // sbus���ջ���ʼ��
    KEY_Init();                                                     // ������ʼ��
    //mt9v03x_init();                                                 // ������ʼ��
    key_init(10);                                                   // ������ʼ��
    ips200_init(IPS200_TYPE);                                       // ��Ļ��ʼ��
    Buzzer_Check(200);                                              // �Լ죬��ʾ��ʼ���ɹ�
}

void Oscilloscope_Init(uint8 Channel_Num)
{
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_WIRELESS_UART); // ������ֳ�ʼ��
    for(int i = 0; i < Channel_Num; i++)
    {
        oscilloscope_data.data[i] = 0;
    }
    oscilloscope_data.channel_num = Channel_Num;
}

// DRV8701��ʼ��
void DRV8701_Init(void)
{
    gpio_init(DIR_CH1, GPO, 0, GPO_PUSH_PULL);
    pwm_init (PWM_CH1, 17000, 0);

}

void DRV8701_MOTOR_DRIVER(int Motor_PWM)
{   
    Motor_PWM = IntClip(Motor_PWM, -PWM_DUTY_MAX, PWM_DUTY_MAX); // �޷�
    if(Motor_PWM >= 0)                      // �����ת
    {
        gpio_set_level(DIR_CH1, 1);
        pwm_set_duty  (PWM_CH1, Motor_PWM);
    }
    else                                // �����ת
    {
        gpio_set_level(DIR_CH1, 0);
        pwm_set_duty  (PWM_CH1, -Motor_PWM);
    }
}

void Encoder_Get()
{
    Delta_x = 0;
    Delta_y = 0;
    Encoder = encoder_get_count(ENCODER1_TIM);                          // �ɼ���Ӧ����������
    encoder_clear_count(ENCODER1_TIM);                                  // �����Ӧ����

    double speed= (-1) * Encoder * 1000 * 45 / 248;                     // �����ٶ�m/s���Ŵ���1000����
    Delta_x += speed * 0.005 * cos(Yaw * PI / 180);                     // ÿ������λ��
    Delta_y += speed * 0.005 * sin(Yaw * PI / 180);

}

// �����ʼ��
void SERVO_Init(void)
{
    pwm_init(SERVO_MOTOR_PWM, SERVO_MOTOR_FREQ, (uint32)SERVO_MOTOR_DUTY(SERVO_MOTOR_MID));// ���ţ����Ƶ�ʣ�ռ�ձ�
}

// �����������
void Servo_SetTest(int16 *angle)
{
    // ����޷�
    if(*angle > SERVO_MOTOR_LMAX) {*angle = SERVO_MOTOR_LMAX;}
    if(*angle < SERVO_MOTOR_RMAX) {*angle = SERVO_MOTOR_RMAX;}

    pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(*angle));
}

// �������
void Servo_Set(int16 angle)
{
    // ����޷�
    if(angle > SERVO_MOTOR_LMAX) {angle = SERVO_MOTOR_LMAX;}
    if(angle < SERVO_MOTOR_RMAX) {angle = SERVO_MOTOR_RMAX;}

    pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(angle));
}

// ��������ʼ��
void Buzzer_Init()
{
    gpio_init(BUZZER_PIN, GPO, 0, GPO_PUSH_PULL);
}

// �������Լ캯��
void Buzzer_Check(int time)
{
    gpio_set_level(BUZZER_PIN, 1);
    system_delay_ms(time);
    gpio_set_level(BUZZER_PIN, 0);
}

// ������ʼ��
void KEY_Init()
{
    gpio_init(LED1, GPO,GPIO_HIGH, GPO_PUSH_PULL);// ��ʼ��  LED1��� Ĭ�ϸߵ�ƽ �������ģʽ
    gpio_init(LED2, GPO,GPIO_HIGH, GPO_PUSH_PULL);// ��ʼ��  LED2��� Ĭ�ϸߵ�ƽ �������ģʽ
    gpio_init(LED3, GPO,GPIO_HIGH, GPO_PUSH_PULL);// ��ʼ��  LED3��� Ĭ�ϸߵ�ƽ �������ģʽ
    gpio_init(LED4, GPO,GPIO_HIGH, GPO_PUSH_PULL);// ��ʼ��  LED4��� Ĭ�ϸߵ�ƽ �������ģʽ

    gpio_init(KEY1, GPI, GPIO_HIGH, GPI_PULL_UP);// ��ʼ��  KEY1���� Ĭ�ϸߵ�ƽ ��������
    gpio_init(KEY2, GPI, GPIO_HIGH, GPI_PULL_UP);// ��ʼ��  KEY2���� Ĭ�ϸߵ�ƽ ��������
    gpio_init(KEY3, GPI, GPIO_HIGH, GPI_PULL_UP);// ��ʼ��  KEY3���� Ĭ�ϸߵ�ƽ ��������
    gpio_init(KEY4, GPI, GPIO_HIGH, GPI_PULL_UP);// ��ʼ��  KEY4���� Ĭ�ϸߵ�ƽ ��������

    gpio_init(SWITCH1, GPI,GPIO_HIGH, GPI_FLOATING_IN);// ��ʼ��  SWITCH1��� Ĭ�ϸߵ�ƽ ��������
    gpio_init(SWITCH2, GPI,GPIO_HIGH, GPI_FLOATING_IN);// ��ʼ��  SWITCH2��� Ĭ�ϸߵ�ƽ ��������

}


void System_Time_Count()
{
    System_Time += 0.005;
}

