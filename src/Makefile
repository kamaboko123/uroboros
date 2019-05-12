
CC = gcc
CFLAGS = -nostdlib -fno-builtin -fno-stack-protector -m32 -O0 -Wall -std=c11
AS = nasm
ASFLAGS = -f elf
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
KERNEL_OBJ = $(addprefix $(TARGET_DIR)/, $(notdir $(KERNEL_SRC:.c=.o)))
KERNEL_LS = scripts/kernel.lds

ASMLIB = $(TARGET_DIR)/asmlib.o
ASMLIB_SRC = kernel/asm/asmlib.s

KERNEL = $(TARGET_DIR)/kernel.bin
IMG = $(TARGET_DIR)/uroboros.img

KERNEL_MAIN = $(TARGET_DIR)/main.o
KERNEL_MAIN_SRC = main.c

KERNEL_IMG = $(TARGET_DIR)/kernel.sys

FONT_DIR = font
FONT = font/bin/hankaku.o

QEMU = qemu-system-x86_64

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

$(TARGET_DIR)/%.o : $(KERNEL_SRC_DIR)/%.c
	mkdir -p $(TARGET_DIR)
	$(CC) -c $(CFLAGS) $(INCLUDE) -o $@ $^

$(KERNEL) : $(KERNEL_OBJ) $(ASMLIB) $(FONT) $(KERNEL_MAIN)
	$(LD) $(LDFLAGS) -T$(KERNEL_LS) -o $@ $(KERNEL_MAIN) $(KERNEL_OBJ) $(ASMLIB) $(FONT)

$(KERNEL_IMG) : $(KERNEL) $(OSL)
	cat $(OSL) $(KERNEL) > $@

$(KERNEL_MAIN): $(KERNEL_MAIN_SRC)
	$(CC) -c $(CFLAGS) $(INCLUDE) -o $@ $^

$(ASMLIB): $(ASMLIB_SRC)
	$(AS) $(ASFLAGS) -o $@ $^

$(IMG) : $(IPL) $(KERNEL_IMG)
	mformat -f 1440 -B $(IPL) -C -i $@ ::
	mcopy $(KERNEL_IMG) -i $@ ::

run: $(IMG)
	$(QEMU) -m 128M -drive format=raw,file=$(IMG),if=floppy

clean:
	rm -rf $(TARGET_DIR)
