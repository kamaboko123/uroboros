
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


;void io_out8(int port, int data)
global io_out8
io_out8:
    mov edx, [esp+4]
    mov al, [esp+8]
    out dx, al
    ret


;int io_load_eflags(void)
global io_load_eflags
io_load_eflags:
    pushf
    pop eax
    ret

;int io_store_eflags(int flags)
global io_store_eflags
io_store_eflags:
    mov eax, [esp+4]
    push eax
    popf
    ret

;void enable_paging(unsigned int pdt_ddr)
global enable_paging
enable_paging:
    cli
    mov ecx, [esp]
    mov eax, [esp + 4]
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    mov esp, 0x00101f00
    jmp ecx

;int load_cr0(void)
global load_cr0
load_cr0:
    mov eax, cr0
    ret


