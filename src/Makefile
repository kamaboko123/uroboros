
CC = gcc
CFLAGS = -nostdlib -fno-builtin -fno-stack-protector -fno-common -fno-pic -m32 -O0 -Wall -std=c11
AS = nasm
ASFLAGS = -f elf -O0
LD = ld

LDFLAGS = -m elf_i386

INCLUDE = -I include

TARGET_DIR = bin

#IPL
IPL = $(TARGET_DIR)/ipl.bin
IPL_SRC = boot/ipl.s
IPL_LS = scripts/ipl.lds

#boot loader
OSL = $(TARGET_DIR)/boot.bin
OSL_SRC = boot/boot.s
OSL_LS = scripts/boot.lds

KERNEL_SRC_DIR = kernel
KERNEL_SRC = $(wildcard $(KERNEL_SRC_DIR)/*.c)
KERNEL_OBJ_DIR = $(TARGET_DIR)/c
KERNEL_OBJ = $(addprefix $(KERNEL_OBJ_DIR)/, $(notdir $(KERNEL_SRC:.c=.o)))

KERNEL_ASM_SRC_DIR = kernel/asm
KERNEL_ASM_SRC = $(wildcard $(KERNEL_ASM_SRC_DIR)/*.s)
KERNEL_ASM_OBJ_DIR = $(TARGET_DIR)/asm
KERNEL_ASM_OBJ = $(addprefix $(KERNEL_ASM_OBJ_DIR)/, $(notdir $(KERNEL_ASM_SRC:.s=.o)))

# release or debug
RUST_BUILD_MODE = release
RUST_LIB_NAME= liburoboros_rs
cargo_build_mode = $(shell if [ "$(RUST_BUILD_MODE)" = "release" ]; then echo --release; fi)
RUST_BUILD_CMD = cargo build --lib -Zbuild-std=std,panic_abort $(cargo_build_mode)
KERNEL_RUST_SRC_DIR = kernel/rust
KERNEL_RUST_SRC = $(wildcard $(KERNEL_RUST_SRC_DIR)/src/*.rs)
KERNEL_RUST_OBJ_DIR = $(TARGET_DIR)/rust
# KERNEL_RUST_OBJ = $(KERNEL_RUST_OBJ_DIR)/$(RUST_LIB_NAME).a
# Rustで書いたコードの組み込みは一旦やめる
KERNEL_RUST_OBJ=


KERNEL = $(TARGET_DIR)/kernel.bin
KERNEL_MEMMAP = $(TARGET_DIR)/kernel.map

KERNEL_LS = scripts/kernel.lds

IMG = $(TARGET_DIR)/uroboros.img

KERNEL_IMG = $(TARGET_DIR)/kernel.sys

FONT_DIR = font
FONT = font/bin/hankaku.o

QEMU = qemu-system-x86_64

BOCHS_CONF = bochs.conf

APP_DISK_IMG = app/bin/app_disk.img


all : clean $(IMG)

$(IPL): $(IPL_SRC) $(IPL_LDS)
	mkdir -p $(TARGET_DIR)
	$(AS) $(ASFLAGS) -o $@.o $(IPL_SRC)
	$(LD) $(LDFLAGS) -T $(IPL_LS) -o $@ $@.o

$(OSL) : $(OSL_SRC) $(OSL_LDS)
	mkdir -p $(TARGET_DIR)
	$(AS) $(ASFLAGS) -o $@.o $(OSL_SRC)
	$(LD) $(LDFLAGS) -T $(OSL_LS) -o $@ $@.o

$(FONT) :
	cd $(FONT_DIR); make

$(KERNEL_OBJ_DIR)/%.o : $(KERNEL_SRC_DIR)/%.c
	mkdir -p $(KERNEL_OBJ_DIR)
	$(CC) -c $(CFLAGS) $(INCLUDE) -o $@ $^
	@echo $(KERNEL_ASM_OBJ)

$(KERNEL_ASM_OBJ_DIR)/%.o : $(KERNEL_ASM_SRC_DIR)/%.s
	mkdir -p $(KERNEL_ASM_OBJ_DIR)
	$(AS) $(ASFLAGS) -o $@ $^

$(KERNEL_RUST_OBJ) : $(KERNEL_RUST_SRC)
	mkdir -p $(KERNEL_RUST_OBJ_DIR)
	cd $(KERNEL_RUST_SRC_DIR); $(RUST_BUILD_CMD)
	cd $(CURDIR)
	cp $(KERNEL_RUST_SRC_DIR)/target/i686-unknown-none/$(RUST_BUILD_MODE)/$(RUST_LIB_NAME).a $@

$(KERNEL) : $(KERNEL_OBJ) $(KERNEL_ASM_OBJ) $(FONT) $(KERNEL_RUST_OBJ)
	$(LD) $(LDFLAGS) -e Main -T$(KERNEL_LS) -Map $(KERNEL_MEMMAP) -o $(TARGET_DIR)/kernel.elf $(KERNEL_OBJ) $(KERNEL_ASM_OBJ) $(FONT) $(KERNEL_RUST_OBJ)
	objcopy -O binary $(TARGET_DIR)/kernel.elf $@

$(KERNEL_IMG) : $(KERNEL) $(OSL)
	cat $(OSL) $(KERNEL) > $@

$(IMG) : $(IPL) $(KERNEL_IMG)
	mformat -f 1440 -B $(IPL) -C -i $@ ::
	mcopy $(KERNEL_IMG) -i $@ ::

$(APP_DISK_IMG) :
	cd app; make

run: $(IMG) $(APP_DISK_IMG)
	$(QEMU) -m 128M -drive format=raw,file=$(IMG),if=floppy,index=0 -drive format=raw,file=$(APP_DISK_IMG),if=floppy,index=1 -serial stdio

debug: $(IMG)
	$(QEMU) -m 128M -drive format=raw,file=$(IMG),if=floppy,index=0 -drive format=raw,file=$(APP_DISK_IMG),if=floppy,index=1 -serial stdio -S -gdb tcp::10000

run_bochs:
	bochs -f $(BOCHS_CONF)

debug_bochs:
	bochs -f $(BOCHS_CONF)

clean:
	rm -rf $(TARGET_DIR)
	cd app; make clean

clean_all:
	rm -rf $(TARGET_DIR)
	cd $(FONT_DIR); make clean
	cd $(FONT_DIR); make clean
	cd $(KERNEL_RUST_SRC_DIR); cargo clean
