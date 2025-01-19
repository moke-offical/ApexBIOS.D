/*
 *		ps2.c
 *		PS2设备驱动
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#include "ps2.h"

/* 等待PS/2控制器的输出缓冲区准备好 */
void PS2_wait_response(void)
{
	while (!(inb(PS2_STATUS_PORT) & 1));
}

/* 等待PS/2控制器的输入缓冲区准备好 */
void PS2_wait_input(void)
{
	while (inb(PS2_STATUS_PORT) & 2);
}

/* 进行PS/2控制器的自检 */
uint8_t PS2_test(void)
{
	outb(PS2_COMMAND_PORT,0xAA);
	PS2_wait_response();
	return inb(PS2_DATA_PORT) == 0x55;
}

/* 等待用户按键并返回按键的ASCII码 */
uint8_t PS2_wait_for_key(void)
{
	const char ScanCodeSet1[] = {
		0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
		'\b', 0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
		'\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
		0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
	};
	while(1) {
		uint8_t c = inb(0x60);
		if(c > 0) return ScanCodeSet1[c];
	}
}

/* 检测PS/2控制器是否存在 */
uint8_t PS2_detect(void)
{
	return PS2_test();
}

/* 获取PS/2控制器的配置字节 */
uint8_t PS2_get_config(void)
{
	PS2_wait_input();
	outb(PS2_COMMAND_PORT, 0x20);
	PS2_wait_response();
	return inb(0x60);	
}

/* 设置PS/2控制器的配置字节 */
void PS2_set_config(uint8_t cfg)
{
	PS2_wait_input();
	outb(PS2_COMMAND_PORT, 0x60);
	PS2_wait_input();
	outb(0x60, cfg);	
}

/* 初始化PS/2控制器 */
void PS2_init(void)
{
	outb(PS2_COMMAND_PORT, 0xAD);
	PS2_wait_input();

	outb(PS2_COMMAND_PORT, 0xA7);
	PS2_wait_input();

	inb(PS2_DATA_PORT);

	PS2_set_config(PS2_get_config() & ~1);
	PS2_set_config(PS2_get_config() & ~(1 << 1));

	PS2_set_config(PS2_get_config() & ~(1 << 6));

	PS2_test();

	outb(PS2_COMMAND_PORT, 0xAE);
	PS2_wait_input();

	outb(PS2_COMMAND_PORT, 0xA8);
	PS2_wait_input();

	PS2_set_config(PS2_get_config() | 1);
	PS2_set_config(PS2_get_config() | (1 << 1));

	PS2_set_config(PS2_get_config() | (1 << 6));

	outb(PS2_COMMAND_PORT, 0xFF);
	PS2_wait_input();

	outb(PS2_COMMAND_PORT, 0xD3);
	PS2_wait_input();
	outb(PS2_COMMAND_PORT, 0xFF);
	PS2_wait_input();
}
