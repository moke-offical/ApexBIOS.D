/*
 *		serial.c
 *		RS-232串口驱动
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#include "serial.h"

/* 通过串口发送字符串 */
void serial_put_str(char *str)
{
	for (int i = 0; str[i] != '\0';i++) {
		if(str[i] == '\n')
			serial_put_char('\r');
		serial_put_char(str[i]);    
	}
}

/* 通过串口发送单个字符 */
void serial_put_char(char ch)
{
	outb(COM1, ch);
}
