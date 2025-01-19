/*
 *		speaker.c
 *		板载蜂鸣器驱动
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#include "speaker.h"

/* 启动PC扬声器发出声音 */
uint8_t pc_speaker_play(void)
{
	outb(0x43, 0xb6);
	outb(0x42, (uint8_t) (1193) );
	outb(0x42, (uint8_t) (1193 >> 8));
	outb(0x61, inb(0x61) | 3);
}

/* 停止PC扬声器发出声音 */
void pc_speaker_stop(void)
{
	outb(0x61, inb(0x61) & 0xFC);
}

/* 发出短暂的蜂鸣声 */
void pc_speaker_beep(void)
{
	pc_speaker_play();
	for (volatile int i = 0; i<0xFFFFFF; i++) __asm__ volatile ("nop");
	pc_speaker_stop();
	for (volatile int i = 0; i<0xFFFFFF; i++) __asm__ volatile ("nop");
}
