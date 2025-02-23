/*
 * Camera.c
 *
 *  Created on: 2025��2��18��
 *      Author: 20483
 */

#include "zf_common_headfile.h"

uint8 block_size = 5;
float clip_value = 15;




int   LeftLine    [MT9V03X_H][2];
int   RightLine   [MT9V03X_H][2];
float CenterLine  [MT9V03X_H][2];
int   LeftLine_x  [MT9V03X_H];
int   LeftLine_y  [MT9V03X_H];
int   RightLine_x [MT9V03X_H];
int   RightLine_y [MT9V03X_H];
float CenterLine_x[MT9V03X_H];
float CenterLine_y[MT9V03X_H];
int   LeftLineNum;
int   RightLineNum;
int   CenterLineNum;


// ͼ�񱸷����飬�ڷ���ǰ��ͼ�񱸷��ٽ��з��ͣ��������Ա���ͼ�����˺�ѵ�����
uint8 image_copy[MT9V03X_H][MT9V03X_W];




//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ͼƬ(x,y)�ĻҶ�ֵ
//  @param      x                x����
//  @param      y                y����
//  @return     int��            ͼƬ(x,y)�ĻҶ�ֵ
//  @since
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
int AT_IMAGE(int x, int y)
{
    return mt9v03x_image[IntClip(y, 0, MT9V03X_H - 1)][IntClip(x, 0, MT9V03X_W - 1)];
}

uint8_t adaptiveThresholdPoint(int x, int y, float block_size, float clip_value)
{
    // block����Ϊ����
    // ȡ�м����ص�
    int half_block = block_size / 2;
    // ����ֲ���ֵ
    int thres = 0;
    for(int dy = -half_block; dy <= half_block; dy++)
    {
        for(int dx = -half_block; dx <= half_block; dx++)
        {
            thres += AT_IMAGE(x + dx, y + dy);
        }
    }
    thres = thres / (block_size * block_size) - clip_value;
    // ���ж�ֵ��
    return (AT_IMAGE(x, y) > thres ? IMG_WHITE  : IMG_BLACK);
}


void Process_Image()
{
    int x1 = MT9V03X_W / 2, y1 = MT9V03X_W - 10;
    LeftLineNum = sizeof(LeftLine) / sizeof(LeftLine[0]);
    for(; x1 > (block_size / 2 + 1); x1--)
    {
        if(adaptiveThresholdPoint(x1 - 1, y1, block_size, clip_value) == IMG_BLACK)
        {
            break;
        }
    }
    if(x1 == (block_size / 2 + 1))
    {
        for(; y1 > (block_size / 2 + 1); y1--)
        {
            if(adaptiveThresholdPoint(x1, y1 - 1, block_size, clip_value) == IMG_BLACK) break; // �����û���ҵ��������Ϸ�Ѱ��
        }
    }
    if(adaptiveThresholdPoint(x1, y1, block_size, clip_value) == IMG_WHITE)
    {
        findline_lefthand_adaptive(x1, y1, block_size, clip_value, LeftLine, &LeftLineNum);
    }
    else
    {
        LeftLineNum = 0;
    }
    for(int i = 0; i < LeftLineNum; i++)
    {
        LeftLine_x[i] = LeftLine[i][0];
        LeftLine_y[i] = LeftLine[i][1];
    }


    int x2 = MT9V03X_W / 2, y2 = MT9V03X_W - 10;
    RightLineNum = sizeof(RightLine) / sizeof(RightLine[0]);
    for(; x2 < MT9V03X_W - (block_size / 2 + 1) - 1; x2++)
    {
        if(adaptiveThresholdPoint(x2 + 1, y2, block_size, clip_value) == IMG_BLACK)
        {
            break;
        }
    }
    if(x2 == MT9V03X_W - (block_size / 2 + 1) - 1)
    {
        for(; y2 > (block_size / 2 + 1); y2--)
        {
            if(adaptiveThresholdPoint(x2, y2 - 1, block_size, clip_value) == IMG_BLACK) break;  //�ұ���û���ҵ��������Ϸ�Ѱ��
        }
    }
    if(adaptiveThresholdPoint(x2, y2, block_size, clip_value) == IMG_WHITE)
    {
        findline_righthand_adaptive(x2, y2, block_size, clip_value, RightLine, &RightLineNum);
    }
    else
    {
        RightLineNum = 0;
    }
    for(int i = 0; i < LeftLineNum; i++)
    {
        RightLine_x[i] = RightLine[i][0];
        RightLine_y[i] = RightLine[i][1];
    }

}

float CalculateAngleError(int CenterLine[MT9V03X_H][2])
{
    float sum_x = 0.0f, sum_y = 0.0f, sum_xy = 0.0f, sum_yy = 0.0f;
    const int N = MT9V03X_H;

    // ��С���˷�����ֱ�߲���
    for (int i = 0; i < N; i++) {
        float y = (float)i;          // ����ͷ����ϵ��yֵ�����µ�����
        float x = (float)CenterLine[i][0] - (MT9V03X_W / 2.0f); // ���Ļ�x����

        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_yy += y * y;
    }

    // ����ֱ��б��
    float denominator = N * sum_yy - sum_y * sum_y;
    if (fabsf(denominator) < 1e-6f) return 0.0f; // ��ֹ������

    float k = (N * sum_xy - sum_x * sum_y) / denominator;

    // ����Ƕ�������͸�ӱ任��
    // ����ˮƽ�ӳ���Ϊ60�ȣ������ʵ������ͷ����������
    const float FOV_DEG = 130.0f;
    float angle_error = atanf(k) * (180.0f / PI) * (FOV_DEG / MT9V03X_W);

    return angle_error;
}

const int dir_front[4][2]      = {{0,  -1},  {1,  0},  {0,  1},  {-1,  0}};
const int dir_frontleft[4][2]  = {{-1, -1},  {1, -1},  {1,  1},  {-1,  1}};
const int dir_frontright[4][2] = {{1,  -1},  {1,  1},  {-1, 1},  {-1, -1}};


//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����Թ�Ѳ��
//  @param      x                     ��ʼ�������
//  @param      y                     ��ʼ��������
//  @param      block_size            Ѳ�������С
//  @param      clip_value            ��ֵ����ֵ
//  @param      pts[][2]              ���������
//  @param      *num                  ����������С
//  @return     void
//  @since
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void findline_lefthand_adaptive(int x, int y, int block_size, int clip_value, int pts[][2], int *num) {

    // ����ֲ���ֵ
    int half = block_size / 2;
    int step = 0, dir = 0, turn = 0;

    while (step < *num && half < x && x <  MT9V03X_W - half - 1 && half < y && y < MT9V03X_H - half - 1 && turn < 4) {
        // ����ֲ���ֵ
        int local_thres = 0;
        for (int dy = -half; dy <= half; dy++) {
            for (int dx = -half; dx <= half; dx++) {
                local_thres += AT_IMAGE(x + dx, y + dy);
            }
        }
        local_thres /= (block_size * block_size);
        local_thres -= clip_value;

        // int current_value = AT_IMAGE(x, y);
        int front_value = AT_IMAGE(x + dir_front[dir][0], y + dir_front[dir][1]);
        int frontleft_value = AT_IMAGE(x + dir_frontleft[dir][0], y + dir_frontleft[dir][1]);
        if (front_value < local_thres) {
            dir = (dir + 1) % 4;
            turn++;
        } else if (frontleft_value < local_thres) {
            x += dir_front[dir][0];
            y += dir_front[dir][1];
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
        } else {
            x += dir_frontleft[dir][0];
            y += dir_frontleft[dir][1];
            dir = (dir + 3) % 4;
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
        }
    }
    *num = step;

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����Թ�Ѳ��
//  @param      x                     ��ʼ�������
//  @param      y                     ��ʼ��������
//  @param      block_size            Ѳ�������С
//  @param      clip_value            ��ֵ����ֵ
//  @param      pts[][2]              �ұ�������
//  @param      *num                  �ұ��������С
//  @return     void
//  @since
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void findline_righthand_adaptive(int x, int y, int block_size, int clip_value, int pts[][2], int *num) {

    // ����ֲ���ֵ
    int half = block_size / 2;
    int step = 0, dir = 0, turn = 0;

    while (step < *num && half < x && x < MT9V03X_W - half - 1 && half < y && y < MT9V03X_H - half - 1 && turn < 4) {
        // ����ֲ���ֵ
        int local_thres = 0;
        for (int dy = -half; dy <= half; dy++) {
            for (int dx = -half; dx <= half; dx++) {
                local_thres += AT_IMAGE(x + dx, y + dy);
            }
        }
        local_thres /= (block_size * block_size);
        local_thres -= clip_value;

        // int current_value = AT_IMAGE(x, y);
        int front_value = AT_IMAGE(x + dir_front[dir][0], y + dir_front[dir][1]);
        int frontright_value = AT_IMAGE(x + dir_frontright[dir][0], y + dir_frontright[dir][1]);
        if (front_value < local_thres) {
            dir = (dir + 3) % 4;
            turn++;
        } else if (frontright_value < local_thres) {
            x += dir_front[dir][0];
            y += dir_front[dir][1];
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
        } else {
            x += dir_frontright[dir][0];
            y += dir_frontright[dir][1];
            dir = (dir + 1) % 4;
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
        }
    }
    *num = step;

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡƫ�������
//  @param      pts_in[][2]            ��������
//  @param      num1                   ���������С
//  @param      angle_out[]            ƫ�������
//  @param      *num2                  ƫ��������С
//  @return     void
//  @since
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void get_error(float pts_in[][2],int num1, float angle_out[])
{
    // ��ȡ�������
    float begin_x = pts_in[0][0];
    float begin_y = pts_in[0][1];
    // ����ƫ���
    for (int i = 0; i < num1; i++)
    {
        if (pts_in[i][0] != 0 && pts_in[i][1] != 0)
        {
            float dx = pts_in[i][0] - begin_x;
            float dy = pts_in[i][1] - begin_y;
            angle_out[i] = atan2(dx, -dy) * 180 / PI;
        }
        else
        {
            angle_out[i] = 0;
        }
    }

}
