/*
 *		cstring.h
 *		基本字符串处理头文件
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#ifndef INCLUDE_CMOS_H_
#define INCLUDE_CMOS_H_

#include <stdint.h>
#include "memory.h"

#define Buffer1 0x1020
#define Buffer2 0x1040

/* 计算字符串的长度 */
int strlen(char *str);

/* 比较两个字符串 */
int strcmp(const char *s1, const char *s2);

/* 将一个32位无符号整数转换为字符串 */
int inttostr(uint32_t num);

/* 将一个小写字母转换为大写字母 */
char toupper(char chr);

#endif // INCLUDE_CMOS_H_
