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
    jmp fin
    
enable_a20:
    ;A20ライン有効化
    mov si, msg_a20_0
    call bios_putstr
    
    ;キーボードコントローラ使うの面倒なので、INT 0x15でA20ラインの有効化
    ;http://www.delorie.com/djgpp/doc/rbinter/ix/15/24.html
    ;https://wiki.osdev.org/A20_Line
    
    mov ax, 0x2403 ;まずはA20ラインが使えるか確認
    int 0x15
    jb error_a20 ;成功するとCFがクリアされる
    cmp ah, 0
    jnz error_a20 ;成功するとAHは0になる
    
    mov si, msg_a20_1
    call bios_putstr
    
    mov ax, 0x2402 ;A20ラインの状態の取得
    int 0x15
    jb error_a20 ;取得失敗
    cmp ah, 0
    jnz error_a20 ;取得失敗
    
    mov si, msg_a20_2
    call bios_putstr
    
    cmp al,1
    jz a20_enabled ;すでに有効
    
    mov si, msg_a20_3
    call bios_putstr
    
    mov ax, 0x2401 ;A20 enable
    int 0x15
    jb error_a20 ;失敗
    cmp ah, 0
    jnz error_a20 ;失敗
    jmp init_gdt
    
a20_enabled:
    mov si, msg_a20_4
    call bios_putstr
init_gdt:
    ;GDT設定
    mov si, msg_gdt_0
    call bios_putstr
    
enable_protect_mode:
    ;CR0設定(プロテクトモード)
    
    jmp pipeline_flush

;ここからプロテクトモード(32bit)
pipeline_flush:
    BITS 32

fin:
    hlt
    jmp fin

msg_a20_0:
    DB `!BOOT! A20 Line\r\n\0`
msg_a20_1:
    DB `BIOS support enable A20 by INT 15h.\r\n\0`
msg_a20_2:
    DB `Success to get A20 status : \0`
msg_a20_3:
    DB `Disable\r\n Try to enable A20 Line.\r\n\0`
msg_a20_4:
    DB `Enable\r\n\0`

msg_gdt_0:
    DB `!BOOT! GDT initialize...\r\n\0`

error_a20:
    mov si, msg_error_a20
    call bios_putstr
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

msg_error_a20:
    DB `Failed to enable A20 Line.\0`

msg:
    DB `!BOOT! start 2nd stage bootloader\r\n\0`

