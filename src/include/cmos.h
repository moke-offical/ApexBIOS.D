/*
 *		cmos.h
 *		cmos存储器头文件
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#ifndef INCLUDE_CMOS_H_
#define INCLUDE_CMOS_H_

#include <stdint.h>

#define bcdtobin(x) (x & 0x0F) + ((x / 16) * 10)

/* 读取CMOS存储器 */
uint8_t cmos_get_reg(uint16_t reg);

/* 获取当前的秒数 */
uint8_t cmos_get_second(void);

/* 获取当前的分钟数 */
uint8_t cmos_get_minute(void);

/* 获取当前的小时数 */
uint8_t cmos_get_hour(void);

/* 获取当前的日期 */
uint8_t cmos_get_day(void);

/* 获取当前的月份 */
uint8_t cmos_get_month(void);

/* 获取当前的年份 */
uint8_t cmos_get_year(void);

#endif // INCLUDE_CMOS_H_
