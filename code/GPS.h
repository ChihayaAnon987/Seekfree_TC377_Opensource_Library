/*
 * GPS.h
 *
 *  Created on: 2025��1��8��
 *      Author: 20483
 */

#ifndef CODE_GPS_H_
#define CODE_GPS_H_

//===================================================�궨��BEG===================================================
#define NUM_GPS_DATA        (     100     )                  // GPS �ɼ�����
#define QS                  (   8.99266   )                  // ����任����
//===================================================�궨��END===================================================


//===================================================ȫ�ֱ���BEG===================================================
extern uint32 Point_NUM;                                       // ��ǰ�ɼ��� GPS ������
extern float  K_Gps;                                           // �νӲ��ֵ�Ȩ��
extern double FilterPoint_Lat;                                 // �˲����γ��
extern double FilterPoint_Lon;                                 // �˲��ľ���
// extern double Now_Lat;                                         // �������ԭ���γ��
// extern double Now_Lon;                                         // �������ԭ��ľ���
extern double Start_Lat;                                       // �����ľ���
extern double Start_Lon;                                       // ������γ��
extern double Delta_Lat;                                       // Ư�ƾ���
extern double Delta_Lon;                                       // Ư��γ��
extern double Angle;                                           // ��λ��
extern double Distance;                                        // �������һ����ľ���
extern float  Yaw;                                             // ƫ����
extern float  Gps_Yaw;                                         // GPSֱ�ӵõ���ƫ����
extern float  Gps_Yaw2;                                        // GPS�õ���ƫ���ǣ��ۼӺͣ�
extern float  Lat_Fix;                                         // γ������ϵ��
extern float  Lon_Fix;                                         // ��������ϵ��
extern double Delta_x;                                         // λ��
extern double Delta_y;                                         // λ��
extern double GPS_GET_LAT[NUM_GPS_DATA];                       // �洢γ�����ݵ�����
extern double GPS_GET_LOT[NUM_GPS_DATA];                       // �洢�������ݵ�����
//===================================================ȫ�ֱ���END===================================================


//===================================================��������BEG===================================================
void GL_CRC(void);                                             // �����ɵ㺯��
void Get_Gps(void);                                            // ��ȡ�������Ϣ
void Get_Gps_Yaw(void);                                        // ��ȡGPSƫ����
//===================================================��������END===================================================


#endif /* CODE_GPS_H_ */
