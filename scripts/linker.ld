/*
 *		linker.ld
 *		针对 BIOS 所写的链接脚本
 *
 *		基于 GPL-3.0 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

OUTPUT_FORMAT(binary)
OUTPUT_ARCH(i386:i386)

SECTIONS {
    . = 0xF0060;
    
    .text : {
        *(.text*)
    }
}
