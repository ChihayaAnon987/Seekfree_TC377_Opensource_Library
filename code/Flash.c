/*
 * Flash.c
 *
 *  Created on: 2025��1��9��
 *      Author: 20483
 */

#include "zf_common_headfile.h"
double_to_uint32_union lat_union[NUM_GPS_DATA];
double_to_uint32_union lon_union[NUM_GPS_DATA];

// �洢���ݵ�Flash
void FLASH_SAV_GPS()
{
    flash_buffer_clear();
    if(Point_NUM != 0)
    {
        flash_union_buffer[0].uint32_type = Point_NUM;
        if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX))                      // �ж��Ƿ�������
        {
            flash_erase_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);                // ������һҳ
        }
        for (int i = 0; i < Point_NUM; i++)
        {
            flash_union_buffer[i * 4 + 1].uint32_type = lat_union[i].uint32_type[0];  // γ�ȸ� 32 λ
            flash_union_buffer[i * 4 + 2].uint32_type = lat_union[i].uint32_type[1];  // γ�ȵ� 32 λ

            flash_union_buffer[i * 4 + 3].uint32_type = lon_union[i].uint32_type[0];  // ���ȸ� 32 λ
            flash_union_buffer[i * 4 + 4].uint32_type = lon_union[i].uint32_type[1];  // ���ȵ� 32 λ
        }
        // д�� Flash ҳ��
        flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);
        flash_buffer_clear();

        LED_Buzzer_Flag_Ctrl(LED1);
    }
}

// �� Flash ��ȡ���ݵ�����
void FLASH_GET_GPS()
{
    flash_buffer_clear();                                                       // ȷ���������ɾ�
    if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX))                      // �ж��Ƿ�������
    {
        flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);
        Point_NUM = flash_union_buffer[0].uint32_type;
        for(int i = 0; i < Point_NUM; i++)
        {
            if(flash_union_buffer[i * 4 + 1].uint32_type != 0)
            {
                if(flash_union_buffer[i * 4 + 2].uint32_type != 0)
                {
                    lat_union[i].uint32_type[0] = flash_union_buffer[i * 4 + 1].uint32_type;  // γ�ȸ� 32 λ
                    lat_union[i].uint32_type[1] = flash_union_buffer[i * 4 + 2].uint32_type;  // γ�ȵ� 32 λ
                    GPS_GET_LAT[i] = lat_union[i].double_type;                                // γ��
                }
            }
            if(flash_union_buffer[i * 4 + 3].uint32_type != 0)
            {
                if(flash_union_buffer[i * 4 + 4].uint32_type != 0)
                {
                    lon_union[i].uint32_type[0] = flash_union_buffer[i * 4 + 3].uint32_type;  // ���ȸ� 32 λ
                    lon_union[i].uint32_type[1] = flash_union_buffer[i * 4 + 4].uint32_type;  // ���ȵ� 32 λ
                    GPS_GET_LOT[i] = lon_union[i].double_type;                                // ����
                }
            }
        }
        flash_buffer_clear();                                                    // ��ջ�����

        LED_Buzzer_Flag_Ctrl(LED1);
    }
    else
    {
        LED_Buzzer_Flag_Ctrl(LED2);
    }
}

// ���� Gps ����
void FLASH_FIX_GPS()
{
    flash_buffer_clear();
    if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX))                      // �ж��Ƿ�������
    {
        flash_erase_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);                // ������һҳ
    }

    for(int i = 0; i < Point_NUM; i++)
    {
        lat_union[i].double_type = GPS_GET_LAT[i];
        lon_union[i].double_type = GPS_GET_LOT[i];
    }
    for (int i = 0; i < Point_NUM; i++)
    {
        flash_union_buffer[i * 4 + 1].uint32_type = lat_union[i].uint32_type[0];  // γ�ȸ� 32 λ
        flash_union_buffer[i * 4 + 2].uint32_type = lat_union[i].uint32_type[1];  // γ�ȵ� 32 λ

        flash_union_buffer[i * 4 + 3].uint32_type = lon_union[i].uint32_type[0];  // ���ȸ� 32 λ
        flash_union_buffer[i * 4 + 4].uint32_type = lon_union[i].uint32_type[1];  // ���ȵ� 32 λ
    }
    // д�� Flash ҳ��
    flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);
    flash_buffer_clear();

    LED_Buzzer_Flag_Ctrl(LED1);
}

// ���Flash����
void FLASH_DEL_GPS()
{
    if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX))                      // �ж��Ƿ�������
    {
        flash_erase_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);
        gpio_set_level(LED1, 0);                                                    //ָʾ������������ɾ��
        gpio_set_level(LED2, 0);
        gpio_set_level(LED3, 0);                                                    //ָʾ������������ɾ��
        gpio_set_level(LED4, 0);
        system_delay_ms(500);
        gpio_set_level(LED1, 1);
        gpio_set_level(LED2, 1);
        gpio_set_level(LED3, 1);
        gpio_set_level(LED4, 1);
    }

}


void FLASH_SAV_PAR()
{
    flash_buffer_clear();
    if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX - 2))                      // �ж��Ƿ�������
    {
        flash_erase_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX - 2);                // ������һҳ
    }
    // ���PID
    flash_union_buffer[0].float_type = Parameter_set0.ServePID[0];
    flash_union_buffer[1].float_type = Parameter_set0.ServePID[1];
    flash_union_buffer[2].float_type = Parameter_set0.ServePID[2];

    //���PID
    flash_union_buffer[3].float_type = Parameter_set0.SpeedPID[0];
    flash_union_buffer[4].float_type = Parameter_set0.SpeedPID[1];
    flash_union_buffer[5].float_type = Parameter_set0.SpeedPID[2];
    // �����ٶȺͻ������
    flash_union_buffer[6].int32_type = Parameter_set0.Speed_Duty;
    flash_union_buffer[7].float_type = Parameter_set0.Distance;
    // �����
    flash_union_buffer[8].int8_type  = Task1_Points;
    flash_union_buffer[9].int8_type  = Task2_Points;
    flash_union_buffer[10].int8_type = Task3_Points;

    // ϵ��
    flash_union_buffer[11].int16_type = Fly_Slope_Alpha;
    flash_union_buffer[12].float_type = K_Straight;

    flash_union_buffer[13].float_type = From_0000_To_2000_ServoPD.Kp;
    flash_union_buffer[14].float_type = From_0000_To_2000_ServoPD.Kd;
    flash_union_buffer[15].float_type = From_2000_To_4000_ServoPD.Kp;
    flash_union_buffer[16].float_type = From_2000_To_4000_ServoPD.Kd;
    flash_union_buffer[17].float_type = From_4000_To_5000_ServoPD.Kp;
    flash_union_buffer[18].float_type = From_4000_To_5000_ServoPD.Kd;
    flash_union_buffer[19].float_type = From_5000_To_6000_ServoPD.Kp;
    flash_union_buffer[20].float_type = From_5000_To_6000_ServoPD.Kd;
    flash_union_buffer[21].float_type = From_6000_To_7000_ServoPD.Kp;
    flash_union_buffer[22].float_type = From_6000_To_7000_ServoPD.Kd;
    flash_union_buffer[23].float_type = From_7000_To_8000_ServoPD.Kp;
    flash_union_buffer[24].float_type = From_7000_To_8000_ServoPD.Kd;
    flash_union_buffer[25].float_type = From_8000_To_9000_ServoPD.Kp;
    flash_union_buffer[26].float_type = From_8000_To_9000_ServoPD.Kd;
    flash_union_buffer[27].float_type = From_9000_To_9900_ServoPD.Kp;
    flash_union_buffer[28].float_type = From_9000_To_9900_ServoPD.Kd;
    
    // ���������ٶ�����
    for(int i = 100; i < 100 + NUM_GPS_DATA; i++)
    {
        flash_union_buffer[i].float_type = GpsDistance[i - 100];
    }
    for(int i = 100 + NUM_GPS_DATA; i < 100 + 2 * NUM_GPS_DATA; i++)
    {
        flash_union_buffer[i].int16_type = GpsTgtEncod[i - 100 - NUM_GPS_DATA];
    }
    // ָʾ�����������Ѷ�ȡ
    LED_Buzzer_Flag_Ctrl(LED1);


    // д�� Flash ҳ��
    flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX - 2);
    flash_buffer_clear();
}

void FLASH_GET_PAR()
{
    flash_buffer_clear();                                                       // ȷ���������ɾ�
    if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX - 2))                      // �ж��Ƿ�������
    {
        flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX - 2);
        // �ٶ�PID
        Parameter_set0.ServePID[0] = flash_union_buffer[0].float_type;
        Parameter_set0.ServePID[1] = flash_union_buffer[1].float_type;
        Parameter_set0.ServePID[2] = flash_union_buffer[2].float_type;

        // ���PID
        Parameter_set0.SpeedPID[0] = flash_union_buffer[3].float_type;
        Parameter_set0.SpeedPID[1] = flash_union_buffer[4].float_type;
        Parameter_set0.SpeedPID[2] = flash_union_buffer[5].float_type;
        // �����ٶȺͻ������
        Parameter_set0.Speed_Duty = flash_union_buffer[6].int32_type;
        Parameter_set0.Distance   = flash_union_buffer[7].float_type;
        // �����
        Task1_Points = flash_union_buffer[8].int8_type ;
        Task2_Points = flash_union_buffer[9].int8_type ;
        Task3_Points = flash_union_buffer[10].int8_type;

        // ϵ��
        Fly_Slope_Alpha = flash_union_buffer[11].int16_type;
        K_Straight      = flash_union_buffer[12].float_type;

        From_0000_To_2000_ServoPD.Kp = flash_union_buffer[13].float_type;
        From_0000_To_2000_ServoPD.Kd = flash_union_buffer[14].float_type;
        From_2000_To_4000_ServoPD.Kp = flash_union_buffer[15].float_type;
        From_2000_To_4000_ServoPD.Kd = flash_union_buffer[16].float_type;
        From_4000_To_5000_ServoPD.Kp = flash_union_buffer[17].float_type;
        From_4000_To_5000_ServoPD.Kd = flash_union_buffer[18].float_type;
        From_5000_To_6000_ServoPD.Kp = flash_union_buffer[19].float_type;
        From_5000_To_6000_ServoPD.Kd = flash_union_buffer[20].float_type;
        From_6000_To_7000_ServoPD.Kp = flash_union_buffer[21].float_type;
        From_6000_To_7000_ServoPD.Kd = flash_union_buffer[22].float_type;
        From_7000_To_8000_ServoPD.Kp = flash_union_buffer[23].float_type;
        From_7000_To_8000_ServoPD.Kd = flash_union_buffer[24].float_type;
        From_8000_To_9000_ServoPD.Kp = flash_union_buffer[25].float_type;
        From_8000_To_9000_ServoPD.Kd = flash_union_buffer[26].float_type;
        From_9000_To_9900_ServoPD.Kp = flash_union_buffer[27].float_type;
        From_9000_To_9900_ServoPD.Kd = flash_union_buffer[28].float_type;

        // ���������ٶ�����
        for(int i = 100; i < 100 + NUM_GPS_DATA; i++)
        {
            GpsDistance[i - 100] = flash_union_buffer[i].float_type;
        }
        for(int i = 100 + NUM_GPS_DATA; i < 100 + 2 * NUM_GPS_DATA; i++)
        {
            GpsTgtEncod[i - 100 - NUM_GPS_DATA] = flash_union_buffer[i].int16_type;
        }

        flash_buffer_clear();                                                    // ��ջ�����
    }

}
 