/*
 *		memory.c
 *		内存管理
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#include "memory.h"

/* 使用CMOS寄存器检测系统内存 */
uint32_t ram_detect(void)
{
	uint32_t low =  (((uint16_t) cmos_get_reg(0x16) << 8) | cmos_get_reg(0x15));
	uint32_t ext =  (((uint16_t) cmos_get_reg(0x18) << 8) | cmos_get_reg(0x17));
	uint32_t xext = (((((uint16_t) cmos_get_reg(0x35) << 8) | cmos_get_reg(0x34)) * 65536)/1024);

	if (xext == 0) {
		if(ext != 0 && ext > 1)
			return ext+1024;
		__asm__ volatile ("hlt");
	} else {
		return xext+16384;
	}  
}

/* 复制内存区域 */
void *memcpy(void *dest, const void *src, uint32_t len)
{
	char *d = dest;
	const char *s = src;
	while (len--) *d++ = *s++;
	return dest;
}

/* 设置内存区域的值 */
void *memset(void *dest, int val, uint32_t len)
{
	while (len-- > 0) *((uint8_t*)dest++) = val;
	return dest;
}
