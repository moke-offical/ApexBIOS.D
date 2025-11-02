/*
 *		memory.h
 *		内存管理头文件
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#ifndef INCLUDE_MEMORY_H_
#define INCLUDE_MEMORY_H_

#include <stdint.h>
#include "cmos.h"
#include "serial.h"
#include "speaker.h"

/* 使用CMOS寄存器检测系统内存 */
uint32_t ram_detect(void);

/* 复制内存区域 */
void *memcpy(void *dest, const void *src, uint32_t len);

/* 设置内存区域的值 */
void *memset(void *dest, int val, uint32_t len);

#endif // INCLUDE_MEMORY_H_
