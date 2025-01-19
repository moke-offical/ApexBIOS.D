/*
 *		floppy.h
 *		软盘驱动头文件
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#ifndef INCLUDE_FLOPPY_H_
#define INCLUDE_FLOPPY_H_

#include <stdint.h>
#include "cmos.h"
#include "ramfb.h"

#define FLOPPY_BASE_PORT 0x3F0

#define FLOPPY_DOR_REG 2
#define FLOPPY_MSR_REG 4
#define FLOPPY_FIFO_REG 5
#define FLOPPY_CCR_REG 7

#define FLOPPY_SPECIFY_COMMAND 3
#define FLOPPY_WRITE_COMMAND 5
#define FLOPPY_READ_COMMAND 6
#define FLOPPY_RECALIBRATE_COMMAND 7
#define FLOPPY_SENSE_COMMAND 8
#define FLOPPY_SEEK_COMMAND 15

#define FLOPPY_DRIVE_MASTER 0
#define FLOPPY_DRIVE_SLAVE 1

#define FLOPPY_DMA_WRITE 0x4A
#define FLOPPY_DMA_READ 0x46
#define FLOPPY_DMA_LEN 0x4800
#define FLOPPY_DMA_ADDRESS 0x8000

/* 获取系统中软盘驱动器的数量 */
uint8_t floppy_get_drives(void);

/* 向软盘控制器发送一个命令 */
void floppy_do_command(uint8_t command);

/* 从软盘控制器的数据寄存器中读取一个字节的数据 */
uint8_t floppy_read_fifo(void);

/* 处理软盘驱动器中断 */
void floppy_sense_intrerrupt(uint8_t *cylinler, uint8_t *status);

/* 启动软盘驱动器的电机 */
void floppy_start_motor(void);

/* 停止软盘驱动器的电机 */
void floppy_stop_motor(void);

/* 对指定的软盘驱动器进行校准 */
void floppy_calibrate(uint8_t drive);

/* 清空软盘控制器的中断队列 */
void floppy_flush_intrerrupt(void);

/* 初始化软盘驱动器 */
void floppy_init(uint8_t drive);

/* 将软盘的磁头移动到指定的柱面和磁头位置 */
void floppy_seek(uint16_t cylinler, uint16_t head);

#endif // INCLUDE_FLOPPY_H_
