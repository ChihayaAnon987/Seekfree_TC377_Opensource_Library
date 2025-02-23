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
#define Task1_Start_Point   (      0      )                  // ��Ŀһ��ʼ��λ
#define Task2_Start_Point   (     10      )                  // ��Ŀ����ʼ��λ
#define Task3_Start_Point   (     50      )                  // ��Ŀ����ʼ��λ
#define QS                  (   8.99266   )                  // ����任����
//===================================================�궨��END===================================================


//===================================================ȫ�ֱ���BEG===================================================
extern uint32 Point_NUM;                                       // ��ǰ�ɼ��� GPS ������
extern float  K_Gps;                                           // �νӲ��ֵ�Ȩ��
extern double FilterPoint_Lat;                                 // �˲����γ��
extern double FilterPoint_Lon;                                 // �˲��ľ���
extern double Start_Lat;                                       // �����ľ���
extern double Start_Lon;                                       // ������γ��
extern double Straight_Lat;                                    // ֱ��10-20m�ľ���
extern double Straight_Lon;                                    // ֱ��10-20m��γ��
extern double Delta_Lat;                                       // Ư�ƾ���
extern double Delta_Lon;                                       // Ư��γ��
extern double Angle;                                           // ��λ��
extern double Delta_Angle;                                     // GPS�������ǵ�������ƫ���
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
extern int8   Task1_Points;                                    // ��Ŀһ���õ�λ����
extern int8   Task2_Points;                                    // ��Ŀ�����õ�λ����
extern int8   Task3_Points;                                    // ��Ŀ�����õ�λ����
extern float  GpsDistance[NUM_GPS_DATA];                       // �洢������������
extern int16  GpsTgtEncod[NUM_GPS_DATA];                       // �洢��λ�ٶȵ�����
//===================================================ȫ�ֱ���END===================================================


//===================================================��������BEG===================================================
void Get_Gps(void);                                            // ��ȡ�������Ϣ
void Get_Gps_Yaw(void);                                        // ��ȡGPSƫ����
//===================================================��������END===================================================


#endif /* CODE_GPS_H_ */
