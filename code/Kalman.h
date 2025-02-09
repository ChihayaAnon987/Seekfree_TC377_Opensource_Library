/*
 * Kalman.h
 *
 *  Created on: 2025��2��5��
 *      Author: 20483
 */

#ifndef CODE_KALMAN_H_
#define CODE_KALMAN_H_

//===================================================���Ͷ���BEG===================================================
typedef struct                                                 // ��������ƫ�����ṹ��
{
    float Xdata;                                               // ��ƫ����X
    float Ydata;                                               // ��ƫ����Y
    float Zdata;                                               // ��ƫ����Z
} kalman_param_t;
//===================================================���Ͷ���END===================================================


//===================================================ȫ�ֱ���BEG===================================================
extern float angle[3];
extern kalman_param_t Kalman_Offset;
extern float kalman_Offset_flag;
//===================================================ȫ�ֱ���END===================================================


//===================================================��������BEG===================================================
float  invSqrt(float x);                                        // ���ټ���1/Sqrt(x)
void   AHRS_init(void);                                         // ��ʼ��IMU���
void   Kalman_Offset_Init(void);                                // ��������ƫ��ʼ��
void   AHRS_getYawPitchRoll(float *angle);                      // ����AHRS ������Ԫ��
void   MatrixAdd(float* fMatrixA, float* fMatrixB, float* Result, unsigned int m, unsigned int n);
void   MatrixSub(float* fMatrixA, float* fMatrixB, float* Result, unsigned int m, unsigned int n);
void   MatrixMultiply(float* fMatrixA, unsigned int uRowA, unsigned int uColA, float* fMatrixB, unsigned int uRowB, unsigned int uColB, float* MatrixResult);
void   MatrixTranspose(float* fMatrixA, unsigned int m, unsigned int n, float* fMatrixB);
void   MatrixProduct(float* A, int m, int n, float* B, int k, float* C);
void   MatrixE(float* fMatrixA, unsigned int n);
double MatrixDet2(float* fMatrixA);
int    MatrixInverse2(float* fMatrixA, float* fMatrixB);
int    MatrixInverse(float* fMatrixA, int n, float ep);
void   UD(float* A, int n, float* U, float* D);
float  Norm(float* fMatrixA, int iRow, int iCol);
//===================================================��������END===================================================


#endif /* CODE_KALMAN_H_ */
