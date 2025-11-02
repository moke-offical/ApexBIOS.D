/*
 *		atapio.c
 *		ATA输入输出驱动
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#include "atapio.h"

/* LBA读取ATA设备 */
char *ata_read_lba(uint32_t lba, void *buffer)
{
	outb(ATA_MASTER + 6, 0xE0 | ((lba >> 24) & 0x0F));
	outb(ATA_MASTER + 1, 0);
	outb(ATA_MASTER + 2, 1);
	outb(ATA_MASTER + 3, lba);
	outb(ATA_MASTER + 4, lba >> 8);
	outb(ATA_MASTER + 5, lba >> 16);
	outb(ATA_MASTER + 7, ATA_READ_SECTORS_COMMAND);
	while (inb(ATA_MASTER + 7) & 128);

	for (int i = 0; i < 256; i++) {
		*((uint16_t*)ATA_BUFFER + i) = inw(ATA_MASTER);
	}
	memcpy(buffer,(void*)ATA_BUFFER,512);
	return buffer;
}

/* ATA控制器是否存在 */
int ata_controller_detect(void)
{
	return PCI_finl_deviceW(0x1, 0x1);
}

/* ATA设备是否存在 */
int ata_driver_detect(void)
{
	if (inb(ATA_MASTER + 0x07) == 0)
		return 0;
	else
		return 1;
}
