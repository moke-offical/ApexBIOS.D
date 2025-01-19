/*
 *		pic.c
 *		可编程中断控制器
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#include "ports.h"
#include "pic.h"

/* 初始化主从PIC控制器 */
void pic_init(uint8_t master_base, uint8_t slave_base)
{
	outb(PIC_MASTER_CMD, PIC_ICW1_INIT);
	outb(PIC_SLAVE_CMD, PIC_ICW1_INIT);

	outb(PIC_MASTER_DATA, master_base);
	outb(PIC_SLAVE_DATA, slave_base);

	outb(PIC_MASTER_DATA, 4);
	outb(PIC_SLAVE_DATA, 2);

	outb(PIC_MASTER_DATA, PIC_ICW4_8086);
	outb(PIC_SLAVE_DATA, PIC_ICW4_8086);

	outb(PIC_MASTER_DATA, 0xff);
	outb(PIC_SLAVE_DATA, 0xff);
}

/* 使能指定的IRQ */
void pic_enable_irq(uint8_t irq)
{
	if (irq > 15) {
		return;
	}
	if (irq > 7) {
		irq -= 8;
		outb(PIC_SLAVE_DATA, inb(PIC_SLAVE_DATA) & ~(1 << irq));
	} else {
		outb(PIC_MASTER_DATA, inb(PIC_MASTER_DATA) & ~(1 << irq));
	}
}

/* 禁用指定的IRQ */
void pic_disable_irq(uint8_t irq)
{
	if (irq > 15) {
		return;
	}
	if (irq > 7) {
		irq -= 8;
		outb(PIC_SLAVE_DATA, inb(PIC_SLAVE_DATA) | (1 << irq));
	} else {
		outb(PIC_MASTER_DATA, inb(PIC_MASTER_DATA) | (1 << irq));
	}
}

/* 设置指定的IRQ为电平触发模式 */
void pic_set_level(uint8_t irq)
{
	if (irq > 15) {
		return;
	}
	if (irq > 7) {
		irq -= 8;
		outb(PIC_SLAVE_ELCR, inb(PIC_SLAVE_ELCR) | (1 << irq));
	} else {
		outb(PIC_MASTER_ELCR, inb(PIC_MASTER_ELCR) | (1 << irq));
	}
}

/* 指定的IRQ为边沿触发模式 */
void pic_set_edge(uint8_t irq)
{
	if (irq > 15) {
		return;
	}
	if (irq > 7) {
		irq -= 8;
		outb(PIC_SLAVE_ELCR, inb(PIC_SLAVE_ELCR) & ~(1 << irq));
	} else {
		outb(PIC_MASTER_ELCR, inb(PIC_MASTER_ELCR) & ~(1 << irq));
	}
}
