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

/* PCI 相关地址 */
PCI_ADDR		= 0xCF8							/* PCI 地址端口 */
PCI_DATA		= 0xCFC							/* PCI 数据端口 */

/* CMOS RTC 相关地址 */
RTC_ADDRESS		= 0x70							/* RTC 地址端口 */

/* I/O Range 配置 */
.if COM_BASE == 0x3F8
	AMD_SB_IO = 0x00000040
	INTEL_SB_IO = 0x30010010
.elseif COM_BASE == 0x2F8
	AMD_SB_IO = 0x00000080
	INTEL_SB_IO = 0x30010001
	VM_SB_IO = 0x04
.elseif COM_BASE == 0x220
	AMD_SB_IO = 0x00000100
	INTEL_SB_IO = 0x30010002
	VM_SB_IO = 0xc8
.elseif COM_BASE == 0x228
	AMD_SB_IO = 0x00000200
	INTEL_SB_IO = 0x30010003
	VM_SB_IO = 0xcc
.elseif COM_BASE == 0x238
	AMD_SB_IO = 0x00000400
	INTEL_SB_IO = 0x30010004
	VM_SB_IO = 0x4c
.elseif COM_BASE == 0x2E8
	AMD_SB_IO = 0x00000800
	INTEL_SB_IO = 0x30010005
	VM_SB_IO = 0x0c
.elseif COM_BASE == 0x338
	AMD_SB_IO = 0x00001000
	INTEL_SB_IO = 0x30010006
	VM_SB_IO = 0x48
.elseif COM_BASE == 0x3E8
	AMD_SB_IO = 0x00002000
	INTEL_SB_IO = 0x30010007
	VM_SB_IO = 0x08
.else
	.print "Invalid COM_BASE"
	.abort
.endif

/* 定义 ROM 调用宏，用于调用指定地址的代码 */
.macro ROM_CALL addr
	mov sp, offset 1f
	jmp \addr
1:
.endm

/* 设置PCI配置地址 */
.macro PCI_CONF_ADDROUT bus, device, function, address
	mov eax, 0x80000000 | (\bus << 24) | (\device << 11) | (\function << 8) | \address
	mov dx, PCI_ADDR
	out dx, eax
.endm

/* 从PCI配置空间读取32位数据 */
.macro PCI_CONF_IN32 bus, device, function, address
	PCI_CONF_ADDROUT \bus, \device, \function, \address
	mov dx, PCI_DATA
	in eax, dx
.endm

/* 向PCI配置空间写入32位数据 */
.macro PCI_CONF_OUT32 bus, device, function, address, data
	PCI_CONF_ADDROUT \bus, \device, \function, \address
	mov dx, PCI_DATA
	mov eax, \data
	out dx, eax
.endm

/* 向诊断卡发送POST代码 */
.macro DIAG value
	.ifnes "\value", "al"
		mov al, \value
	.endif
	out 0x80, al
.endm

/* 设置两个数据四元组 */
.macro SETUP_STACK_DATA
mmx_000000ff:
	.quad 0xff
mmx_0000ffff:
	.quad 0xffff
.endm

/* 确定该寄存器在栈上占用的字节数 */
.macro GET_GP_STACK_DELTA reg
	GP_STACK_DELTA = 1
	.ifeqs "\reg", "ax"
		GP_STACK_DELTA = 2
	.endif
	.ifeqs "\reg", "bx"
		GP_STACK_DELTA = 2
	.endif
	.ifeqs "\reg", "cx"
		GP_STACK_DELTA = 2
	.endif
	.ifeqs "\reg", "dx"
		GP_STACK_DELTA = 2
	.endif
	.ifeqs "\reg", "di"
		GP_STACK_DELTA = 2
	.endif
	.ifeqs "\reg", "si"
		GP_STACK_DELTA = 2
	.endif
	.ifeqs "\reg", "bp"
		GP_STACK_DELTA = 2
	.endif
	.ifeqs "\reg", "sp"
		GP_STACK_DELTA = 2
	.endif
	.ifeqs "\reg", "eax"
		GP_STACK_DELTA = 4
	.endif
	.ifeqs "\reg", "ebx"
		GP_STACK_DELTA = 4
	.endif
	.ifeqs "\reg", "ecx"
		GP_STACK_DELTA = 4
	.endif
	.ifeqs "\reg", "edx"
		GP_STACK_DELTA = 4
	.endif
	.ifeqs "\reg", "edi"
		GP_STACK_DELTA = 4
	.endif
	.ifeqs "\reg", "esi"
		GP_STACK_DELTA = 4
	.endif
	.ifeqs "\reg", "ebp"
		GP_STACK_DELTA = 4
	.endif
	.ifeqs "\reg", "esp"
		GP_STACK_DELTA = 4
	.endif
.endm

/* 确定对应的扩展寄存器名称 */
.macro GET_EREG reg
	.if GP_STACK_DELTA == 4
		EREG = \reg
	.elseif GP_STACK_DELTA == 2
		EREG = e\reg
	.else
	.ifeqs "\reg", "al"
		EREG = eax
	.endif
	.ifeqs "\reg", "ah"
		EREG = eax
	.endif
	.ifeqs "\reg", "bl"
		EREG = ebx
	.endif
	.ifeqs "\reg", "bh"
		EREG = ebx
	.endif
	.ifeqs "\reg", "cl"
		EREG = ecx
	.endif
	.ifeqs "\reg", "ch"
		EREG = ecx
	.endif
	.ifeqs "\reg", "dl"
		EREG = edx
	.endif
	.ifeqs "\reg", "dh"
		EREG = edx
	.endif
	.endif
.endm

/* 寄存器内容交换 */
.macro SWAP_IF_NEEDED reg
	.ifeqs "\reg", "ah"
		xchg ah, al
	.endif
	.ifeqs "\reg", "bh"
		xchg bh, bl
	.endif
	.ifeqs "\reg", "ch"
		xchg ch, cl
	.endif
	.ifeqs "\reg", "dh"
		xchg dh, dl
	.endif
.endm

/* 寄存器内容推送到mm5寄存器 */
.macro PUSHX reg
	GET_GP_STACK_DELTA \reg
	GET_EREG \reg
	SWAP_IF_NEEDED \reg

	movd mm7, EREG

	.if (GP_STACK_DELTA == 1)
		pand mm7, [mmx_000000ff]
	.elseif (GP_STACK_DELTA == 2)
		pand mm7, [mmx_0000ffff]
	.endif

	psllq mm5, (GP_STACK_DELTA * 0x08)
	por mm5, mm7

	SWAP_IF_NEEDED \reg
.endm

/* 推入当前 SP */
.macro PUSH_SP
	movd mm7, esp
	pand mm7, [mmx_0000ffff]
	psllq mm6, 0x10
	por mm6, mm7
.endm

/* 弹出当前 SP */
.macro POP_SP
	movd esp, mm6
	psrlq mm6, 0x10
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

/* 初始化定时器 */
init_timer:
	mov dx, RTC_ADDRESS							/* 将RTC（实时时钟）地址寄存器的地址赋给dx */
	mov al, 0x0A								/* 将要访问的RTC状态寄存器A的索引号0x0A赋给al */
	out dx, al									/* 将al的值输出到dx指定的端口，即向RTC地址寄存器写入0x0A */

	inc dx										/* 将dx加1，指向RTC数据寄存器的地址 */
	in al, dx									/* 从dx指定的端口读取数据到al，即读取RTC状态寄存器A的当前值 */
	and al, 0xf0								/* 将al与0xf0进行与运算，清零低四位，保留高四位的当前设置 */
	or al, 0x06									/* 将al与0x06进行或运算，设置低三位为011 */

	out dx, al									/* 将修改后的al值输出到dx指定的端口，即写回RTC状态寄存器A */
	dec dx										/* 将dx减1，重新指向RTC地址寄存器的地址 */

	mov al, 0x0B								/* 将要访问的RTC状态寄存器B的索引号0x0B赋给al */
	out dx, al									/* 将al的值输出到dx指定的端口，即向RTC地址寄存器写入0x0B */

	inc dx										/* 将dx加1，指向RTC数据寄存器的地址 */
	in al, dx									/* 从dx指定的端口读取数据到al，即读取RTC状态寄存器B的当前值 */
	or al, 0x40									/* 将al与0x40进行或运算，设置al的第6位为1 */
	out dx, al									/* 将修改后的al值输出到dx指定的端口，即写回RTC状态寄存器B */

/* 检测 CPU MMX */
check_mmx:
	xor edx, edx								/* 将edx寄存器清零 */
	xor eax, eax								/* 将eax寄存器清零 */
	inc eax										/* 将eax加1，eax=1 */

	/* 尝试 INTEL */
	cpuid										/* 执行CPUID指令，获取CPU相关信息，结果存放在eax、ebx、ecx、edx中 */
	shr edx, 0x10								/* 将edx右移16位，使第23位移到第7位 */
	and dl, 0x80								/* 将dl与0x80进行与运算，保留dl的第7位，即原edx的第23位 */
	jne 0f										/* 如果结果不为0，即检测到Intel MMX支持，跳转到标签0 */

	/* 尝试 AMD */
	xor edx, edx								/* 将edx寄存器清零 */
	mov eax, 0x80000001							/* 将0x80000001赋给eax */
	cpuid										/* 执行CPUID指令，获取CPU相关信息 */
	shr edx, 0x10								/* 将edx右移16位，使第22位移到第6位 */
	and dl, 0x40								/* 将dl与0x40进行与运算，保留dl的第6位，即原edx的第22位 */
	je clog										/* 如果结果为0，即没有MMX支持，跳转到halt标签，程序结束 */

0:	mov cx, 5									/* 宁愿保守一点，也不要冒险...，设置等待时间，这里设置为5，比2ms更保守 */
	ROM_CALL wait_timer							/* 调用等待定时器的函数，等待一段时间以满足ISA PnP规范的要求 */

	ROM_CALL lpc_init							/* 调用初始化LPC总线的函数，确保LPC总线可以正常使用 */

	mov di, offset pnp_superio_base				/* 将pnp_superio_base的偏移地址赋给di */
	movd mm0, edi								/* 保留一个指向superio基础的全局指针，将edi的值赋给mm0寄存器，以便后续使用 */

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

/* 初始化 LPC 总线 */
lpc_init:
	PCI_CONF_IN32 0, 0x1e, 0, 0x00				/* ICH DMI 到 PCI 桥接（总线 0，设备 30，功能 0）
	cmp eax, 0x244e8086							/* Intel ICH#（桌面）
	je lpc_intel
	cmp eax, 0x24488086							/* Intel ICH-M#（移动）
	je lpc_intel
	PCI_CONF_IN32 0, 0x14, 3, 0x00				/* AMD SBxx0 南桥（总线 0，设备 20，功能 3）
	cmp eax, 0x439d1002							/* SB7x0, SB9x0(?)
	je lpc_amd
	cmp eax, 0x438d1002							/* SB6x0, SB8x0
	je lpc_amd
	jmp sp

/* AMD LPC 初始化 */
lpc_amd:
	PCI_CONF_OUT32 0, 0x14, 3, 0x44, AMD_SB_IO	/* 配置 AMD SB IO
	PCI_CONF_OUT32 0, 0x14, 3, 0x48, 0x0020ff03	/* 0x2E/0x4E 和 0x80（POST 端口）转发，其他全部禁用
	jmp sp

/* Intel LPC 初始化 */
lpc_intel:
	PCI_CONF_OUT32 0, 0x1f, 0, 0x80, INTEL_SB_IO
	jmp sp

/* 定时器 */
wait_timer:
	mov dx, RTC_ADDRESS							/* 将RTC（实时时钟）地址寄存器的地址赋给dx */
	mov al, 0x0C								/* 将要访问的RTC寄存器C的索引号0x0C赋给al */
	out dx, al									/* 将al的值输出到dx指定的端口，即向RTC地址寄存器写入0x0C */

	inc dx										/* 将dx加1，指向RTC数据寄存器的地址 */
	in al, dx									/* 从dx指定的端口读取数据到al，即读取RTC寄存器C的当前值 */

0:	in al, dx									/* 再次从dx指定的端口读取数据到al，即读取RTC寄存器C的当前值 */
	and al, 0x40								/* 将al与0x40进行与运算，检查al的第6位，即PI位 */
	je 0b										/* 如果PI位为0（未设置），则跳转回标签0处继续等待 */
	loop 0b										/* 如果PI位已设置，说明已过1ms，循环计数器cx减1 */
	jmp sp										/* 跳转到sp标签处继续执行后续代码 */

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

/* 设置两个数据四元组 */
SETUP_STACK_DATA

/* Super I/O 芯片基址 */
pnp_superio_base:
	.word 0x2e, 0x4e

/* 开始字符串，用于显示 BIOS 启动信息 */
start_string:
	.string "ApexBIOS v0.01 Starting.\r\n"

/* 定义重置段，用于 BIOS 重置时跳转到入口点 */
.section reset, "ax"
	.byte 0xe9									/* 跳转指令的 opcode */
	.int entry - ( . + 2 )						/* 计算跳转偏移量 */
	.align 16, 0xff								/* 16 字节对齐，填充 0xff */
