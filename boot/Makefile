# Makefile

CC = gcc
CFLAGS = -nostdlib -fno-builtin -fno-stack-protector -m32 -O0
AS = nasm
ASFLAGS = -f elf
LD = ld

COMMON = Makefile

TARGET_DIR = bin

#IPL
IPL = $(TARGET_DIR)/ipl.bin
IPL_SRC = ipl.s
IPL_LS = ipl.lds

#boot loader
OSL = $(TARGET_DIR)/boot.bin
OSL_SRC = boot.s
OSL_LS = boot.lds

#kernel
KERNEL = $(TARGET_DIR)/kenel.sys
KERNEL_SRC = kernel.c
KERNEL_LS = kernel.lds
KERNEL_ENTRY = Main

IMG = $(TARGET_DIR)/uroboros.img

QEMU = qemu-system-x86_64

all : $(IMG) $(COMMON)

$(IPL): $(IPL_SRC) $(IPL_LDS)
	mkdir -p $(TARGET_DIR)
	$(AS) $(ASFLAGS) -o $@.o $(IPL_SRC)
	$(LD) -m elf_i386 -T $(IPL_LS) -o $@ $@.o

$(OSL) : $(OSL_SRC) $(OSL_LDS)
	mkdir -p $(TARGET_DIR)
	$(AS) $(ASFLAGS) -o $@.o $(OSL_SRC)
	$(LD) -m elf_i386 -T $(OSL_LS) -o $@ $@.o

$(KERNEL): $(KERNEL_SRC) $(KERNEL_LDS)
	mkdir -p $(TARGET_DIR)
	$(CC) -c $(CFLAGS) -o $@.o $(KERNEL_SRC)
	$(LD) -m elf_i386 -e $(KERNEL_ENTRY) -T $(KERNEL_LS) -o $@.bin $@.o
	cat $(OSL) $@.bin > $@

$(IMG): $(IPL) $(OSL) $(KERNEL)
	mformat -f 1440 -B $(IPL) -C -i $@ ::
	mcopy $(KERNEL) -i $@ ::

run: $(IMG)
	$(QEMU) -drive format=raw,file=$(IMG),if=floppy
debug: $(IMG)
	$(QEMU) -drive format=raw,file=$(IMG),if=floppy -gdb tcp::10000 -S

clean:
	rm -rf $(TARGET_DIR)

