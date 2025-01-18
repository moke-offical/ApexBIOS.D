/*
 *		linker.ld
 *		针对 BIOS 所写的链接脚本
 *
 *		基于 MIT 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

OUTPUT_ARCH(i8086)

main_address = 4096M - 4K;

gdtptr_offset = gdtptr & 0xffff;
nullidt_offset = nullidt & 0xffff;

MEMORY {
	ROM (rx) : org = 4096M - 512K, len = 512K
}

SECTIONS {
	ENTRY(entry)

	.begin : {
		*(begin)
	} >ROM
	.main main_address : {
		*(main)
	}
	.reset 4096M - 0x10 : {
		*(reset)
	}
	.igot 0 : {
		*(.igot.plt)
	}
}
