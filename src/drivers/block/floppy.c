/*
 *		floppy.c
 *		软盘驱动
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#include "floppy.h"

#define delay 0xFFFFFF

/* 获取系统中软盘驱动器的数量 */
uint8_t floppy_get_drives(void)
{
	uint8_t drives = cmos_get_reg(0x10);
	uint8_t master = ((drives >> 4) & 0x0F);
	uint8_t slave = (drives & 0x0F);
	if (master == 0 && slave == 0)
		return 0;
	else if ((master > 0 && slave == 0) || (master == 0 && slave > 0))
		return 1;
	return 2;
}

/* 向软盘控制器发送一个命令 */
void floppy_do_command(uint8_t command)
{
	for (int i = 0; i < 0xFFFF; i++) {
		if (inb(FLOPPY_BASE_PORT + FLOPPY_MSR_REG) & 0x80) {
			outb(FLOPPY_BASE_PORT + FLOPPY_FIFO_REG, command);
			return;
		}
	}
}

/* 从软盘控制器的数据寄存器中读取一个字节的数据 */
uint8_t floppy_read_fifo(void)
{
	for (int i = 0; i < 0xFFFF; i++) {
		if (inb(FLOPPY_BASE_PORT + FLOPPY_MSR_REG) & 0x80) {
			return inb(FLOPPY_BASE_PORT + FLOPPY_FIFO_REG);
		}
	}
}

/* 从软盘控制器的数据寄存器中读取一个字节的数据 */
void floppy_sense_intrerrupt(uint8_t *cylinler, uint8_t *status)
{
	floppy_do_command(FLOPPY_SENSE_COMMAND);
	*status = floppy_read_fifo();
	*cylinler = floppy_read_fifo();
}

/* 启动软盘驱动器的电机 */
void floppy_start_motor(void)
{
	outb(FLOPPY_BASE_PORT + FLOPPY_DOR_REG, 0x1C);
}

/* 停止软盘驱动器的电机 */
void floppy_stop_motor(void)
{
	outb(FLOPPY_BASE_PORT + FLOPPY_DOR_REG, 0x0C);
}

/* 对指定的软盘驱动器进行校准 */
void floppy_calibrate(uint8_t drive)
{
	floppy_start_motor();
	for (int i = 0; i < 10; i++) {
		floppy_do_command(FLOPPY_RECALIBRATE_COMMAND);
		floppy_do_command(drive);
		for(int j = 0; j < delay; j++);
		int cylinler, status;
		floppy_sense_intrerrupt(&cylinler, &status);
		if (!cylinler) {
			floppy_stop_motor();
			return;
		}
	}
}

/* 清空软盘控制器的中断队列 */
void floppy_flush_intrerrupt(void)
{
	int tmp;
	floppy_sense_intrerrupt(&tmp, &tmp);
}

/* 初始化软盘驱动器 */
void floppy_init(uint8_t drive)
{
	outb(FLOPPY_BASE_PORT + FLOPPY_DOR_REG, 0x00);
	outb(FLOPPY_BASE_PORT + FLOPPY_DOR_REG, 0x0C);
	for (int j = 0; j < delay; j++);
	for (int i = 0; i <= 4; i++)
		floppy_flush_intrerrupt();
	outb(FLOPPY_BASE_PORT + FLOPPY_CCR_REG, 0x00);
	floppy_do_command(FLOPPY_SPECIFY_COMMAND);
	floppy_do_command(0xDF);
	floppy_do_command(0x02);
	floppy_calibrate(drive);
}

/* 将软盘的磁头移动到指定的柱面和磁头位置 */
void floppy_seek(uint16_t cylinler, uint16_t head)
{
	floppy_start_motor();
	for (int i = 0; i < 10; i++) {
		floppy_do_command(FLOPPY_SEEK_COMMAND);
		floppy_do_command(head << 2);
		floppy_do_command(cylinler);
		for (int j = 0; j < delay; j++);
		int cylinler, status;
		floppy_sense_intrerrupt(&cylinler, &status);
		if (cylinler) {
			floppy_stop_motor();
			return;
		}
	}
}
