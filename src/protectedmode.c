/*
 *		protectedmode.c
 *		保护模式后的C语言入口
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

#include "serial.h"
#include "pic.h"
#include "memory.h"
#include "ramfb.h"
#include "ps2.h"
#include "ahci.h"
#include "floppy.h"
#include "speaker.h"
#include "cpuid.h"
#include "atapio.h"
#include "eis.h"
#include "bitmap.h"
#include "logo.lib.h"
#include "EPA.lib.h"

/* 来自RoofAlan */
int inttostr(uint32_t num);

/* BIOS保护模式入口 */
extern void C_ENTRY(void)
{
	/* 初始化SSE指令集 */
	if (sse_detect())
		sse_init();

	/* 初始化AVX指令集 */
	if (avx_detect())
		avx_init();

	/* 初始化FPU */
	if (fpu_detect()) 
		fpu_init();

	/* 初始化PIC */
	pic_init(0x08, 0x70);

	/* 初始化PS/2 */
	if (PS2_detect())
		PS2_init();

	/* 初始化软盘驱动器 */
	if (floppy_get_drives())
		floppy_init(FLOPPY_DRIVE_MASTER);

	/* 初始化VGA */
	RAMFB_init(640, 480);
	RAMFB_set_color(0xA8A8A8);

	/* ↓BIOS界面↓ */
	bmp_analysis((Bmp *)logo, 8, 11, 1);
	bmp_analysis((Bmp *)EPA, 480, 16, 1);

	RAMFB_put_str("\n         Apex Modular BIOS v0.01.\n");
	RAMFB_put_str("         Copyright (C) 2020 ViudiraTech.\n\n");

	RAMFB_put_str("Chipset: ");

	if (IS_Q35)
		RAMFB_put_str("Q35\n\n");
	else if (IS_I440FX)
		RAMFB_put_str("i440FX\n\n");
	else if (IS_MICROVM){
		RAMFB_set_color(0xFF0000);
		RAMFB_put_str("MicroVM (Unsupported)\n\n");
		while(1);
	} else {
		RAMFB_set_color(0xFF0000);
		RAMFB_put_str("Unknown\n\n");
		while(1);
	}

	RAMFB_put_str("CPU: ");
	RAMFB_put_str(cpu_get_brand());
	RAMFB_put_str("\n");

	RAMFB_put_str("Memory: ");
	RAMFB_put_str((char*)inttostr(ram_detect()));
	RAMFB_put_str("K\n\n");

	RAMFB_put_str("Detecting AHCI   ... ");
	if (ahci_detect_controller())
		RAMFB_put_str("AHCI Controller present.\n");
	else
		RAMFB_put_str("AHCI Controller Not present.\n");

	RAMFB_put_str("Detecting IDE    ... ");
	if (ata_controller_detect())
		RAMFB_put_str("IDE Controller present.\n");
	else
		RAMFB_put_str("IDE Controller Not present.\n");

	RAMFB_put_str("Detecting Floppy ... ");
	if (floppy_get_drives()) {
		RAMFB_put_str("Floppy Number: ");
		RAMFB_put_str((char*)inttostr(floppy_get_drives()));
		RAMFB_put_str("\n\n");
	} else {
		RAMFB_put_str("Floppy Not present.\n\n");
	}

	/* 尝试引导 */
	if (ata_controller_detect() && ata_driver_detect()) {
		RAMFB_put_str("Booting from Hard Disk...\n");
		memset((void*)0x7C00, 0, 512);
		ata_read_lba(0, (void*)0x7c00);
		if (*((uint16_t*)0x7dfe) == 0xAA55) {
			void (*boot)(void) = (void (*)(void))(0x7c00);
			boot();
		} else {
			RAMFB_put_str("Boot failed: not bootable disk.\n");
		}
	} else {
		RAMFB_put_str("No bootable device.\n");
	}

	while(1) {
		__asm__ volatile ("hlt");
	}
}
