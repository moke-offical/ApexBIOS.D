# =====================================================
#		Makefile
#		APEX BIOS 编译脚本
#
#		基于 GPL-3.0 开源协议
#		Copyright © 2020 ViudiraTech，保留最终解释权。
# =====================================================

ifeq ($(VERBOSE), 1)
  V=
  Q=
else
  V=@printf "\033[1;32m[Build]\033[0m $@ ...\n";
  Q=@
endif

# 目录和文件
SRCDIR		:= src
OBJDIR		:= build
SPTDIR      := scripts
LIBDIR      := $(SRCDIR)/lib
ICLDIR      := $(SRCDIR)/include

rwildcard	= $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
reverse		= $(if $(1),$(call reverse,$(wordlist 2,$(words $(1)),$(1)))) $(firstword $(1))
SRC			= $(call rwildcard,$(SRCDIR),*.c)
OBJS		= $(call reverse,$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC)))

OUTFILE		= bios.rom

# 编译器、工具及其选项
ASM			= nasm
ASMFLAGS	= -fbin

CC			= gcc
CFLAGS		= -m32 -ffreestanding -fno-pic -w -Os -fno-stack-protector

LD			= ld
LDFLAGS		= -nostdlib -static -T

QEM         = qemu-system-x86_64
QEMFLAGS    = -serial stdio -vga none -device ramfb -m 256M

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(V)$(CC) $(CFLAGS) -I ./$(ICLDIR)/ -c $^ -o $@

%.fmt: %
	$(Q)printf "\033[1;32m[Format]\033[0m $< ...\n"
	$(Q)clang-format -i $<

# 伪目标
.PHONY: format build clean run

# 格式化代码
format: $(C_SOURCES:%=%.fmt) $(S_SOURCES:%=%.fmt) $(HEADERS:%=%.fmt)
	$(Q)printf "\033[1;32m[Done]\033[0m Code Format complete.\n\n"

build: $(OBJS)
    # 链接文件生成可执行文件
	@$(LD) $(LDFLAGS) $(SPTDIR)/linker.ld $(OBJS) $(LIBDIR)/logo.lib $(LIBDIRDIR)/EPA.lib -o $(OBJDIR)/c_entry.bin
	@printf "\n\033[1;32m[LINK]\033[0m Linking..."

    # 汇编文件生成 ROM 文件
	@stat -L -c "" $(OBJDIR)/c_entry.bin
	@$(ASM) $(ASMFLAGS) $(SRCDIR)/entry16.asm -o $(OUTFILE)
	@printf "\033[1;32m[Done]\033[0m Compilation complete.\n\n"

# 清理目标
clean:
	@rm -rf $(OBJDIR) $(OUTFILE)
	@printf "\033[1;32m[Done]\033[0m Clean done.\n\n"

# 运行 BIOS ROM 文件
run:
	$(QEM) -bios $(OUTFILE) $(QEMFLAGS)
