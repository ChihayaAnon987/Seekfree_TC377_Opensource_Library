/*
 * Flash.h
 *
 *  Created on: 2025年1月9日
 *      Author: 20483
 */

#ifndef CODE_FLASH_H_
#define CODE_FLASH_H_

//===================================================宏定义BEG===================================================
#define FLASH_SECTION_INDEX       (0)                        // 存储数据用的扇区
#define FLASH_PAGE_INDEX          (11)                       // 存储数据用的页码 倒数第一个页码
//===================================================宏定义END===================================================


//===================================================类型定义BEG===================================================
typedef union                                                  // 定义 union，拆分 double 为两个 uint32_t 类型
{
    double double_type;                                        // 原始 double 数据
    uint32_t uint32_type[2];                                   // 用两个 uint32_t 存储 double 数据的高低 32 位
}double_to_uint32_union;
//===================================================类型定义END===================================================


//===================================================全局变量BEG===================================================
extern double_to_uint32_union lat_union[NUM_GPS_DATA];         // 存储纬度数据的数组
extern double_to_uint32_union lon_union[NUM_GPS_DATA];         // 存储经度数据的数组
//===================================================全局变量END===================================================


//===================================================函数声明BEG===================================================
void FLASH_SAV_GPS(void);                                      // 存储数据到 Flash
void FLASH_GET_GPS(void);                                      // 从 Flash 读数据
void FLASH_FIX_GPS(void);                                      // 修正 Gps 数据
void FLASH_DEL_GPS(void);                                      // 清除 Flash 数据
void FLASH_SAV_PAR(void);                                      // 存储 PID 参数到 Flash
void FLASH_GET_PAR(void);                                      // 从 Flash 读取 PID 参数
//===================================================函数声明END===================================================

#endif /* CODE_FLASH_H_ */
