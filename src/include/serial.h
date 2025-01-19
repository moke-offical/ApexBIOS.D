/*
 *		serial.h
 *		RS-232串口驱动头文件
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#ifndef INCLUDE_SERIAL_H_
#define INCLUDE_SERIAL_H_

#include "ports.h"

#define COM1 0x3F8

/* 通过串口发送字符串 */
void serial_put_str(char *str);

/* 通过串口发送单个字符 */
void serial_put_char(char ch);

#endif // INCLUDE_SERIAL_H_
