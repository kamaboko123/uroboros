# Makefile

CC = gcc
CFLAGS = -nostdlib -fno-builtin -fno-stack-protector -m32
LD = ld

COMMON = Makefile

TARGET_DIR = bin
IPL = $(TARGET_DIR)/ipl.bin
IPL_SRC = ipl.s
IPL_LS = ipl.lds

IMG = $(TARGET_DIR)/uroboros.img

QEMU = qemu-system-x86_64

all : $(IMG) $(COMMON)

$(IPL): $(IPL_SRC) $(IPL_LDS)
	mkdir -p $(TARGET_DIR)
	$(CC) -c $(CFLAGS) -o $@.o -T $(IPL_LS) $(IPL_SRC)
	$(CC) $(CFLAGS) -o $@ -T $(IPL_LS) $(IPL_SRC)

$(IMG): $(IPL)
	mformat -f 1440 -B $(IPL) -C -i $@ ::

run: $(IMG)
	$(QEMU) -drive format=raw,file=$(IMG),if=floppy
debug: $(IMG)
	$(QEMU) -drive format=raw,file=$(IMG),if=floppy -gdb tcp::10000 -S

clean:
	rm -rf $(TARGET_DIR)

