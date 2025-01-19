/*
 *		eis.c
 *		指令集相关
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#include "eis.h"

/* FPU是否存在 */
int fpu_detect(void)
{
	uint32_t cr0;
	__asm__ volatile("mov %%cr0,%0" : "=r"(cr0));
	return !(cr0 & 0xF00);
}

/* FPU初始化 */
void fpu_init(void)
{
	__asm__ volatile("finit");
}

/* SSE指令集是否存在 */
int sse_detect(void)
{
	uint32_t eax, ebx, ecx, edx;
	cpuid(0x1, &eax, &ebx, &ecx, &edx);
	return (ebx & (1 << 25)) | (ebx & (1 << 26)) | (ecx & (1 << 0)) | (ecx & (1 << 19)) | (ecx & (1 << 20));
}

/* SSE指令集初始化 */
void sse_init(void)
{
	uint32_t tmp;
	__asm__ volatile("mov %%cr0, %0" : "=r"(tmp));
	tmp &= 0xFFFB;
	tmp |= 2;
	__asm__ volatile("mov %0, %%cr0" :: "r"(tmp));
	__asm__ volatile("mov %%cr4, %0" : "=r"(tmp));
	tmp |= 3 << 9;
	__asm__ volatile("mov %0, %%cr4" :: "r"(tmp));
}

/* AVX指令集是否存在 */
int avx_detect(void)
{
	uint32_t eax, ebx, ecx, edx;
	cpuid(0x1, &eax, &ebx, &ecx, &edx);
	return (ecx & (1 << 28)); 
}

/* AVX指令集初始化 */
void avx_init(void)
{
	uint32_t tmp = 0;
	__asm__ volatile("mov %0,%%ecx" : "=r"(tmp));
	__asm__ volatile("xgetbv");
	__asm__ volatile("mov %%eax,%0" : "=r"(tmp));
	tmp |= 7;
	__asm__ volatile("mov %0,%%eax" : "=r"(tmp));
	__asm__ volatile("xsetbv");
}
