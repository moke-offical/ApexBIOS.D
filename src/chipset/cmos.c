/*
 *		cmos.c
 *		cmos存储器
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#include "cmos.h"
#include "ports.h"

/* 读取CMOS存储器 */
uint8_t cmos_get_reg(uint16_t reg)
{
	outb(0x70, reg);
	return inb(0x71);
}

/* 获取当前的秒数 */
uint8_t cmos_get_second(void)
{
	uint8_t val = cmos_get_reg(0x0);
	if (!(cmos_get_reg(0x0B) & 0x04))
		val = bcdtobin(val);
	return val;
}

/* 获取当前的分钟数 */
uint8_t cmos_get_minute(void)
{
	uint8_t val = cmos_get_reg(0x2);
	if (!(cmos_get_reg(0x0B) & 0x04))
		val = bcdtobin(val);
	return val;
}

/* 获取当前的小时数 */
uint8_t cmos_get_hour(void)
{
	uint8_t val = cmos_get_reg(0x4);
	if (!(cmos_get_reg(0x0B) & 0x04))
		val = bcdtobin(val);
	return val;
}

/* 获取当前的日期 */
uint8_t cmos_get_day(void)
{
	uint8_t val = cmos_get_reg(0x7);
	if (!(cmos_get_reg(0x0B) & 0x04))
		val = bcdtobin(val);
	return val;
}

/* 获取当前的月份 */
uint8_t cmos_get_month(void)
{
	uint8_t val = cmos_get_reg(0x8);
	if (!(cmos_get_reg(0x0B) & 0x04))
		val = bcdtobin(val);
	return val;
}

/* 获取当前的年份 */
uint8_t cmos_get_year(void)
{
	uint8_t val = cmos_get_reg(0x9);
	if (!(cmos_get_reg(0x0B) & 0x04))
		val = bcdtobin(val);
	return val;
}
