#include "kernel.h"

void Main(uint8_t *kargs, ...){
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
    init_kvmalloc(VMALLOC_START, VMALLOC_INIT_END);
    //GDTを正式なものにする
    init_gdt((GDT_SEG_DESC *)GDT_ADDR, (GDTR *)GDTR_ADDR);
    
    //IDT初期化
    init_idt((IDT *)IDT_ADDR, (IDTR *)IDTR_ADDR);

    //TSS初期化
    TSS32 *tss0 = (TSS32 *)kvmalloc(sizeof(TSS32));
    init_tss(tss0, (GDT_SEG_DESC *)GDT_ADDR + GDT_SEGNUM_TSS0, (GDTR *)GDTR_ADDR);

    //global変数の領域を確保
    sys = (System *)kvmalloc(sizeof(System));
    
    //グラフィック初期化
    sys->vram = (uint8_t *)VRAM_ADDR_V;
    init_palette();
    init_screen(4);
    
    //PICの初期化
    //これ以降PICより割り込みが入る可能性があるので、先に割り込みを無効化する
    io_cli();
    init_pic(~(PIC_IMR_IRQ0 | PIC_IMR_IRQ4 | PIC_IMR_IRQ6), PIC_INTR_VEC_BASE);
    
    //pit(タイマ)
    init_pit(11920); //大体1ms
    set_idt((IDT *)IDT_ADDR, 0x20, int20_handler);

    //システムタイマ
    timer_init();
    sys->task_timer = q8_make(TIMER_QUEUE_SIZE, 0);
    timer_alloc(sys->task_timer, 1, TIMER_MODE_ONESHOT);

    //serial port
    init_serial_port();
    sys->com1_in = q8_make(256, 0xff);
    sys->com1_out = q8_make(5000, 0xff);
    set_idt((IDT *)IDT_ADDR, 0x24, int24_handler);

    //シリアルポートとコンソールを接続
    sys->console1 = console_init(sys->com1_in, sys->com1_out);
    
    //マルチタスク
    init_mtask();
    Process *p;
    
    //p = proc_alloc();
    //ktask_init(p, "mainloop", mainloop);
   
    p = proc_alloc();
    ktask_init(p, "task_console", (uint32_t)task_console, 0);
    
    p = proc_alloc();
    utask_init(p, "z", task_ring3);

    p = proc_alloc();
    ktask_init(p, "task_a", (uint32_t)task_a, 0);
    p = proc_alloc();
    ktask_init(p, "task_b", (uint32_t)task_b, 0);
    
    p = proc_alloc();
    ktask_init(p, "task_fdc", (uint32_t)task_fdc, 0);
    set_idt((IDT *)IDT_ADDR, 0x26, int26_handler);
    
    
    //p = proc_alloc();
    //ktask_init(p, "test_task1", (uint32_t)test_task, sizeof(char *) + sizeof(int), "test_task1\n", 10000000);
    
    //p = proc_alloc();
    //ktask_init(p, "test_task2", (uint32_t)test_task, sizeof(char *) + sizeof(int), "test_task2\n", 15000000);
    

    print_asc(0, 0, 7, "Welcome to UroborOS!");
    // スケジューラタスクに切り替えて、これ以降はスケジューラによるタスク選択に委ねる
    start_mtask(sys->cpu->sched.sched_proc->context);
}

void task_a(void){
    //return;
    for(char *c="taska!!"; *c!='\0'; c++){
        serial_putc(*c);
    }
    for(;;);
    ktask_exit();
}
void task_b(void){
    for(char *c="taskb!!"; *c!='\0'; c++){
        serial_putc(*c);
    }
    while(1){
        io_hlt();
    }
    ktask_exit();
}

void task_fdc(){
    FdcResult result;
    result = init_fdc();
    char str[128];
    sprintf(str, "fdc init result: %d\n", result);
    serial_putstr(str);

    uint32_t fd_size = FD_SECTOR_SIZE * FD_CYLINDERS * FD_HEADS * FD_SECTORS;
    uint8_t *buf = (uint8_t *)kvmalloc(fd_size);
    map_memory_4k((PDE *)KERNEL_PDT, FD_BUFFER_V, FD_BUFFER_P);

    uint32_t cnt = 0;
    // read all sectors
    for(int c = 0; c < FD_CYLINDERS; c++){
        for(int h = 0; h < FD_HEADS; h++){
            for(int s = 1; s <= FD_SECTORS; s++){
                fdc_cmd_read_data(0, FD_BUFFER_P, c, h, s);
                memcpy((char *)buf + cnt, (char *)FD_BUFFER_V, 512);
                cnt += FD_SECTOR_SIZE;
            }
            serial_putstr(".");
        }
        serial_putstr("\n");
    }
    serial_putstr("read all sectors\n");
    serial_putstr("===\n");
    for(int i = 0; i < 512*4; i++){
        char str[64];
        sprintf(str, "%02x ", buf[i]);
        serial_putstr(str);
        if(i % 16 == 15){
            serial_putstr("\n");
        }
    }
    serial_putstr("===\n");
    BREAK();
    while(1){}
    ktask_exit();
}

void test_task(char *str, int interval){
    while(1){
        serial_putstr(str);
        for(int i = 0; i < interval; i++);
    }
}

void task_console(void){
    for(;;){
        io_hlt();
        console_run(sys->console1);
    } 
}

