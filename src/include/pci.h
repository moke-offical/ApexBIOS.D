/*
 *		pci.h
 *		PCI设备驱动头文件
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#ifndef INCLUDE_PCI_H_
#define INCLUDE_PCI_H_

#include <stdint.h>
#include "ports.h"

#define PCI_CFG_ADDRESS 0xCF8
#define PCI_CFG_VENDOR 0x00
#define PCI_CFG_DEVICE 0x02
#define PCI_CFG_INTERFACE 0x09
#define PCI_CFG_SUBCLASS 0x0A
#define PCI_CFG_CLASS 0x0B

typedef struct {
	uint16_t Vendor;
	uint16_t Device;
} PCIChipset;

#define Q35_VENDOR 0x8086
#define Q35_DEVICE 0x29C0
#define I440FX_VENDOR 0x8086
#define I440FX_DEVICE 0x1237
#define MICROVM_VENDOR_DEVICE 0xFFFF

#define IS_Q35 (PCI_get_chipset().Device == Q35_DEVICE && PCI_get_chipset().Vendor == Q35_VENDOR)
#define IS_I440FX (PCI_get_chipset().Device == I440FX_DEVICE && PCI_get_chipset().Vendor == I440FX_VENDOR)
#define IS_MICROVM (PCI_get_chipset().Device == MICROVM_VENDOR_DEVICE && PCI_get_chipset().Vendor == MICROVM_VENDOR_DEVICE)

/* 读取PCI设备的32位配置寄存器 */
uint16_t PCI_read_dword(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset);

/* 读取PCI设备的16位配置寄存器 */
uint16_t PCI_read_word(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset);

/* 读取PCI设备的8位配置寄存器 */
uint16_t PCI_read_byte(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset);

/* 写入PCI设备的32位配置寄存器 */
void PCI_write_dword(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint32_t data);

/* 写入PCI设备的16位配置寄存器 */
void PCI_write_word(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint16_t data);

/* 读取PCI设备的8位配置寄存器 */
void PCI_write_byte(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint8_t data);

/* 获取设备基地址寄存器值 */
uint64_t PCI_get_bar(uint8_t bus, uint8_t slot, uint8_t function, int bar);

/* 获取PCI芯片组的厂商和设备ID */
PCIChipset PCI_get_chipset(void);

/* 查找指定类、子类和编程接口的PCI设备 */
uint8_t PCI_finl_device(uint8_t cls, uint8_t subcls, uint8_t programif);

/* 查找指定类和子类的PCI设备，不考虑编程接口 */
uint8_t PCI_finl_deviceW(uint8_t cls, uint8_t subcls);

#endif // INCLUDE_PCI_H_
