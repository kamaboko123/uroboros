BITS 16

READ_TRACKS EQU 10

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
    je load_osl
    
    mov ah, 0x0e
    mov bx, 15
    int 0x10
    jmp putloop

load_osl:
    ;bufferの初期位置(0x7e00に置きたいので、esは0x7e00/16 = 0x07e0する)
    mov ax, 0x07e0
    mov es, ax
    mov cl, 1 ; ttssssss t:トラック上位2bit(1始まり) s:セクタ(1始まり)
    mov dh, 0 ; ヘッド(0始まり)
    mov ch, 0 ; トラック下位8bit
    
osl_read_loop:
    mov bx, 0
    mov ah, 0x2 ;読み込み
    mov al, 1 ; 読み込むセクタ数
    mov dl, 0 ; ドライブ番号(A)
    
    int 0x13
    jc error
    
    ;0x0200足してbufferを512byteすすめる(esに入れた値の16倍されるので、esに足すのは0x20)
    mov ax, es
    add ax, 0x0020
    mov es, ax
    
    ;1セクタ分進める
    add cl, 1
    
    ;18以下なら次を読む
    cmp cl, 18
    jbe osl_read_loop
    
    mov cl, 1 ; セクタを戻す
    add dh, 1 ; ヘッド
    cmp dh, 2
    jb osl_read_loop ; 2未満なら次
    
    mov dh, 0 ;ヘッドを戻す
    add ch, 1 ;トラック
    cmp ch, READ_TRACKS
    jb osl_read_loop
    
    ;コピー先は0x7e00(0x7c00 - 0x7dffはブートセクタがロードされている)から
    ;boot.binが置かれる位置は0x4200(FAT12の最初のデータ格納位置)である
    ;0x7e00 + 0x4200 = 0xc000
    jmp 0xc000

error:
    mov si, err_msg
error_loop:
    mov al, [si]
    add si, 1
    cmp al, 0
    je fin
    
    mov ah, 0x0e
    mov bx, 15
    int 0x10
    jmp error_loop
   

fin:
    hlt
    jmp fin

buf:
    DB 0x00

msg:
    DB `\r\nIPL running!\r\nstart to copy 2nd stage bootloader to memory...\r\n\r\n\0`

err_msg:
    DB `error\0`

;end of boot sector(must be 0x55, 0xaa)
TIMES 0x01fe-($-$$) DB 0
DB 0x55, 0xaa

