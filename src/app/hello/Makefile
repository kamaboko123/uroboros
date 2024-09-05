#Makefile

CC = gcc
CFLAGS = -nostdlib -m32 -fno-builtin -O0 -fno-common
LDFLAGS = -m elf_i386
LS = ../lib/app.lds

LIB = $(TARGET_DIR)/lib/applib.o

TARGET_DIR = bin
DIST_DIR = $(TARGET_DIR)/dist
TMP_DIR = $(TARGET_DIR)/tmp

# target file
APP = $(DIST_DIR)/hello
APP_SRC = hello.c

INCLUDE = -I ../lib/include

$(APP): $(APP_SRC)
	mkdir -p $(DIST_DIR)
	mkdir -p $(TMP_DIR)
	$(CC) -c $(CFLAGS) $(INCLUDE) -o $(TMP_DIR)/$(APP_SRC:.c=.o) $(APP_SRC)
	ld $(LDFLAGS) -o $(APP) -e main -T $(LS) $(STDLIB) $(TMP_DIR)/$(APP_SRC:.c=.o)

clean:
	rm -rf $(TARGET_DIR)
