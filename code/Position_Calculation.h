/*
 * Position_Calculation.h
 *
 *  Created on: 2025��1��9��
 *      Author: 20483
 */

#ifndef CODE_POSITION_CALCULATION_H_
#define CODE_POSITION_CALCULATION_H_


//===================================================�궨��BEG===================================================
//#define Points_Num       (     14     )                      // �ȵ����
//===================================================�궨��END===================================================


//===================================================ȫ�ֱ���BEG===================================================
extern int    Track_Points_NUM;                                // ��ǰ׷�ٵڼ�����
extern double Angle_Error;                                     // �Ƕ����
extern float  Fusion_angle;                                    // GPS��IMU�����˲���ĽǶ�
extern float  Fusion_alpha;                                    // GPS��IMU�����˲���Ȩ��
extern int16  Target_Duty;                                     // ռ�ձ�
//===================================================ȫ�ֱ���END===================================================


//===================================================��������BEG===================================================
void Stright_Some_Distance(void);                              // ֱ��һ�ξ���
void GPS_IMU_Complementary_Filtering(void);                    // GPS��IMU�����˲�
void Track_Follow(void);                                       // ����ѭ������
void Point_Switch(void);                                       // ��λ�л�
//===================================================��������END===================================================

#endif /* CODE_POSITION_CALCULATION_H_ */
