/*********************************************************************************************************************
* TC377 Opensourec Library ����TC377 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC377 ��Դ���һ����
*
* TC377 ��Դ�� ���������
* �����Ը���������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù�������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ����֤Э�� ������������Ϊ���İ汾
* ��������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ����֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          zf_common_font
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.9.20
* ����ƽ̨          TC377TP
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-11-03       pudding            first version
********************************************************************************************************************/

#ifndef _zf_common_font_h
#define _zf_common_font_h

#include "zf_common_typedef.h"

//-------������ɫ----------
typedef enum
{
    RGB565_WHITE    = (0xFFFF),                                                     // ��ɫ
    RGB565_BLACK    = (0x0000),                                                     // ��ɫ
    RGB565_BLUE     = (0x001F),                                                     // ��ɫ
    RGB565_PURPLE   = (0xF81F),                                                     // ��ɫ
    RGB565_PINK     = (0xFE19),                                                     // ��ɫ
    RGB565_RED      = (0xF800),                                                     // ��ɫ
    RGB565_MAGENTA  = (0xF81F),                                                     // Ʒ��
    RGB565_GREEN    = (0x07E0),                                                     // ��ɫ
    RGB565_CYAN     = (0x07FF),                                                     // ��ɫ
    RGB565_YELLOW   = (0xFFE0),                                                     // ��ɫ
    RGB565_BROWN    = (0xBC40),                                                     // ��ɫ
    RGB565_GRAY     = (0x8430),                                                     // ��ɫ

    RGB565_39C5BB   = (0x3616),
    RGB565_66CCFF   = (0x665F),
}rgb565_color_enum;

extern const uint8      ascii_font_8x16[][16];
extern const uint8      ascii_font_6x8[][6];
extern const uint8      chinese_test[8][16];
extern const uint8      oled_16x16_chinese[][16];
extern const uint8      gImage_seekfree_logo[38400];
extern const uint8      my_picture[64800];


#endif