/*
 * Common_peripherals.c
 *
 *  Created on: 2025年1月7日
 *      Author: 20483
 */
#include "zf_common_headfile.h"

int16 Servo_Angle = SERVO_MOTOR_MID;
int16 Encoder;

void CPU0_Init()
{
    IMU_Init();                                                     // IMU初始化
    gnss_init(TAU1201);                                             // GPS初始化
    SERVO_Init();                                                   // 舵机初始化
    Wireless_Init(4);                                               // 无线串口初始化
    uart_receiver_init();                                           // sbus接收机初始化
    pit_ms_init(CCU60_CH0, 10);                                     // 中断，IMU数据采集，采样周期为0.01s
    pit_ms_init(CCU60_CH1, 5);                                      // 中断 编码器、舵机电机PID
    pit_ms_init(CCU61_CH0, 100);                                    // 中断 GPS数据解析
    encoder_dir_init(ENCODER1_TIM, ENCODER1_PLUS, ENCODER1_DIR);    // 编码器初始化
    PID_GPS.imax = 1000.0;
    PID_IMU.imax = 500.0;
    PID_MOTOR.imax = 1000.0;
}

void CPU1_Init()
{
    Buzzer_Init();                                                  // 蜂鸣器初始化
    KEY_Init();                                                     // 按键初始化
    //mt9v03x_init();                                                 // 总钻风初始化
    key_init(10);                                                   // 按键初始化
    ips200_init(IPS200_TYPE);                                       // 屏幕初始化
    Buzzer_Check(200);                                              // 自检，表示初始化成功
}

void Wireless_Init(uint8 Channel_Num)
{
    wireless_uart_init();                                           // 无线串口初始化
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_WIRELESS_UART); // 逐飞助手初始化
    seekfree_assistant_oscilloscope_struct oscilloscope_data;       // 初始化逐飞助手示波器的结构体
    oscilloscope_data.channel_num = Channel_Num;                    // 示波器通道数
}

// DRV8701初始化
void DRV8701_Init(void)
{
    gpio_init(DIR_CH1, GPO, 0, GPO_PUSH_PULL);
    pwm_init (PWM_CH1, 17000, 0);

}

void DRV8701_MOTOR_DRIVER(int Speed)
{
    Speed = IntClip(Speed, -PWM_DUTY_MAX, PWM_DUTY_MAX); // 限幅
    if(Speed >= 0)                      // 电机正转
    {
        gpio_set_level(DIR_CH1, 1);
        pwm_set_duty  (PWM_CH1, (uint32)Speed);
    }
    else                                // 电机反转
    {
        gpio_set_level(DIR_CH1, 0);
        pwm_set_duty  (PWM_CH1, (uint32)Speed);
    }
}

void Encoder_Get()
{
    Encoder = encoder_get_count(ENCODER1_TIM);                          // 采集对应编码器数据
    encoder_clear_count(ENCODER1_TIM);                                  // 清除对应计数

}

// 舵机初始化
void SERVO_Init(void)
{
    pwm_init(SERVO_MOTOR_PWM, SERVO_MOTOR_FREQ, (uint32)SERVO_MOTOR_DUTY(SERVO_MOTOR_MID));// 引脚，舵机频率，占空比
}

// 舵机驱动1
void SERVO_SET(int16 *angle)
{
    // 舵机限幅
    if(*angle > SERVO_MOTOR_LMAX) {*angle = SERVO_MOTOR_LMAX;}
    if(*angle < SERVO_MOTOR_RMAX) {*angle = SERVO_MOTOR_RMAX;}

    pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(*angle));
}

// 舵机驱动2
void Servo_Set2(int16 angle)
{
    // 舵机限幅
    if(angle > SERVO_MOTOR_LMAX) {angle = SERVO_MOTOR_LMAX;}
    if(angle < SERVO_MOTOR_RMAX) {angle = SERVO_MOTOR_RMAX;}

    pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(angle));
}

// 蜂鸣器初始化
void Buzzer_Init()
{
    gpio_init(BUZZER_PIN, GPO, 0, GPO_PUSH_PULL);
}

// 蜂鸣器自检函数
void Buzzer_Check(int time)
{
    gpio_set_level(BUZZER_PIN, 1);
    system_delay_ms(time);
    gpio_set_level(BUZZER_PIN, 0);
}

// 按键初始化
void KEY_Init()
{
    gpio_init(LED1, GPO,GPIO_HIGH, GPO_PUSH_PULL);// 初始化  LED1输出 默认高电平 推挽输出模式
    gpio_init(LED2, GPO,GPIO_HIGH, GPO_PUSH_PULL);// 初始化  LED2输出 默认高电平 推挽输出模式
    gpio_init(LED3, GPO,GPIO_HIGH, GPO_PUSH_PULL);// 初始化  LED3输出 默认高电平 推挽输出模式
    gpio_init(LED4, GPO,GPIO_HIGH, GPO_PUSH_PULL);// 初始化  LED4输出 默认高电平 推挽输出模式

    gpio_init(KEY1, GPI, GPIO_HIGH, GPI_PULL_UP);// 初始化  KEY1输入 默认高电平 上拉输入
    gpio_init(KEY2, GPI, GPIO_HIGH, GPI_PULL_UP);// 初始化  KEY2输入 默认高电平 上拉输入
    gpio_init(KEY3, GPI, GPIO_HIGH, GPI_PULL_UP);// 初始化  KEY3输入 默认高电平 上拉输入
    gpio_init(KEY4, GPI, GPIO_HIGH, GPI_PULL_UP);// 初始化  KEY4输入 默认高电平 上拉输入

    gpio_init(SWITCH1, GPI,GPIO_HIGH, GPI_FLOATING_IN);// 初始化  SWITCH1输出 默认高电平 浮空输入
    gpio_init(SWITCH2, GPI,GPIO_HIGH, GPI_FLOATING_IN);// 初始化  SWITCH2输出 默认高电平 浮空输入

}



