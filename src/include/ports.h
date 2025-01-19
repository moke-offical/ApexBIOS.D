/*
 *		ports.h
 *		计算机端口驱动头文件
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#ifndef INCLUDE_PORTS_H_
#define INCLUDE_PORTS_H_

#include <stdint.h>

/* 从指定的16位端口读取8位数据 */
uint8_t inb(uint16_t port);

/* 从指定的16位端口读取16位数据 */
uint16_t inw(uint16_t port);

/* 从指定的16位端口读取32位数据 */
uint32_t inl(uint16_t port);

/* 向指定的16位端口写入8位数据 */
void outb(uint16_t port, uint8_t data);

/* 向指定的16位端口写入16位数据 */
void outw(uint16_t port, uint16_t data);

/* 向指定的16位端口写入32位数据 */
void outl(uint16_t port, uint32_t data);

#endif // INCLUDE_PORTS_H_
