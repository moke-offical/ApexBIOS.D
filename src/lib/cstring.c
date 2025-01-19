/*
 *		cstring.c
 *		基本字符串处理
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#include "cstring.h"

/* 计算字符串的长度 */
int strlen(char *str)
{
	int i = 0;
	for(; str[i] !=0; i++);
	return i;
}

/* 比较两个字符串 */
int strcmp(const char *s1, const char *s2)
{
	while (*s1 == *s2++)
		if (*s1++ == 0)
			return 0;
	return (*(unsigned char *)s1 - *(unsigned char *)--s2);
}

/* 将一个32位无符号整数转换为字符串 */
int inttostr(uint32_t num)
{
	int k = 0, tmp = 0;

	do {
		tmp = num % 10;
		*((char *)Buffer1 + (k++)) = tmp + '0';
		num = num / 10;
	} while(num != 0);

	tmp = k;
	k = 0;

	for (int i = tmp - 1; i >= 0; i--) {
		*((char *)Buffer2 + (k++)) = *((char *)Buffer1 + i);
	}
	*((char *)Buffer2+tmp) = '\0';
	return Buffer2;
}

/* 将一个小写字母转换为大写字母 */
char toupper(char chr)
{
	if(chr >= 'A' && chr <= 'Z')
		return chr + 32;
	return chr;
}
