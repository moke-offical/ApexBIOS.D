/*
 *		cpuid.c
 *		cpuid相关操作
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#include "cpuid.h"

/* 获取CPUID */
void cpuid(uint32_t reg, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx)
{
	__asm__ volatile("cpuid" : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx) : "0" (reg));
}

/* 获取CPU品牌名称 */
char *cpu_get_brand(void)
{
	char *name = (char *)CPUIDBrandVar;
	cpuid(0x80000002, (uint32_t *)(name + 0), (uint32_t *)(name + 4), (uint32_t *)(name + 8), (uint32_t *)(name + 12));
	cpuid(0x80000003, (uint32_t *)(name + 16), (uint32_t *)(name + 20), (uint32_t *)(name + 24), (uint32_t *)(name + 28));
	cpuid(0x80000004, (uint32_t *)(name + 32), (uint32_t *)(name + 36), (uint32_t *)(name + 40), (uint32_t *)(name + 44));
	name[47] = 0;
	return name;
}

/* 检测CPU是否存在 */
int cpu_detect(void)
{
	char *brand = cpu_get_brand();
	return brand[0] != 0;
}

/* 检测CPU是否支持64位 */
int cpu_is_64b(void)
{
	uint32_t eax, ebx, ecx, edx;
	cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
	return (edx & (1 << 29));
}
