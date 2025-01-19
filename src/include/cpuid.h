/*
 *		cpuid.h
 *		cpuid相关操作头文件
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#ifndef INCLUDE_CPUID_H_
#define INCLUDE_CPUID_H_

#include <stdint.h>

#define CPUIDBrandVar 0x1060

/* 获取CPUID */
void cpuid(uint32_t reg, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx);

/* 获取CPU品牌名称 */
char *cpu_get_brand(void);

/* 检测CPU是否存在 */
int cpu_detect(void);

/* 检测CPU是否支持64位 */
int cpu_is_64b(void);

#endif // INCLUDE_CPUID_H_
