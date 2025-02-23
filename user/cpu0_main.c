/*********************************************************************************************************************
* TC377 Opensourec Library ����TC377 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC377 ��Դ���һ����
*
* TC377 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
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
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          cpu0_main
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
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��

// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������

// **************************** �������� ****************************







int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�� ������ ���������ʼ�������

    CPU0_Init();                     // ���г�ʼ��
    FLASH_GET_GPS();                 // �� Flash ��ȡ GPS ����
    FLASH_GET_PAR();                 // �� Flash ��ȡ����


    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        if(Start_Flag == 1)
        {
            break;
        }
        else
        {
            Start_Lat = gnss.latitude;
            Start_Lon = gnss.longitude;
            if(Task_Flag == 1)
            {
                Delta_Lat = gnss.latitude - GPS_GET_LAT[Task1_Start_Point];
                Delta_Lon = gnss.longitude - GPS_GET_LOT[Task1_Start_Point];
            }
            if(Task_Flag == 2)
            {
                Delta_Lat = gnss.latitude - GPS_GET_LAT[Task2_Start_Point];
                Delta_Lon = gnss.longitude - GPS_GET_LOT[Task2_Start_Point];
            }
            if(Task_Flag == 3)
            {
                Delta_Lat = gnss.latitude - GPS_GET_LAT[Task3_Start_Point];
                Delta_Lon = gnss.longitude - GPS_GET_LOT[Task3_Start_Point];
            }
        }
    }

    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
        if(Control_Flag == 0)
        {
            Track_Follow();
        }
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

#pragma section all restore
// **************************** �������� ****************************

