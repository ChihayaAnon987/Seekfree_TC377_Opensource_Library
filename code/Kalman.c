/*
 * Kalman.c
 *
 *  Created on: 2025��2��5��
 *      Author: 20483
 */

#include "zf_common_headfile.h"

// volatile float exInt, eyInt, ezInt;         // ������
// volatile float Ya_offset = 0, P_offset = 0, R_offset = 0;
// volatile float integralFBhand, handdiff;
// volatile uint32_t lastUpdate, now;          // �������ڼ��� ��λ us
// float f;
kalman_param_t kalman_param;       // ��Ʈ����
float kalman_Offset_flag = 0;
volatile float q0, q1, q2, q3, w1, w2, w3;  // ȫ����Ԫ��
float angle[3] = {0};

float P[49] = {0.0001, 0, 0, 0, 0, 0, 0,
               0, 0.0001, 0, 0, 0, 0, 0,
               0, 0, 0.0001, 0, 0, 0, 0,
               0, 0, 0, 0.0001, 0, 0, 0,
               0, 0, 0, 0, 0.0002, 0, 0,
               0, 0, 0, 0, 0, 0.0002, 0,
               0, 0, 0, 0, 0, 0, 0.0002};

float Q[49] = {0.0001, 0, 0, 0, 0, 0, 0,
               0, 0.0001, 0, 0, 0, 0, 0,
               0, 0, 0.0001, 0, 0, 0, 0,
               0, 0, 0, 0.0001, 0, 0, 0,
               0, 0, 0, 0, 0.0005, 0, 0,
               0, 0, 0, 0, 0, 0.0005, 0,
               0, 0, 0, 0, 0, 0, 0.0005};

float R[36] = {0.0003, 0, 0, 0, 0, 0,
               0, 0.0003, 0, 0, 0, 0,
               0, 0, 0.0003, 0, 0, 0,
               0, 0, 0, 0.0002, 0, 0,
               0, 0, 0, 0, 0.0002, 0,
               0, 0, 0, 0, 0, 0.0002};

float A[49], B[49], E[42], FF[36], X[49], Z[49], Ht[42], Ft[49], K[42], O[49], T[6], F[49], Y[7], P1[49], U1[36], U1t[36], DD[36], X1[36], X2[36];
float H[42] = {
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0};

float I[49] = {1, 0, 0, 0, 0, 0, 0,
               0, 1, 0, 0, 0, 0, 0,
               0, 0, 1, 0, 0, 0, 0,
               0, 0, 0, 1, 0, 0, 0,
               0, 0, 0, 0, 1, 0, 0,
               0, 0, 0, 0, 0, 1, 0,
               0, 0, 0, 0, 0, 0, 1};

/****************************************************************************************************
//  @brief      ���ټ���1/Sqrt(x)
//  @param      x                      ����
//  @return     y                      1/Sqrt(x)
//  @since
//  Sample usage:
****************************************************************************************************/
float invSqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long*)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}

/****************************************************************************************************
//  @brief      IMU��س�ʼ��
//  @param      void                      
//  @return     void                   
//  @since
//  Sample usage:
****************************************************************************************************/
void AHRS_init()
{
    // imu963ra_init();

    // ������ƫ��
    // w1 = Gyro_Offset.Xdata; // 0.095f;
    // w2 = Gyro_Offset.Ydata; // 0.078f;
    // w3 = Gyro_Offset.Zdata; // -0.014f;
    w1 = 0.095f;
    w2 = 0.078f;
    w3 = -0.014f;

    q0 = 1.0;
    q1 = 0;
    q2 = 0;
    q3 = 0;
}


void AHRS_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) {
    float norm;
    float bx, bz;
    float vx, vy, vz, wx, wy, wz;
    float g = 9.79973;
    float Ha1, Ha2, Ha3, Ha4, Hb1, Hb2, Hb3, Hb4;
    float e1, e2, e3, e4, e5, e6;
    float halfT;

    // �Ȱ���Щ�õõ���ֵ���
    float q0q0 = q0 * q0;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;

    // �����ų�
    bx = 0.5500;
    bz = 0.8351;
    // bx = 0.0024;
    // bz = 0.0051;
    halfT = 0.005;

    norm = invSqrt(ax * ax + ay * ay + az * az);
    ax = ax * norm * g;
    ay = ay * norm * g;
    az = az * norm * g;

    norm = invSqrt(mx * mx + my * my + mz * mz);
    mx = mx * norm;
    my = my * norm;
    mz = mz * norm;

    gx = gx - w1;
    gy = gy - w2;
    gz = gz - w3;

    Ha1 = (-q2) * g;
    Ha2 = q3 * g;
    Ha3 = -q0 * g;
    Ha4 = q1 * g;
    Hb1 = bx * q0 - bz * q2;
    Hb2 = bx * q1 + bz * q3;
    Hb3 = -bx * q2 - bz * q0;
    Hb4 = -bx * q3 + bz * q1;

    H[0]  = Ha1;
    H[1]  = Ha2;
    H[2]  = Ha3;
    H[3]  = Ha4;
    H[7]  = Ha4;
    H[8]  = -Ha3;
    H[9]  = Ha2;
    H[10] = -Ha1;
    H[14] = -Ha3;
    H[15] = -Ha4;
    H[16] = Ha1;
    H[17] = Ha2;
    H[21] = Hb1;
    H[22] = Hb2;
    H[23] = Hb3;
    H[24] = Hb4;
    H[28] = Hb4;
    H[29] = -Hb3;
    H[30] = Hb2;
    H[31] = -Hb1;
    H[35] = -Hb3;
    H[36] = -Hb4;
    H[37] = Hb1;
    H[38] = Hb2;

    // ״̬����
    q0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * halfT;
    q1 = q1 + (q0 * gx + q2 * gz - q3 * gy) * halfT;
    q2 = q2 + (q0 * gy - q1 * gz + q3 * gx) * halfT;
    q3 = q3 + (q0 * gz + q1 * gy - q2 * gx) * halfT;
    // ��Ԫ����һ
    norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 = q0 * norm;
    q1 = q1 * norm;
    q2 = q2 * norm;
    q3 = q3 * norm;
    // F��ֵ
    F[0]  = 1;
    F[8]  = 1;
    F[16] = 1;
    F[24] = 1;
    F[32] = 1;
    F[40] = 1;
    F[48] = 1;
    F[1]  = -gx * halfT;
    F[2]  = -gz * halfT;
    F[3]  = -gz * halfT;
    F[4]  = 0;
    F[5]  = 0;
    F[6]  = 0;
    F[7]  = gx * halfT;
    F[9]  = gz * halfT;
    F[10] = -gy * halfT;
    F[11] = 0;
    F[12] = 0;
    F[13] = 0;
    F[14] = gy * halfT;
    F[15] = -gz * halfT;
    F[17] = gx * halfT;
    F[18] = 0;
    F[19] = 0;
    F[20] = 0;
    F[21] = gz * halfT;
    F[22] = gy * halfT;
    F[23] = -gx * halfT;
    F[25] = 0;
    F[26] = 0;
    F[27] = 0;
    F[28] = 0;
    F[29] = 0;
    F[30] = 0;
    F[31] = 0;
    F[33] = 0;
    F[34] = 0;
    F[35] = 0;
    F[36] = 0;
    F[37] = 0;
    F[38] = 0;
    F[39] = 0;
    F[41] = 0;
    F[42] = 0;
    F[43] = 0;
    F[44] = 0;
    F[45] = 0;
    F[46] = 0;
    F[47] = 0;

    // �������˲�
    MatrixMultiply(F, 7, 7, P, 7, 7, A);        // A=F*P
    MatrixTranspose(F, 7, 7, Ft);               // Fת��F'
    MatrixMultiply(A, 7, 7, Ft, 7, 7, B);       // B=A*F'=F*P*F'
    MatrixAdd(B, Q, P1, 7, 7);
    MatrixTranspose(H, 6, 7, Ht);               // Hת��H'
    MatrixMultiply(P1, 7, 7, Ht, 7, 6, E);      // E=P*H'
    MatrixMultiply(H, 6, 7, E, 7, 6, FF);       // FF=H*P*H' 6*6
    MatrixAdd(FF, R, X, 6, 6);                  // X=FF+R 6*6
    UD(X, 6, U1, DD);                           // X��UD�ֽ�
    MatrixTranspose(U1, 6, 6, U1t);             // U1��ת��
    MatrixMultiply(U1, 6, 6, DD, 6, 6, X1);     // X1=U1*DD
    MatrixMultiply(X1, 6, 6, U1t, 6, 6, X2);    // X2=U1*DD*U1t
    MatrixInverse(X2, 6, 0);                    // X��
    MatrixMultiply(E, 7, 6, X2, 6, 6, K);       // ����K   7*6

    vx = 2 * (q1q3 - q0q2) * g;
    vy = 2 * (q0q1 + q2q3) * g;
    vz = (q0q0 - q1q1 - q2q2 + q3q3) * g;

    wx = 2 * bx * (0.5 - q2q2 - q3q3) + 2 * bz * (q1q3 - q0q2);
    wy = 2 * bx * (q1q2 - q0q3) + 2 * bz * (q0q1 + q2q3);
    wz = 2 * bx * (q0q2 + q1q3) + 2 * bz * (0.5 - q1q1 - q2q2);

    e1 = ax - vx;
    e2 = ay - vy;
    e3 = az - vz;
    e4 = mx - wx;
    e5 = my - wy;
    e6 = mz - wz;
    T[0] = e1;
    T[1] = e2;
    T[2] = e3;
    T[3] = e4;
    T[4] = e5;
    T[5] = e6;
    MatrixMultiply(K, 7, 6, T, 6, 1, Y); // Y=K*(Z-Y) 7*1
    q0 = q0 + Y[0];
    q1 = q1 + Y[1];
    q2 = q2 + Y[2];
    q3 = q3 + Y[3];
    w1 = w1 + Y[4];
    w2 = w2 + Y[5];
    w3 = w3 + Y[6];

    MatrixMultiply(K, 7, 6, H, 6, 7, Z); // Z= K*H 7*7
    MatrixSub(I, Z, O, 7, 7); // O=I-K*H
    MatrixMultiply(O, 7, 7, P1, 7, 7, P);

    norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 = q0 * norm;
    q1 = q1 * norm;
    q2 = q2 * norm;
    q3 = q3 * norm;
}


/****************************************************************************************************
//  @brief      ��ȡ��Ԫ��
//  @param      q                     ��Ԫ������
//  @since
//  Sample usage:
****************************************************************************************************/
void AHRS_getQ(float * q)
{
    // IMU_getValues(mygetqval);
    AHRS_AHRSupdate(ANGLE_TO_RAD(imu963ra_gyro_transition(imu963ra_gyro_x)),
                    ANGLE_TO_RAD(imu963ra_gyro_transition(imu963ra_gyro_y)),
                    ANGLE_TO_RAD(imu963ra_gyro_transition(imu963ra_gyro_z)),
                    imu963ra_acc_x, imu963ra_acc_y, imu963ra_acc_z,
                    imu963ra_mag_x, imu963ra_mag_y, imu963ra_mag_z);
    // �������ǵĲ���ֵת�ɻ���ÿ��
    // ���ٶȺʹ����Ʊ���ADCֵ����Ҫת��

    q[0] = q0; // ���ص�ǰֵ
    q[1] = q1;
    q[2] = q2;
    q[3] = q3;
}



/****************************************************************************************************
//  @brief      ��ȡ��̬��
//  @param      angles                ��̬������
//  @since
//  Sample usage:
****************************************************************************************************/
void AHRS_getYawPitchRoll(float * angles)
{
    imu963ra_get_acc();     // ��ȡ���ٶȼ�����
    imu963ra_get_gyro();    // ��ȡ����������
    imu963ra_get_mag();     // ��ȡ����������
    float q[4];             // ��Ԫ��

    AHRS_getQ(q);           // ����ȫ����Ԫ��
    angles[0] = atan2(2 * q[0] * q[1] + 2 * q[2] * q[3], -2 * q[1] * q[1] - 2 * q[2] * q[2] + 1) * 180 / PI;// �����roll
    angles[1] = asin( 2 * q[0] * q[2] - 2 * q[1] * q[3]) * 180 / PI;                                        // ������pitch                   
    angles[2] = atan2(2 * q[0] * q[3] + 2 * q[1] * q[2], -2 * q[2] * q[2] - 2 * q[3] * q[3] + 1) * 180 / PI;// ƫ����yaw
    

}


void Kalman_Offset()
{
    kalman_param.Xdata = 0;
    kalman_param.Ydata = 0;
    kalman_param.Zdata = 0;

    for(uint16_t i = 0; i < 1000; i++)
    {
        kalman_param.Xdata += angle[0];
        kalman_param.Ydata += angle[1];
        kalman_param.Zdata += angle[2];
    }
    kalman_param.Xdata /= 1000;
    kalman_param.Ydata /= 1000;
    kalman_param.Zdata /= 1000;

    kalman_Offset_flag = 1;
}

/****************************************************************************************************
//  @brief      ����ӷ�
//  @param      fMatrixA              ��һ������
//  @param      fMatrixB              �ڶ�������
//  @param      Result                �������
//  @param      m                     ���������
//  @param      n                     ���������
//  @since
//  Sample usage:
****************************************************************************************************/
void MatrixAdd(float* fMatrixA, float* fMatrixB, float* Result, unsigned int m, unsigned int n)
{
	unsigned int index_i = 0;
	unsigned int index_j = 0;
	unsigned int itemp = 0;

	for (index_i = 0; index_i < m; index_i++)
		for (index_j = 0; index_j < n; index_j++)
		{
			itemp = index_i * n + index_j;
			*(Result + itemp) = *(fMatrixA + itemp) + *(fMatrixB + itemp);
		}
} 


/****************************************************************************************************
//  @brief      �������
//  @param      fMatrixA              ��һ������
//  @param      fMatrixB              �ڶ�������
//  @param      Result                �������
//  @param      m                     ���������
//  @param      n                     ���������
//  @since
//  Sample usage:
****************************************************************************************************/
void MatrixSub(float* fMatrixA, float* fMatrixB, float* Result, unsigned int m, unsigned int n)
{
    unsigned int index_i = 0;
    unsigned int index_j = 0;
    unsigned int itemp = 0;

    for (index_i = 0; index_i < m; index_i++)
    {
        for (index_j = 0; index_j < n; index_j++)
        {
            itemp = index_i * n + index_j;
            *(Result + itemp) = *(fMatrixA + itemp) - *(fMatrixB + itemp);
        }
    }
}


/****************************************************************************************************
//  @brief      ����˷�
//  @param      fMatrixA              ��һ������
//  @param      uRowA                 ��һ�����������
//  @param      uColA                 ��һ�����������
//  @param      fMatrixB              �ڶ�������
//  @param      uRowB                 �ڶ������������
//  @param      uColB                 �ڶ������������
//  @param      MatrixResult          �������
//  @since
//  Sample usage:
****************************************************************************************************/
void MatrixMultiply(float* fMatrixA, unsigned int uRowA, unsigned int uColA,
                    float* fMatrixB, unsigned int uRowB, unsigned int uColB,
                    float* MatrixResult)
{
    unsigned int index_i = 0;
    unsigned int index_j = 0;
    unsigned int index_l = 0;
    unsigned int index_u = 0;
    unsigned int index_k = 0;
    unsigned int index_v = 0;

    for (index_i = 0; index_i < uRowA; index_i++)
    {
        for (index_j = 0; index_j < uColB; index_j++)
        {
            index_u = index_i * uColB + index_j;
            MatrixResult[index_u] = 0.0;

            for (index_l = 0; index_l < uColA; index_l++)
            {
                index_k = index_i * uColA + index_l;
                index_v = index_l * uColB + index_j;

                *(MatrixResult + index_u) += (*(fMatrixA + index_k)) * (*(fMatrixB + index_v));
            }
        }
    }
}


/****************************************************************************************************
//  @brief      ����ת��
//  @param      fMatrixA              �������
//  @param      m                     ������������
//  @param      n                     ������������
//  @param      fMatrixB              �������
//  @since
//  Sample usage:
****************************************************************************************************/
void MatrixTranspose(float* fMatrixA, unsigned int m, unsigned int n, float* fMatrixB)
{
    unsigned int index_i = 0;
    unsigned int index_j = 0;
    unsigned int index_u = 0;
    unsigned int index_v = 0;

    for (index_i = 0; index_i < m; index_i++)
    {
        for (index_j = 0; index_j < n; index_j++)
        {
            index_u = index_j * m + index_i;
            index_v = index_i * n + index_j;
            *(fMatrixB + index_u) = *(fMatrixA + index_v);
        }
    }
}


/****************************************************************************************************
//  @brief      ����˷�
//  @param      A                     ��һ������
//  @param      m                     ��һ�����������
//  @param      n                     ��һ�����������
//  @param      B                     �ڶ�������
//  @param      k                     �ڶ������������
//  @param      C                     �������
//  @since
//  Sample usage:
****************************************************************************************************/
void MatrixProduct(float* A, int m, int n, float* B, int k, float* C)
{
    int tem = 0;
    int i = 0;
    int j = 0;
    int l = 0;
    int met = 0;

    for (tem = 0; tem < m; tem++)
    {
        for (met = 0; met < k; met++)
        {
            C[tem * k + met] = 0.;
        }
    }

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < k; j++)
        {
            for (l = 0; l < n; l++)
            {
                C[i * k + j] = C[i * k + j] + A[i * n + l] * B[l * k + j];
            }
        }
    }
}


/****************************************************************************************************
//  @brief      ���ɵ�λ����
//  @param      fMatrixA              �������
//  @param      n                     �����ά��
//  @since
//  Sample usage:
****************************************************************************************************/
void MatrixE(float* fMatrixA, unsigned int n)
{
    unsigned int index_i = 0;
    unsigned int index_j = 0;

    for (; index_i < n; index_i++)
    {
        for (index_j = 0; index_j < n; index_j++)
        {
            if (index_i == index_j)
                *(fMatrixA + index_i * n + index_j) = 1;
            else
                *(fMatrixA + index_i * n + index_j) = 0;
        }
    }
}


/****************************************************************************************************
//  @brief      ������׾��������ʽ
//  @param      fMatrixA              �������
//  @since
//  Sample usage:
****************************************************************************************************/
double MatrixDet2(float* fMatrixA)
{
    return (*fMatrixA) * (*(fMatrixA + 3)) - (*(fMatrixA + 1)) * (*(fMatrixA + 2));
}


/****************************************************************************************************
//  @brief      ������׾���������
//  @param      fMatrixA              �������
//  @param      fMatrixB              �������
//  @return     0���ɹ���1��ʧ��
//  @since
//  Sample usage:
****************************************************************************************************/
int MatrixInverse2(float* fMatrixA, float* fMatrixB)
{
    float fmatrix_vlaue = 0.0;

    fmatrix_vlaue = MatrixDet2(fMatrixA);

    if (fmatrix_vlaue == 0.0)
    {
        return 1;
    }

    *fMatrixB = *(fMatrixA + 3) / fmatrix_vlaue;
    *(fMatrixB + 1) = -(*(fMatrixA + 2) / fmatrix_vlaue);
    *(fMatrixB + 2) = -(*(fMatrixA + 1) / fmatrix_vlaue);
    *(fMatrixB + 3) = -(*(fMatrixA) / fmatrix_vlaue);
    return 0;
}


/****************************************************************************************************
//  @brief      �������������
//  @param      fMatrixA              �������
//  @param      n                     �����ά��
//  @param      ep                    ����
//  @return     0���ɹ���1��ʧ��
//  @since
//  Sample usage:
****************************************************************************************************/
int MatrixInverse(float* fMatrixA, int n, float ep)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    int u = 0;
    int v = 0;
    int temp_row[10], temp_col[10];
    int *prow = temp_row, urow = 0;    //�ҵ������ֵ������
    int *pcol = temp_col, ucol = 0;    //�ҵ������ֵ������
    float ftemp_max = 0.0;
    float ftemp = 0.0;

    // ep = ep;                    //��ֹepû��ʹ�õľ���

    for (i = 0; i < 10; i++)
    {
        temp_row[i] = 0;
        temp_col[i] = 0;
    }

    for (k = 0; k < n; k++)
    {
        ftemp_max = 0.0;    //�ҵ�ʣ��������е����ֵ
        for (i = k; i < n; i++)
            for (j = k; j < n; j++)
            {
                l = i * n + j;
                ftemp = fabs(*(fMatrixA + l));
                if (ftemp_max < ftemp)
                {
                    ftemp_max = ftemp;
                    *(prow + k) = urow = i;
                    *(pcol + k) = ucol = j;
                }
            }

        if (ftemp_max < ep)
        {
            return 0;
        }

        if (urow != k)    //�����ǰ�в��������ֵ����
        {
            for (j = 0; j < n; j++)
            {
                u = k * n + j;
                v = urow * n + j;
                ftemp = *(fMatrixA + u);
                *(fMatrixA + u) = *(fMatrixA + v);
                *(fMatrixA + v) = ftemp;
            }
        }

        if (ucol != k)    //�����ǰ�в��������ֵ����
        {
            for (i = 0; i < n; i++)
            {
                u = i * n + k;
                v = i * n + ucol;
                ftemp = *(fMatrixA + u);
                *(fMatrixA + u) = *(fMatrixA + v);
                *(fMatrixA + v) = ftemp;
            }
        }

        l = k * n + k;
        ftemp = *(fMatrixA + l) = 1.0 / (*(fMatrixA + l));

        for (j = 0; j < n; j++)
        {
            if (j != k)
            {
                u = k * n + j;
                *(fMatrixA + u) *= *(fMatrixA + l);
            }
        }

        for (i = 0; i < n; i++)    //����֪��Ϊʲô
        {
            if (i != k)
            {
                for (j = 0; j < n; j++)
                {
                    if (j != k)
                    {
                        u = i * n + j;
                        *(fMatrixA + u) -= (*(fMatrixA + i * n + k)) * (*(fMatrixA + k * n + j));
                    }
                }
            }
        }

        for (i = 0; i < n; i++)
        {
            if (i != k)
            {
                u = i * n + k;
                *(fMatrixA + u) = -(*(fMatrixA + u)) * (*(fMatrixA + l));
            }
        }
    }

    for (k = n - 1; k >= 0; k--)
    {
        if ((*(pcol + k)) != k)
        {
            for (j = 0; j < n; j++)
            {
                u = k * n + j;
                v = (*(pcol + k)) * n + j;
                ftemp = *(fMatrixA + u);
                *(fMatrixA + u) = *(fMatrixA + v);
                *(fMatrixA + v) = ftemp;
            }
        }

        if ((*(prow + k)) != k)
        {
            for (i = 0; i < n; i++)
            {
                u = i * n + k;
                v = i * n + (*(prow + k));
                ftemp = *(fMatrixA + u);
                *(fMatrixA + u) = *(fMatrixA + v);
                *(fMatrixA + v) = ftemp;
            }
        }
    }
    return 1;
}


/****************************************************************************************************
//  @brief      ����UD�ֽ�
//  @param      A                     �������
//  @param      n                     �����ά��
//  @param      U                     U����
//  @param      D                     D����
//  @since
//  Sample usage:
****************************************************************************************************/
void UD(float* A, int n, float* U, float* D)
{
    int i = 0;
    int j = 0;
    int k = 0;
    float sum1 = 0.0;
    float sum2 = 0.0;
    int m = n - 1;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (i == j)
            {
                U[i * n + j] = 1;
                D[i * n + j] = 0;
            }
            else
            {
                D[i * n + j] = 0;
                U[i * n + j] = 0;
            }
        }
    }

    D[m * n + m] = A[m * n + m];

    for (i = 0; i < n; i++)
    {
        U[i * n + m] = A[i * n + m] / D[m * n + m];
    }

    for (j = n - 2; j >= 0; j--)
    {
        for (k = j + 1; k < n; k++)
        {
            sum2 = sum2 + D[k * n + k] * U[j * n + k] * U[j * n + k];
        }

        D[j * n + j] = A[j * n + j] - sum2;
        sum2 = 0;

        for (i = 0; i < j; i++)
        {
            for (k = j + 1; k < n; k++)
            {
                sum1 = sum1 + D[k * n + k] * U[i * n + k] * U[j * n + k];
            }

            U[i * n + j] = (A[i * n + j] - sum1) / D[j * n + j];
            sum1 = 0;
        }
    }
}


/****************************************************************************************************
//  @brief      �������
//  @param      fMatrixA              �������
//  @param      iRow                  ���������
//  @param      iCol                  ���������
//  @return     ����ķ���
//  @since
//  Sample usage:
****************************************************************************************************/
float Norm(float* fMatrixA, int iRow, int iCol)
{
    int i = 0;
    int j = 0;
    int index = 0;
    float local_result = 0.0;

    for (i = 0; i < iRow; i++)
    {
        for (j = 0; j < iCol; j++)
        {
            index = i * iCol + j;
            local_result += (*(fMatrixA + index)) * (*(fMatrixA + index));
        }
    }
    local_result = sqrt(local_result);

    return local_result;
}
