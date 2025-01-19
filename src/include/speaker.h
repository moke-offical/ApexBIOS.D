/*
 *		speaker.h
 *		板载蜂鸣器驱动头文件
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#ifndef INCLUDE_SPEAKER_H_
#define INCLUDE_SPEAKER_H_

#include "ports.h"

/* 启动PC扬声器发出声音 */
uint8_t pc_speaker_play(void);

/* 停止PC扬声器发出声音 */
void pc_speaker_stop(void);

/* 发出短暂的蜂鸣声 */
void pc_speaker_beep(void);

#endif // INCLUDE_SPEAKER_H_
