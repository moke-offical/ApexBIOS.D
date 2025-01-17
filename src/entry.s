/*
 *		entry.s
 *		适用于x86的基本输入输出程序（ApexBIOS v0.01）
 *
 *		基于 MIT 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

.intel_syntax noprefix							/* 使用 Intel 语法，无需前缀 */
.code16											/* 使用 16 位代码 */

/* 定义超级 I/O 基地址 */
SUPERIO_BASE	= 0x2e

/* 定义超级 I/O 寄存器地址 */
PC97338_FER		= 0x00							/* 功能启用寄存器 */
PC97338_FAR		= 0x01							/* 功能地址寄存器 */
PC97338_PTR		= 0x02							/* 端口地址寄存器 */

/* 定义串口基地址及相关寄存器地址 */
COM_BASE		= 0x3f8							/* 串口基地址 */
COM_RB			= 0x00							/* 接收缓冲区寄存器 */
COM_TB			= 0x00							/* 发送缓冲区寄存器 */
COM_BRD_LO		= 0x00							/* 波特率除数寄存器低字节 */
COM_BRD_HI		= 0x01							/* 波特率除数寄存器高字节 */
COM_IER			= 0x01							/* 中断使能寄存器 */
COM_FCR			= 0x02							/* FIFO 控制寄存器 */
COM_LCR			= 0x03							/* 线路控制寄存器 */
COM_MCR			= 0x04							/* 调制解调器控制寄存器 */
COM_LSR			= 0x05							/* 线路状态寄存器 */

/* 定义 ROM 调用宏，用于调用指定地址的代码 */
.macro ROM_CALL addr
	mov sp, offset 1f							/* 将栈指针移动到标签 1 的位置 */
	jmp \addr									/* 跳转到指定地址 */
1:												/* 标签 1 */
.endm

/* 定义开始段，包含 BIOS 版本信息 */
.section begin, "a"
	.ascii "APEXBIOS v0.20"						/* BIOS 版本信息字符串 */
	.align 16									/* 16 字节对齐 */

/* 定义主程序段 */
.section main, "ax"
.globl entry									/* 定义全局入口点 */
entry:
	cli											/* 关闭中断 */
	cld											/* 清除方向标志 */
	mov ax, cs									/* 将代码段寄存器值赋给 ax */
	mov ds, ax									/* 将 ax 值赋给数据段寄存器 */
	mov ss, ax									/* 将 ax 值赋给堆栈段寄存器 */

/* 初始化超级 I/O */
init_superio:
	mov dx, SUPERIO_BASE						/* 将超级 I/O 基地址赋给 dx */
	in al, dx									/* 从 dx 指定端口读取数据到 al */
	in al, dx									/* 再次读取，用于某些硬件初始化 */
	mov si, offset superio_conf					/* 将超级 I/O 配置数据的偏移地址赋给 si */
	mov cx, (serial_conf - superio_conf) / 2	/* 计算配置数据长度 */
.write_superio_conf:
	mov ax, [si]								/* 从 si 指向的地址读取数据到 ax */
	ROM_CALL superio_out						/* 调用 ROM 中的 superio_out 函数 */
	add si, 0x02								/* si 指针后移 2 字节 */
	loop .write_superio_conf					/* 循环直到 cx 为 0 */

/* 初始化串口 */
init_serial:
	mov si, offset serial_conf					/* 将串口配置数据的偏移地址赋给 si */
	mov cx, (start_string - serial_conf) / 2	/* 计算配置数据长度 */
.write_serial_conf:
	mov ax, [si]								/* 从 si 指向的地址读取数据到 ax */
	ROM_CALL serial_out							/* 调用 ROM 中的 serial_out 函数 */
	add si, 0x02								/* si 指针后移 2 字节 */
	loop .write_serial_conf						/* 循环直到 cx 为 0 */

/* 打印启动信息字符串 */
print_start:
	mov si, offset start_string					/* 将启动信息字符串的偏移地址赋给si */
	ROM_CALL print_string						/* 调用print_string函数打印字符串 */

/* 死循环，用于暂停程序 */
clog:
	hlt											/* 暂停当前处理器的活动 */
	jmp clog									/* 无条件跳转到 clog，形成死循环 */

/* 超级 I/O 输出函数 */
superio_out:
	mov dx, SUPERIO_BASE						/* 将超级 I/O 基地址赋给 dx */
	out dx, al									/* 将 al 中的数据输出到 dx 指定端口 */
	inc dx										/* dx 增 1 */
	xchg al, ah									/* 交换 al 和 ah 的值 */
	out dx, al									/* 将 al 中的数据输出到 dx 指定端口 */
	jmp sp										/* 跳转到 sp 指向的地址 */

/* 串口输出函数 */
serial_out:
	mov dx, COM_BASE							/* 将串口基地址赋给 dx */
	add dl, al									/* 将 al 中的数据加到 dl 上 */
	mov al, ah									/* 将 ah 中的数据赋给 al */
	out dx, al									/* 将 al 中的数据输出到 dx 指定端口 */
	jmp sp										/* 跳转到 sp 指向的地址 */

/* 输出字符到串口 */
putchar:
	mov dx, COM_BASE + COM_LSR					/* 设置端口地址为串口的线路状态寄存器 */
	mov ah, al									/* 将要输出的字符保存在ah中 */
.tx_wait:
	in al, dx									/* 从线路状态寄存器读取状态 */
	and al, 0x20								/* 检查发送缓冲区是否为空（0x20表示发送缓冲区空） */
	jz .tx_wait									/* 如果不为空，继续等待 */
	mov dx, COM_BASE + COM_TB					/* 设置端口地址为串口的发送缓冲区 */
	mov al, ah									/* 将要输出的字符恢复到al中 */
	out dx, al									/* 将字符输出到发送缓冲区 */
	jmp sp										/* 结束函数 */

/* 打印字符串到串口 */
print_string:
	lodsb										/* 加载字符串中的下一个字符到al，si自增 */
	or al, al									/* 判断al是否为0 */
	jnz .write_char								/* 不为0则跳转到write_char */
	jmp sp										/* 为0则结束函数 */
.write_char:
	shl esp, 0x10								/* 左移esp */
	ROM_CALL putchar							/* 调用putchar函数输出字符 */
	shr esp, 0x10								/* 右移esp恢复原值 */
	jmp print_string							/* 循环继续处理下一个字符 */

/* 超级 I/O 配置数据 */
superio_conf:
	.byte PC97338_FER,	0x0f					/* 功能启用寄存器配置 */
	.byte PC97338_FAR,	0x10					/* 功能地址寄存器配置 */
	.byte PC97338_PTR,	0x00					/* 端口地址寄存器配置 */

/* 串口配置数据 */
serial_conf:
	.byte COM_MCR,		0x00					/* 调制解调器控制寄存器配置 */
	.byte COM_FCR,		0x07					/* FIFO 控制寄存器配置 */
	.byte COM_LCR,		0x80					/* 线路控制寄存器配置 */
	.byte COM_BRD_LO,	0x01					/* 波特率除数寄存器低字节配置 */
	.byte COM_BRD_HI,	0x00					/* 波特率除数寄存器高字节配置 */
	.byte COM_LCR,		0x03					/* 线路控制寄存器配置 */

/* 开始字符串，用于显示 BIOS 启动信息 */
start_string:
	.string "ApexBIOS v0.01 Starting.\r\n"

/* 定义重置段，用于 BIOS 重置时跳转到入口点 */
.section reset, "ax"
	.byte 0xe9									/* 跳转指令的 opcode */
	.int entry - ( . + 2 )						/* 计算跳转偏移量 */
	.align 16, 0xff								/* 16 字节对齐，填充 0xff */
