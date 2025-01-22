/*
 *		ramfb.h
 *		基于RAMFB的图形显示驱动头文件
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#ifndef INCLUDE_RAMFB_H_
#define INCLUDE_RAMFB_H_

#include <stdint.h>
#include <stddef.h>
#include "serial.h"
#include "endianness.h"
#include "ports.h"
#include "cstring.h"

#define FWCFG_DMA_PORT 0x518
#define FWCFG_ROOT 0x19

#define FWCFGCommand_ERROR (1 << 0)
#define FWCFGCommand_READ (1 << 1)
#define FWCFGCommand_SKIP (1 << 2)
#define FWCFGCommand_SELECT (1 << 3)
#define FWCFGCommand_WRITE (1 << 4)

#define VideoMemory 0x100000

#define CursorX *((uint32_t*)0x1000)
#define CursorY *((uint32_t*)0x1004)
#define ScreenW *((uint32_t*)0x1008)
#define ScreenH *((uint32_t*)0x100C)
#define Color *((uint32_t*)0x1010)
#define FontScaling 2

struct FWCFGFile {
	uint32_t Size;
	uint16_t Selector;
	uint8_t Offset[2];
	char Name[56];
} __attribute__((packed));

struct FWCFGCommand {
	uint32_t Control;
	uint32_t Length;
	uint64_t Address;
} __attribute__((packed));

struct RAMFBStruct {
	uint64_t Address;
	uint32_t FOURCC;
	uint32_t Flags;
	uint32_t Width;
	uint32_t Height;
	uint32_t Stride;
} __attribute__((packed)) ;

/* 在FWCFG中查找指定文件的信息 */
int FWCFG_locate_file(char *filename, struct FWCFGFile *info);

/* 从FWCFG中读取数据 */
void FWCFG_read(uint16_t selector, void *buf, uint32_t len, uint32_t offset);

/* 向FWCFG中写入数据 */
void FWCFG_write(uint16_t selector, const void *buf, uint32_t len, uint32_t offset);

/* 在指定位置绘制一个像素 */
void RAMFB_put_pix(int x, int y, uint32_t clr);

/* 绘制一个填充矩形 */
void RAMFB_put_rectf(int x, int y, int w, int h, uint32_t clr);

/* 绘制一个空心矩形 */
void RAMFB_put_rect(int x, int y, int w, int h, uint32_t clr);

/* 在指定位置绘制一个字符 */
void RAMFB_put_char(int x, int y, char chr);

/* 在屏幕上绘制字符串 */
void RAMFB_put_str(char *str);

/* 设置绘制颜色 */
void RAMFB_set_color(uint32_t color);

/* 设置光标位置 */
void RAMFB_set_cursor(int x, int y);

/* 初始化RAMFB */
void RAMFB_init(int width, int height);

#endif // INCLUDE_RAMFB_H_
