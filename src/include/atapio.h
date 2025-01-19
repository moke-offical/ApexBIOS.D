/*
 *		atapio.h
 *		ATA输入输出驱动头文件
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#ifndef INCLUDE_ATAPIO_H_
#define INCLUDE_ATAPIO_H_

#include <stdint.h>
#include "ports.h"
#include "memory.h"
#include "pci.h"

#define ATA_MASTER 0x1F0
#define ATA_SLAVE 0x170

#define ATA_READ_SECTORS_COMMAND 0x20
#define ATA_BUFFER 0x1090

/* LBA读取ATA设备 */
char *ata_read_lba(uint32_t lba, void *buffer);

/* ATA控制器是否存在 */
int ata_controller_detect(void);

/* ATA设备是否存在 */
int ata_driver_detect(void);

#endif // INCLUDE_ATAPIO_H_
