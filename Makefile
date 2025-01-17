# 编译器和工具
ASM			:= as
LD			:= ld
OBJCOPY		:= objcopy

# 编译和链接选项
SFLAGS		:= --32
LDFLAGS		:= -mi386pe

# 目录和文件
LINKFILE	:= ./scripts/linker.ld
SRCDIR		:= ./src/
OBJS		:= $(SRCDIR)entry.o

# 伪目标
.PHONY: all clean run

# 默认目标
all: bios.rom

# 清理目标
clean:
	@echo "Cleaning..."
	@rm -f -v $(SRCDIR)*.o $(SRCDIR)*.out bios.rom xMemLayout.map
	@echo

# 汇编文件生成目标文件
$(SRCDIR)%.o: $(SRCDIR)%.s Makefile
	@echo "[AS]  $<"
	@$(ASM) $(SFLAGS) $< -o $@

# 链接目标文件生成可执行文件
$(SRCDIR)bios.out: $(OBJS) $(LINKFILE)
	@echo "[LD]  $@"
	@$(LD) $(LDFLAGS) -T$(LINKFILE) -o $@ $(OBJS) -Map xMemLayout.map

# 生成 ROM 文件
bios.rom: $(SRCDIR)bios.out
	@echo "[ROM] $@"
	@$(OBJCOPY) -O binary -j .begin -j .main -j .reset --gap-fill=0xff $< $@
	@echo

# 运行 BIOS ROM 文件
run:
	qemu-system-i386 -bios bios.rom -d in_asm
