BITS 16

;ブート中の仮SS
;フロッピーの読み込みはESを0x29ずつ加算して読んでいったので、ESの値をSSに代入して、フロッピーの中身のすぐ後ろをstackにする
;0x7e00から10トラック分ロードしたとしたら(0x7e00 + (512*18*10)) = 0x1e600
init_sp:
    mov ax, es
    mov ss, ax
    mov sp, 0x00

boot:
    mov si, msg
    call bios_putstr
    
enable_a20:
    
    
fin:
    hlt
    jmp fin

bios_putstr:
.loop:
    mov al, [si]
    cmp al, 0
    je .end
    add si, 1
    
    mov ah, 0x0e
    mov bx, 15
    int 0x10
    jmp .loop
.end:
    ret


msg:
    DB `!BOOT! start 2nd stage bootloader\r\n\0`

