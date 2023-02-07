#include "kernel.h"

SystemQueue *SYSQ;

void Main(uint64_t *gdt0){
    //カーネル関連の最低限のページを初期化
    //カーネルの再配置
    init_kernel_mem();
    //stack
    uint32_t new_stack_p;
    for(int i = 0; i < mem_npage(KERNEL_STACK_SIZE); i++){
        uint32_t mem = (uint32_t)pmalloc_4k();
        if(i == 0){
            new_stack_p = mem;
        }
        map_memory_4k((PDE *)KERNEL_PDT, KERNEL_STACK_TOP_V + i * MEM_PAGE_SIZE, mem);
    }
    
    //ページングの有効化
    //スタックの再配置を行う（コピーもする）
    //CR3にページングテーブルのアドレスをストア
    //CR0のPGフラグを立てる
    enable_paging(KERNEL_PDT, new_stack_p + KERNEL_STACK_SIZE - 4, KERNEL_STACK_TOP_V + KERNEL_STACK_SIZE - 4);
    
    //vmalloc初期化
    init_vmalloc(VMALLOC_START, VMALLOC_INIT_END, VMALLOC_MAX_END);
    
    //GDTを正式なものにする
    init_gdt((GDT *)GDT_ADDR, (GDTR *)GDTR_ADDR);
    
    //IDT初期化
    init_idt((IDT *)IDT_ADDR, (IDTR *)IDTR_ADDR);

    
    //グラフィック初期化
    init_palette();
    init_screen(4);
    
    SYSQ = (SystemQueue *)vmalloc(sizeof(SystemQueue));
    
    io_cli();
    init_pic(~(PIC_IMR_IRQ0 | PIC_IMR_IRQ4), PIC_INTR_VEC_BASE);
    
    //pit(タイマ)
    //init_pit(119); //大体1ms
    init_pit(0); //大体18Hz
    SYSQ->timer = q8_make(256, 0);
    set_idt((IDT *)IDT_ADDR, 0x20, int20_handler);

    //serial port
    init_serial_port();
    SYSQ->com1 = q8_make(256, 0);
    set_idt((IDT *)IDT_ADDR, 0x24, int24_handler);
    
    /*
    if(get_paging_status()){
        print_asc(0, 16, 7, "paging is enable!");
    }
    else{
        print_asc(0, 16, 7, "paging is disable!");
    }
    */
    
    io_sti();

    uint32_t cnt = 0;
    char s[64];
    char console_str[64];
    char *cs = console_str;
    *cs = '\0';
    for(;;){
        print_asc(0, 0, 7, "Welcome to UroborOS!");
        
        uint8_t data = q8_de(SYSQ->timer);
        if(data != SYSQ->timer->default_value){
            cnt++;
            init_screen(4);
        }
        sprintf(s, "cnt: %d", cnt);
        print_asc(0, 16, 7, s);
        
        //sprintf(s, "SYSQ: %x", (uint32_t)(*SYSQ->timer));
        //print_asc(0, 32, 7, s);
        
        char com1_data = q8_de(SYSQ->com1);
        if(com1_data != SYSQ->com1->default_value){
            *cs = com1_data;
            cs++;
            *cs = '\0';
            init_screen(4);
        }
        print_asc(0, 32, 7, console_str);
        
        /*
        uint8_t serial_in = read_serial();
        if(serial_in != 0){
            sprintf(s, "serial: %x", serial_in);
        }
        print_asc(0, 32, 7, s);
        print_asc(0, 48, 7, "aaa");
        */
    }
    
    //char s[64];
    //sprintf(s, "g: 0x%08x", *((uint64_t *)(gdt+1)));
    //print_asc(0, 32, 7, s);
    
    /*
    char *p = pmalloc_4k();
    sprintf(s, "[palloc_4k] alloc : 0x%08x", p);
    print_asc(0, 48, 7, s);
    for(int i = 1; i < 5; i++){
        uint32_t mem =(uint32_t) vmalloc(i * 0x1000+3);
        *(uint32_t *)mem = 1000;
        sprintf(s, "[vmalloc] 0x%08x - 0x%08x", mem, i*0x1000 - 1);
        print_asc(0, i * 16 + 64, 7, s);
    }
    */
    
}

