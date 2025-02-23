/*
 * Camera.h
 *
 *  Created on: 2025��2��18��
 *      Author: 20483
 */

#ifndef CODE_CAMERA_H_
#define CODE_CAMERA_H_


//===================================================�궨��BEG===================================================
#define IMG_BLACK               (      0       )              // ��ɫ
#define IMG_WHITE               (     255      )              // ��ɫ
#define BOUNDARY_NUM            (MT9V03X_H * 3 / 2)           // �������
//===================================================�궨��END===================================================


//===================================================ȫ�ֱ���BEG===================================================
extern uint8 block_size;
extern float clip_value;
extern uint8 image_copy[MT9V03X_H][MT9V03X_W];
extern int   LeftLine    [MT9V03X_H][2];
extern int   RightLine   [MT9V03X_H][2];
extern float CenterLine  [MT9V03X_H][2];
extern int   LeftLine_x  [MT9V03X_H];
extern int   LeftLine_y  [MT9V03X_H];
extern int   RightLine_x [MT9V03X_H];
extern int   RightLine_y [MT9V03X_H];
extern float CenterLine_x[MT9V03X_H];
extern float CenterLine_y[MT9V03X_H];
extern int   LeftLineNum;
extern int   RightLineNum;
extern int   CenterLineNum;
//===================================================ȫ�ֱ���END===================================================


//===================================================��������BEG===================================================
int     AT_IMAGE(int x, int y);
uint8_t adaptiveThresholdPoint(int x, int y, float block_size, float clip_value);
void    Process_Image(void);
float   CalculateAngleError(int CenterLine[MT9V03X_H][2]);
void    findline_lefthand_adaptive(int x, int y, int block_size, int clip_value, int pts[][2], int *num);
void    findline_righthand_adaptive(int x, int y, int block_size, int clip_value, int pts[][2], int *num);
//===================================================��������END===================================================



#endif /* CODE_CAMERA_H_ */
