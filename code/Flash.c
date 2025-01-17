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
        flash_union_buffer[410].uint32_type = Point_NUM;
        if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX))                      // �ж��Ƿ�������
        {
            flash_erase_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);                // ������һҳ
        }
        for (int i = 0; i < Point_NUM; i++)
        {
            flash_union_buffer[i * 4 + 0].uint32_type = lat_union[i].uint32_type[0];  // γ�ȸ� 32 λ
            flash_union_buffer[i * 4 + 1].uint32_type = lat_union[i].uint32_type[1];  // γ�ȵ� 32 λ

            flash_union_buffer[i * 4 + 2].uint32_type = lon_union[i].uint32_type[0];  // ���ȸ� 32 λ
            flash_union_buffer[i * 4 + 3].uint32_type = lon_union[i].uint32_type[1];  // ���ȵ� 32 λ
        }
        // д�� Flash ҳ��
        flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);

        gpio_set_level(LED3, 0);                     //ָʾ�����������Ѵ洢
        system_delay_ms(1000);
        gpio_set_level(LED3, 1);
    }
    else
    {
        gpio_set_level(LED4, 0);                                                 //ָʾ����������û�е�
        system_delay_ms(1000);
        gpio_set_level(LED4, 1);
    }
    flash_buffer_clear();
    if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX - 1))                      // �ж��Ƿ�������
    {
        flash_erase_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX - 1);                // ������һҳ
    }
    flash_union_buffer[0].float_type = Lat_Fix;
    flash_union_buffer[1].float_type = Lon_Fix;
    flash_union_buffer[2].float_type = K_Gps;



    flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX - 1);
}

// �� Flash ��ȡ���ݵ�����
void FLASH_GET_GPS()
{
    flash_buffer_clear();                                                       // ȷ���������ɾ�
    if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX))                      // �ж��Ƿ�������
    {
        flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);
        Point_NUM = flash_union_buffer[410].uint32_type;
        for(int i = 0; i < Point_NUM; i++)
        {
            if(flash_union_buffer[i * 4 + 0].uint32_type != 0)
            {
                if(flash_union_buffer[i * 4 + 1].uint32_type != 0)
                {
                    lat_union[i].uint32_type[0] = flash_union_buffer[i * 4 + 0].uint32_type;  // γ�ȸ� 32 λ
                    lat_union[i].uint32_type[1] = flash_union_buffer[i * 4 + 1].uint32_type;  // γ�ȵ� 32 λ
                    GPS_GET_LAT[i] = lat_union[i].double_type;                                // γ��
                }
            }
            if(flash_union_buffer[i * 4 + 2].uint32_type != 0)
            {
                if(flash_union_buffer[i * 4 + 3].uint32_type != 0)
                {
                    lon_union[i].uint32_type[0] = flash_union_buffer[i * 4 + 2].uint32_type;  // ���ȸ� 32 λ
                    lon_union[i].uint32_type[1] = flash_union_buffer[i * 4 + 3].uint32_type;  // ���ȵ� 32 λ
                    GPS_GET_LOT[i] = lon_union[i].double_type;                                // ����
                }
            }
        }
        flash_buffer_clear();                                                    // ��ջ�����
        
        gpio_set_level(LED1, 0);                                                 //ָʾ�����������Ѷ�ȡ
        system_delay_ms(1000);
        gpio_set_level(LED1, 1);
    }
    else
    {
        gpio_set_level(LED2, 0);                                                 //ָʾ����������Flash��������
        system_delay_ms(1000);
        gpio_set_level(LED2, 1);

    }
    flash_buffer_clear();                                                           // ��ջ�����
    if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX - 1))                      // �ж��Ƿ�������
    {
        flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX - 1);
        Lat_Fix = flash_union_buffer[0].float_type;
        Lon_Fix = flash_union_buffer[1].float_type;
        K_Gps   = flash_union_buffer[2].float_type;


    }
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
        system_delay_ms(1000);
        gpio_set_level(LED1, 1);
        gpio_set_level(LED2, 1);
        gpio_set_level(LED3, 1);
        gpio_set_level(LED4, 1);
    }

}
