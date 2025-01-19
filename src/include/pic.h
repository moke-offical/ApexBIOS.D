/*
 *		pic.h
 *		可编程中断控制器头文件
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#ifndef INCLUDE_PIC_H_
#define INCLUDE_PIC_H_

#include <stdint.h>

#define PIC_MASTER_CMD 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_CMD 0xa0
#define PIC_SLAVE_DATA 0xa1
#define PIC_MASTER_ELCR 0x4d0
#define PIC_SLAVE_ELCR 0x4d1

#define PIC_ICW1_INIT ((1 << 4) | (1 << 0))
#define PIC_ICW4_8086 (1 << 0)

/* 初始化主从PIC控制器 */
void pic_init(uint8_t master_base, uint8_t slave_base);

/* 使能指定的IRQ */
void pic_enable_irq(uint8_t irq);

/* 禁用指定的IRQ */
void pic_disable_irq(uint8_t irq);

/* 设置指定的IRQ为电平触发模式 */
void pic_set_level(uint8_t irq);

/* 指定的IRQ为边沿触发模式 */
void pic_set_edge(uint8_t irq);

#endif // INCLUDE_PIC_H_
