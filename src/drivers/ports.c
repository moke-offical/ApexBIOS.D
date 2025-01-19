/*
 *		ports.c
 *		计算机端口驱动
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#include "ports.h"

/* 从指定的16位端口读取8位数据 */
uint8_t inb(uint16_t port)
{
	uint8_t ret;
	__asm__ volatile("in %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

/* 从指定的16位端口读取16位数据 */
uint16_t inw(uint16_t port)
{
	uint16_t ret;
	__asm__ volatile("in %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

/* 从指定的16位端口读取32位数据 */
uint32_t inl(uint16_t port)
{
	uint32_t ret;
	__asm__ volatile("in %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

/* 向指定的16位端口写入8位数据 */
void outb(uint16_t port, uint8_t data)
{
	__asm__ volatile("out %0, %1" :: "a"(data), "Nd"(port));
}

/* 向指定的16位端口写入16位数据 */
void outw(uint16_t port, uint16_t data)
{
	__asm__ volatile("out %0, %1" :: "a"(data), "Nd"(port));
}

/* 向指定的16位端口写入32位数据 */
void outl(uint16_t port, uint32_t data)
{
	__asm__ volatile("out %0, %1" :: "a"(data), "Nd"(port));
}
