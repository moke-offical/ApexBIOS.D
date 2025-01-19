/*
 *		ps2.h
 *		PS2设备驱动头文件
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#ifndef INCLUDE_PS2_H_
#define INCLUDE_PS2_H_

#include "ports.h"
#include "ramfb.h"

#define PS2_DATA_PORT 0x60
#define PS2_STATUS_PORT 0x64
#define PS2_COMMAND_PORT 0x64

/* 等待PS/2控制器的输出缓冲区准备好 */
void PS2_wait_response(void);

/* 等待PS/2控制器的输入缓冲区准备好 */
void PS2_wait_input(void);

/* 进行PS/2控制器的自检 */
uint8_t PS2_test(void);

/* 等待用户按键并返回按键的ASCII码 */
uint8_t PS2_wait_for_key(void);

/* 检测PS/2控制器是否存在 */
uint8_t PS2_detect(void);

/* 获取PS/2控制器的配置字节 */
uint8_t PS2_get_config(void);

/* 设置PS/2控制器的配置字节 */
void PS2_set_config(uint8_t cfg);

/* 初始化PS/2控制器 */
void PS2_init(void);

#endif // INCLUDE_PS2_H_
