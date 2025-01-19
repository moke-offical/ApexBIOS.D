/*
 *		ahci.h
 *		ahci驱动头文件
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#ifndef INCLUDE_AHCI_H_
#define INCLUDE_AHCI_H_

#include <stdint.h>

/* AHCI控制器是否存在 */
uint8_t ahci_detect_controller(void);

#endif // INCLUDE_AHCI_H_
