/*
 *		endianness.h
 *		字节序转换相关头文件
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#ifndef INCLUDE_ENDIANNESS_H_
#define INCLUDE_ENDIANNESS_H_

#include <stdint.h>

/* 交换8位字节的高4位和低4位 */
uint8_t swapendianness8(uint8_t byte);

/* 交换16位字的高8位和低8位 */
uint16_t swapendianness16(uint16_t byte);

/* 交换32位字的字节序 */
uint32_t swapendianness32(uint32_t byte);

/* 交换64位字的字节序 */
uint64_t swapendianness64(uint64_t byte);

#endif // INCLUDE_ENDIANNESS_H_
