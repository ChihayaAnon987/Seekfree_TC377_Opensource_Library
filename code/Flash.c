/*
 * Flash.c
 *
 *  Created on: 2025年1月9日
 *      Author: 20483
 */

#include "zf_common_headfile.h"

double_to_uint32_union lat_union[NUM_GPS_DATA];
double_to_uint32_union lon_union[NUM_GPS_DATA];

// 存储数据到Flash
void FLASH_SAV_GPS()
{
    flash_buffer_clear();
    if(Point_NUM != 0)
    {
        flash_union_buffer[410].uint32_type = Point_NUM;
        if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX))                      // 判断是否有数据
        {
            flash_erase_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);                // 擦除这一页
        }
        for (int i = 0; i < Point_NUM; i++)
        {
            flash_union_buffer[i * 4 + 0].uint32_type = lat_union[i].uint32_type[0];  // 纬度高 32 位
            flash_union_buffer[i * 4 + 1].uint32_type = lat_union[i].uint32_type[1];  // 纬度低 32 位

            flash_union_buffer[i * 4 + 2].uint32_type = lon_union[i].uint32_type[0];  // 经度高 32 位
            flash_union_buffer[i * 4 + 3].uint32_type = lon_union[i].uint32_type[1];  // 经度低 32 位
        }
        // 写入 Flash 页面
        flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);

        gpio_set_level(LED3, 0);                     //指示灯亮，表明已存储
        system_delay_ms(1000);
        gpio_set_level(LED3, 1);
    }
    else
    {
        gpio_set_level(LED4, 0);                                                 //指示灯亮，表明没有点
        system_delay_ms(1000);
        gpio_set_level(LED4, 1);
    }
    flash_buffer_clear();
    if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX - 1))                      // 判断是否有数据
    {
        flash_erase_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX - 1);                // 擦除这一页
    }
    flash_union_buffer[0].float_type = Lat_Fix;
    flash_union_buffer[1].float_type = Lon_Fix;
    flash_union_buffer[2].float_type = K_Gps;



    flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX - 1);
}

// 从 Flash 读取数据到数组
void FLASH_GET_GPS()
{
    flash_buffer_clear();                                                       // 确保缓冲区干净
    if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX))                      // 判断是否有数据
    {
        flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);
        Point_NUM = flash_union_buffer[410].uint32_type;
        for(int i = 0; i < Point_NUM; i++)
        {
            if(flash_union_buffer[i * 4 + 0].uint32_type != 0)
            {
                if(flash_union_buffer[i * 4 + 1].uint32_type != 0)
                {
                    lat_union[i].uint32_type[0] = flash_union_buffer[i * 4 + 0].uint32_type;  // 纬度高 32 位
                    lat_union[i].uint32_type[1] = flash_union_buffer[i * 4 + 1].uint32_type;  // 纬度低 32 位
                    GPS_GET_LAT[i] = lat_union[i].double_type;                                // 纬度
                }
            }
            if(flash_union_buffer[i * 4 + 2].uint32_type != 0)
            {
                if(flash_union_buffer[i * 4 + 3].uint32_type != 0)
                {
                    lon_union[i].uint32_type[0] = flash_union_buffer[i * 4 + 2].uint32_type;  // 经度高 32 位
                    lon_union[i].uint32_type[1] = flash_union_buffer[i * 4 + 3].uint32_type;  // 经度低 32 位
                    GPS_GET_LOT[i] = lon_union[i].double_type;                                // 经度
                }
            }
        }
        flash_buffer_clear();                                                    // 清空缓冲区
        
        gpio_set_level(LED1, 0);                                                 //指示灯亮，表明已读取
        system_delay_ms(1000);
        gpio_set_level(LED1, 1);
    }
    else
    {
        gpio_set_level(LED2, 0);                                                 //指示灯亮，表明Flash中无数据
        system_delay_ms(1000);
        gpio_set_level(LED2, 1);

    }
    flash_buffer_clear();                                                           // 清空缓冲区
    if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX - 1))                      // 判断是否有数据
    {
        flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX - 1);
        Lat_Fix = flash_union_buffer[0].float_type;
        Lon_Fix = flash_union_buffer[1].float_type;
        K_Gps   = flash_union_buffer[2].float_type;


    }
}

// 清除Flash数据
void FLASH_DEL_GPS()
{
    if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX))                      // 判断是否有数据
    {
        flash_erase_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);
        gpio_set_level(LED1, 0);                                                    //指示灯亮，表明已删除
        gpio_set_level(LED2, 0);
        gpio_set_level(LED3, 0);                                                    //指示灯亮，表明已删除
        gpio_set_level(LED4, 0);
        system_delay_ms(1000);
        gpio_set_level(LED1, 1);
        gpio_set_level(LED2, 1);
        gpio_set_level(LED3, 1);
        gpio_set_level(LED4, 1);
    }

}
