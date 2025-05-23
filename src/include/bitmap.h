/*
 *		bitmap.h
 *		位图图像解析头文件
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#ifndef INCLUDE_BITMAP_H_
#define INCLUDE_BITMAP_H_

#include <stdint.h>

typedef struct {
	uint16_t magic;
	uint32_t fileSize;
	uint32_t reserved;
	uint32_t bmpDataOffset;
	uint32_t bmpInfoSize;
	uint32_t frameWidth;
	uint32_t frameHeight;
	uint16_t reservedValue;
	uint16_t bitsPerPixel;
	uint32_t compressionMode;
	uint32_t frameSize;
	uint32_t horizontalResolution;
	uint32_t verticalResolution;
	uint32_t usedColorCount;
	uint32_t importantColorCount;
} __attribute__((packed)) Bmp;

/* 解析BMP并显示 */
void bmp_analysis(Bmp *bmp, uint32_t x, uint32_t y, int isTransparent);

#endif // INCLUDE_BITMAP_H_
