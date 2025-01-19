/*
 *		ahci.c
 *		ahci驱动
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#include "ahci.h"

/* AHCI控制器是否存在 */
uint8_t ahci_detect_controller(void)
{
	return PCI_finl_device(0x1, 0x6, 0x1);
}
