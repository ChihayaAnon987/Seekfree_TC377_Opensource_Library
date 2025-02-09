/*
 * RemoteCtrl.h
 *
 *  Created on: 2025年1月12日
 *      Author: 20483
 */

#ifndef CODE_REMOTECTRL_H_
#define CODE_REMOTECTRL_H_

//===================================================遥控约定BEG===================================================
// 通道1: 方向控制通道，用来控制舵机
// 通道2: 速度控制通道，用来控制电机
// 通道3: 两种状态，记录点位
// 通道4: 三种状态，控制车辆控制模式，低电平为循迹模式，否则为遥控模式
// 通道5: 两种状态，无用
// 通道6: 两种状态，无用
//===================================================遥控约定END===================================================


//===================================================宏定义BEG===================================================
#define CHANNAL_HIGH_LEVEL          (     1792     )         // 通道高电平
#define CHANNAL_LOW_LEVEL           (     192      )          // 通道低电平
//===================================================宏定义END===================================================


//===================================================全局变量BEG===================================================
extern int    Control_Flag;                                    // 遥控器控制标志位，每按下一次翻转一次，当它为1时是遥控模式，为0时是循迹模式
extern int    Channal_3_Press_Flag;                            // 通道3按键是否按下
extern int    RemoteCtrl_Speed;                                // 遥控器速度控制量
extern int16  RemoteCtrl_Direction;                            // 遥控器方向控制量
//===================================================全局变量END===================================================


//===================================================函数声明BEG===================================================
void RemoteCtrl_Program(void);                                 // 遥控程序，放到CPU1的主循环中
void RemoteCtrl_Direction_Speed(void);                         // 方向速度控制
void CtrlMode_Switch(void);                                    // 控制模式切换
void Is_Channal_3_Press(void);                                 // 检测通道3是否按下
//===================================================函数声明END===================================================



#endif /* CODE_REMOTECTRL_H_ */
