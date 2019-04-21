.intel_syntax noprefix
.text
.code16

#jmp to IPL(generary : EB 3E)
jmp entry
.byte   0x90
.ascii "IPL     " # name string of OEM (8byte)
.word   512 # sector size(should be 512 byte)
.byte   1 # clustor size (should be 1 sector)
.word   1 # start sector of FAT(normally 1 sector)
.byte   2 # number of FAT (should be 2)
.word   224 # size of root directory (normally 224)
.word   2880 # size of drive(should be2880 sector)
.byte   0xf0 # media type(should be 0xf0)
.word   9 # length of FAT area(should be 9 sector)
.word   18 # number of sector per track (should be 18 sector)
.word   2 # number of head(should be 2)
.int    0 # partion(should be 0 if not use partion)
.int    2880 # size of dirve
.byte   0,0,0x29 # unknown(physical drive number, reserved, sigunature(if set 0x29, following 3 data is used)
.int    0xffffffff # maybe serial number ofvolume
.ascii  "UROBOROS-OS" # disk name(11byte)
.ascii  "FAT12   " # name of format(8byte)
.skip   18,0 # padding?

# 62 byte地点
entry:
    mov ax, 0
    mov ss, ax
    mov sp, 0x7c00
    mov ds, ax
    mov es, ax
    
    mov si, offset msg

putloop:
    mov al, [si]
    add si, 1
    cmp al, 0
    je fin
    
    mov ah, 0x0e
    mov bx, 15
    int 0x10
    jmp putloop

fin:
    hlt
    jmp fin

msg:
    .ascii "\nhello, world\n\n"

#end of boot sector(must be 0x55, 0xaa)
.org 0x01fe
.byte 0x55, 0xaa

