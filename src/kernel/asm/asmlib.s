extern int_handler_pit
extern int_handler_serial

;void lgdt(uint32_t gdtr)
global lgdt
lgdt:
    mov eax, [esp+4]
    ;jmp loop
    lgdt [eax]
    mov ax, 0x08
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x10:.lgdt_reload
.lgdt_reload:
    ret

;void lidt(uint32_t idtr)
global lidt
lidt:
    mov eax, [esp+4]
    lidt [eax]
    ret


global loop
loop:
    jmp loop

;void io_hlt(void)
global io_hlt
io_hlt:
    hlt
    ret

;void io_cli(void)
global io_cli
io_cli:
    cli
    ret

;void io_sti(void)
global io_sti
io_sti:
    sti
    ret

;void io_wait(void)
global io_wait
io_wait:
    mov eax, 0x0000000000
    out 0x80, al
    ret

;void io_out8(uint32_t port, uint32_t data)
global io_out8
io_out8:
    mov edx, [esp+4]
    mov al, [esp+8]
    out dx, al
    ret

;uint8_t io_in8(uint32_t port)
global io_in8
io_in8:
    mov edx, [esp+4]
    mov eax, 0
    in al, dx
    ret

;int io_load_eflags(void)
global io_load_eflags
io_load_eflags:
    pushf
    pop eax
    ret

;void io_store_eflags(uint32_t flags)
global io_store_eflags
io_store_eflags:
    mov eax, [esp+4]
    push eax
    popf
    ret


ORG_STACK_BASE EQU 0x00031000

;void enable_paging(unsigned int pdt_ddr, unsigned int new_stack_base_phy, unsigned int new_stack_base_virt)
global enable_paging
enable_paging:
    cli
    
    ;CR3レジスタにPDTのアドレスをセット
    mov eax, [esp + 4]
    mov cr3, eax
    
    ;espの計算（仮想アドレス）、edxに保管してあとで切り替える
    ;新しいスタックの仮想アドレス + (現在のesp - 今のスタックの底)
    mov edx, esp
    sub edx, ORG_STACK_BASE
    add edx, [esp + 12]
    
    ;ebpの計算（仮想アドレス）
    ;新しいスタックの仮想アドレス + (現在のebp - 今のスタックの底)
    sub ebp, ORG_STACK_BASE
    add ebp, [esp + 12]
    
    ;espの計算（物理アドレス）、eaxに保管してコピーに使う
    ;新しいスタックの物理アドレス + (現在のesp - 今のスタックの底)
    mov eax, esp
    sub eax, ORG_STACK_BASE
    add eax, [esp + 8]
    
;スタックの再配置(中身をページング有効化後に仮想アドレスでアクセスされる場所にコピー)
copy_stack:
    mov ecx, [esp]
    add esp, 4
    mov [eax], ecx
    add eax, 4
    cmp esp, ORG_STACK_BASE
    jle copy_stack
    
    ;espを仮想アドレスに切り替え
    mov esp, edx
    
    ;ページング有効化
    mov ecx, cr0
    or ecx, 0x80000000
    mov cr0, ecx
    ret
    

;uint32_t load_cr0(void)
global load_cr0
load_cr0:
    mov eax, cr0
    ret


;void int20_handler(void)
;PIT(IRQ0)
global int20_handler
int20_handler:
    push es
    push ds
    pusha
    call int_handler_pit
    popa
    pop ds
    pop es
    iret

;void int24_handler(void)
;serial COM1(IRQ4)
global int24_handler
int24_handler:
    push es
    push ds
    pusha
    call int_handler_serial
    popa
    pop ds
    pop es
    iret


;void int_handler_null(void);
global int_handler_null
int_handler_null:
    ;segment registerの保存
    ;カーネルコード実行中の割り込みならssは変更されないので保存不要
    push es
    push ds
    ;レジスタの保存
    pusha
    
    ;mov ax, 0x60
    ;out 0x20, al
    ;mov eax, [0x00f00ff0]
    ;inc eax
    ;mov [0x00f00ff0], eax

    popa
    pop ds
    pop es
    iret


;void interrupt(uint32_t num)
global interrupt
interrupt:
    mov eax, [esp+4]
    int 0x24
    ret

