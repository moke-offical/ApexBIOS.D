/*
 *		entry.s
 *		ApexBIOS 入口
 *
 *		基于 MIT 开源协议
 *		Copyright © 2020 ViudiraTech，保留最终解释权。
 */

.code16

.macro post_code value
	.ifnes "\value", "al"
		mov %eax, \value
		mov %al, %ah
	.endif
	out %al, $0x80
.endm

.section begin, "a"
	.ascii "APEXBIOS v0.20"
	.align 16

.section reset, "ax"
	.byte 0xe9
	.int entry - ( . + 2 )
	.align 16, 0xff

.section main, "ax"
.globl entry
entry:
	cli
	movl %eax, %ebp

	xorl %eax, %eax
	movl %eax, %cr3

	movw %cs, %ax
	shlw $4, %ax
	movw $nullidt_offset, %bx
	subw %ax, %bx
	lidt %cs:(%bx)
	movw $gdtptr_offset, %bx
	subw %ax, %bx
	lgdtl %cs:(%bx)
1:
	movl %cr0, %eax
	andl $0x7FFAFFD1, %eax
	orl	$0x60000001, %eax
	movl %eax, %cr0
2:
	movl %ebp, %eax
	ljmpl $0x08, $protected_mode_entry

gdt:
	.word 0x0000, 0x0000
	.byte 0x00, 0x00, 0x00, 0x00
	.word 0xffff, 0x0000
	.byte 0x00, 0x9b, 0xcf, 0x00
	.word 0xffff, 0x0000
	.byte 0x00, 0x93, 0xcf, 0x00
	.word 0xffff, 0x0000
	.byte 0x00, 0x9b, 0xaf, 0x00
gdt_end:

.align 4

.globl gdtptr
gdtptr:
	.word gdt_end - gdt -1
	.long gdt

.globl nullidt
nullidt:
	.word 0
	.long 0
	.word 0

.code32

.globl protected_mode_entry
protected_mode_entry:
	movl %eax, %ebp

	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
	xor	%ax, %ax
	movw %ax, %fs
	movw %ax, %gs

	movl %ebp, %eax
	movd %eax, %mm0

__timestamp:
	rdtsc
	movd %eax, %mm1
	movd %edx, %mm2

enable_sse:
	mov %cr4, %eax
	or $(1 <<  9), %ax
	mov %eax, %cr4

done:
	hlt
	jmp done
