/*
 *		endianness.c
 *		字节序转换相关
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#include "endianness.h"

/* 交换8位字节的高4位和低4位 */
uint8_t swapendianness8(uint8_t byte)
{
	return (byte >> 4) | (byte << 4);
}

/* 交换16位字的高8位和低8位 */
uint16_t swapendianness16(uint16_t byte)
{
	return (byte >> 8) | (byte << 8);
}

/* 交换32位字的字节序 */
uint32_t swapendianness32(uint32_t byte)
{
	return ((byte >> 8) & 0x0000FF00) | ((byte >> 24) & 0x000000FF) |((byte << 24) & 0xFF000000) | ((byte << 8) & 0x00FF0000);
}

/* 交换64位字的字节序 */
uint64_t swapendianness64(uint64_t byte)
{
	return ((uint64_t) swapendianness32(byte & 0xFFFFFFFF) << 32) | swapendianness32((byte >> 32) & 0xFFFFFFFF);
}
