BITS 16

;jmp to IPL(generary : EB 3E)
jmp entry
DB   0x90
DB "IPL     " ; name string of OEM (8byte)
DW   512 ; sector size(should be 512 byte)
DB   1 ; clustor size (should be 1 sector)
DW   1 ; start sector of FAT(normally 1 sector)
DB   2 ; number of FAT (should be 2)
DW   224 ; size of root directory (normally 224)
DW   2880 ; size of drive(should be2880 sector)
DB   0xf0 ; media type(should be 0xf0)
DW   9 ; length of FAT area(should be 9 sector)
DW   18 ; number of sector per track (should be 18 sector)
DW   2 ; number of head(should be 2)
DD    0 ; partion(should be 0 if not use partion)
DD    2880 ; size of dirve
DB   0,0,0x29 ; unknown(physical drive number, reserved, sigunature(if set 0x29, following 3 data is used)
DD    0xffffffff ; maybe serial number ofvolume
DB  "UROBOROS-OS" ; disk name(11byte)
DB  "FAT12   " ; name of format(8byte)
TIMES 18 DB 0

; 62 byte地点
entry:
    mov ax, 0
    mov ss, ax
    mov sp, 0x7c00
    mov ds, ax
    mov es, ax
    
    mov si, msg

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
    DB `\nhello, world\n\n\0`

;end of boot sector(must be 0x55, 0xaa)
TIMES 0x01fe-($-$$) DB 0
DB 0x55, 0xaa

