/*
 *		eis.h
 *		指令集相关头文件
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#ifndef INCLUDE_EIS_H_
#define INCLUDE_EIS_H_

#include <stdint.h>
#include "ramfb.h"
#include "cpuid.h"

/* FPU是否存在 */
int fpu_detect(void);

/* FPU初始化 */
void fpu_init(void);

/* SSE指令集是否存在 */
int sse_detect(void);

/* SSE指令集初始化 */
void sse_init(void);

/* AVX指令集是否存在 */
int avx_detect(void);

/* AVX指令集初始化 */
void avx_init(void);

#endif // INCLUDE_EIS_H_
