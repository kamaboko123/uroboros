[BITS 16]

;カーネルサイズを4で割って+1以上
KERNEL_SIZE EQU 1024 * 512

;ブート中の仮SS
;フロッピーの読み込みはESを0x29ずつ加算して読んでいったので、ESの値をSSに代入して、フロッピーの中身の後ろから8KBをstackにする
;0x7e00から10トラック分ロードしたとしたら(0x7e00 + (512*18*10)) + 64 * 1024 = 0x2e600
init_sp:
    mov ax, es
    add ax, 1024*8/16
    mov ss, ax
    mov sp, 0x00
    
boot:
    mov si, msg
    call bios_putstr
    
    cli
enable_a20:
    ;A20ライン有効化
    mov si, msg_a20_0
    call bios_putstr
    
    mov al, 0xff
    out 0x21, al
    nop
    out 0xa1, al
    
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
    mov si, msg_gdt_0
    call bios_putstr
    
    ;GDT設定
    lgdt [GDT]
    
init_video:
    ; 320 x 200 x 8bit-color
    mov al, 0x13
    mov ah, 0x00
    int 0x10

enable_protect_mode:
    ;CR0設定(プロテクトモード)
    ;ページングを使用しない(CR0の最上位bitを0, 最下位bitを1にする)
    mov eax, cr0
    and eax, 0x7fffffff
    or eax, 0x00000001
    mov cr0, eax
    
    jmp entry_protect_mode

entry_protect_mode:
    mov ax, 1*8
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    mov esi, kernel
    mov edi, 0x00100000
    mov ecx, KERNEL_SIZE/4
    call memcpy
    
    mov esp, 0x00031000

    ;GTD0のアドレスをカーネルに渡す
    mov eax, GDT0
    push eax
    ;戻りアドレス、Mainから戻ることはないので何でもいい(とりあえずエントリポイントにしておく)
    mov ebx, 0x00007c00
    push ebp

    jmp dword 2*8:0x00100000
_hlt:
    hlt
    jmp _hlt

memcpy: ;esiのアドレスからediのアドレスにコピー 4byteずつecx回コピー
    mov eax, [esi]
    add esi, 4
    mov [edi], eax
    add edi, 4
    sub ecx, 1
    jnz memcpy ;引き算結果が0でなければmemcpyへ
    ret

fin:
    hlt
    jmp fin

msg_a20_0:
    DB `!BOOT! A20 Line\r\n\0`
msg_a20_1:
    DB ` BIOS support enable A20 by INT 15h.\r\n\0`
msg_a20_2:
    DB ` Success to get A20 status : \0`
msg_a20_3:
    DB `Disable\r\n Try to enable A20 Line.\r\n\0`
msg_a20_4:
    DB `Enable\r\n\0`

msg_gdt_0:
    DB `!BOOT! GDT initialize...\0`

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


align 16
GDT0:
    DW 0x0000, 0x0000, 0x0000, 0x0000
    
    DW 0xffff, 0x0000, 0x9200, 0x00cf
    DW 0xffff, 0x0000, 0x9a00, 0x00cf
    DW 0xffff, 0x0000, 0xf200, 0x00cf
    DW 0xffff, 0x0000, 0xf800, 0x00cf
    
    ;GDT1(kernal データセグメント)
    ;DB 0xFF, 0xFF ; Segment Limit Low
    ;DW 0x0000 ; Segment Base Low
    ;DB 0x00 ; Segment Address Mid
    ;DB 10010010b ; Type(0-3)、S(4)、DPL(5-6)、P(7) カッコ内はビット
    ;DB 11001111b ; Segment Limit Hi(0-3)、AVL(4)、0(5)、D/B(6)、G(7)
    ;DB 0x00 ; Base Address Hi
    
    ;GDT2(kernel コードセグメント)
    ;DB 0xFF, 0xFF ; Segment Limit Low
    ;DW 0x0000 ; Segment Base Low
    ;DB 0x00 ; Segment Address Mid
    ;DB 10011010b ; Type(0-3)、S(4)、DPL(5-6)、P(7) カッコ内はビット
    ;DB 11001111b ; Segment Limit Hi(0-3)、AVL(4)、0(5)、D/B(6)、G(7)
    ;DB 0x00 ; Base Address Hi
    
    ;GDT3(user データセグメント)
    ;GDT4(user コードセグメント)

align 16
GDT:
    DW 8*5-1 ;GDTのサイズ - 1 [byte]
    DD GDT0

kernel:
