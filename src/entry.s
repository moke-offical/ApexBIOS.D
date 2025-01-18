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
1:	movl %cr0, %eax
	andl $0x7FFAFFD1, %eax
	orl	$0x60000001, %eax
	movl %eax, %cr0
2:	movl %ebp, %eax
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

protected_mode_entry:
	movd %eax, %mm0

__timestamp:
	rdtsc
	movd %eax, %mm1
	movd %edx, %mm2

enable_sse:
	mov %cr4, %eax
	or $(1 <<  9), %ax
	mov %eax, %cr4

init_superio:
	mov $0x2e, %dx
	in %dx, %al
	in %dx, %al
	mov $superio_conf, %si
	mov $(superio_conf_end - superio_conf) / 2, %cx
.write_superio_conf:
	mov (%si), %ax
	mov $0x2e, %dx
	out %al, %dx
	inc %dx
	xchg %al, %ah
	out %al, %dx
	add $0x02, %si
	loop .write_superio_conf

init_serial:
	mov $serial_conf, %si
	mov $(serial_conf_end - serial_conf) / 2, %cx
.write_serial_conf:
	mov (%si), %ax
	mov $0x3f8, %dx
	add %dl, %al
	mov %ah, %al
	out %al, %dx
	add $0x02, %si
	loop .write_serial_conf

print_start:
	movl $start, %esi
	call print_str

	movl $copy, %esi
	call print_str

done:
	hlt
	jmp done

putchar:
	mov $0x3f8 + 0x05, %dx
	mov %al, %ah
.tx_wait:
	in %dx, %al
	and $0x20, %al
	jz .tx_wait
	mov $0x3f8 + 0x00, %dx
	mov %ah, %al
	out %al, %dx
	ret

print_str:
	movb (%esi), %al
	test %al, %al
	jz .end_print
	call putchar
	inc %esi
	jmp print_str
.end_print:
	ret

superio_conf:
	.byte 0x00, 0x0f
	.byte 0x01, 0x10
	.byte 0x02, 0x00
superio_conf_end:

serial_conf:
	.byte 0x04, 0x00
	.byte 0x02, 0x07
	.byte 0x03, 0x80
	.byte 0x00, 0x01
	.byte 0x01, 0x00
	.byte 0x03, 0x03
serial_conf_end:

start:
	.string "ApexBIOS v0.01\r\n"

copy:
	.string "Copyright (c) 2020 ViudiraTech.\r\n"
