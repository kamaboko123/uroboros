BITS 16

boot:
    mov si, msg
print_boot_msg:
    mov al, [si]
    add si, 1
    cmp al, 0
    je fin
    
    mov ah, 0x0e
    mov bx, 15
    int 0x10
    jmp print_boot_msg

fin:
    hlt
    jmp fin

msg:
    DB `!BOOT! start 2nd stage bootloader\r\n\0`

