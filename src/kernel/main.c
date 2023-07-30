#include "kernel.h"

SystemQueue *SYSQ;
TIMERCTL *timerctl;
Console *console;
extern Cpu *CPU;
void mainloop(void);
void task_a(void);
void task_b(void);
void task_console(void);

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
    init_kvmalloc(VMALLOC_START, VMALLOC_INIT_END, VMALLOC_MAX_END);
    
    //GDTを正式なものにする
    init_gdt((GDT *)GDT_ADDR, (GDTR *)GDTR_ADDR);
    
    //IDT初期化
    init_idt((IDT *)IDT_ADDR, (IDTR *)IDTR_ADDR);

    
    //グラフィック初期化
    init_palette();
    init_screen(4);
    
    SYSQ = (SystemQueue *)kvmalloc(sizeof(SystemQueue));
    
    io_cli();
    init_pic(~(PIC_IMR_IRQ0 | PIC_IMR_IRQ4), PIC_INTR_VEC_BASE);
    
    //pit(タイマ)
    //init_pit(119); //大体1ms
    init_pit(0); //大体18Hz
    SYSQ->timer = q8_make(256, 0);
    set_idt((IDT *)IDT_ADDR, 0x20, int20_handler);

    //システムタイマ
    init_timer();
    SYSQ->task_timer = q8_make(256, 0);
    alloc_timer(SYSQ->task_timer, 1);

    //serial port
    init_serial_port();
    SYSQ->com1_in = q8_make(256, 0xff);
    SYSQ->com1_out = q8_make(5000, 0xff);
    set_idt((IDT *)IDT_ADDR, 0x24, int24_handler);
    
    //set_idt((IDT *)IDT_ADDR, 0x40, int40_handler);
    
    //シリアルポートとコンソールを接続
    console = console_init(SYSQ->com1_in, SYSQ->com1_out);
    
    //マルチタスク
    init_mtask();
    Process *p;
    
    p = proc_alloc();
    ktask_init(p, "sched", sched);
    CPU->sched.sched_proc = p;
    
    //p = proc_alloc();
    //ktask_init(p, "mainloop", mainloop);
   
    p = proc_alloc();
    ktask_init(p, "task_console", task_console);
    
    //p = proc_alloc();
    //utask_init(p, "task_user", task_ring3);

    //p = proc_alloc();
    //ktask_init(p, "task_a", task_a);

    //Context *tmp = vmalloc(sizeof(Context));
    //BREAK();
    
    p = proc_alloc();
    ktask_init(p, "dummy", 0);
    p->status = RUNNING;
    CPU->proc = p;
    
    io_sti();
    context_switch(&p->context, CPU->sched.sched_proc->context);
    /*
    for(;;){
        serial_putc('a');
    }*/
}

void task_a(void){
    /*
    for(;;){
        io_hlt();
    }
    */
    //return;
    for(char *c="taska!!"; *c!='\0'; c++){
        serial_putc(*c);
    }
    ktask_exit();
}
void task_b(void){
    /*
    for(;;){
        io_hlt();
    }
    */
    //return;
    for(char *c="taskb!!"; *c!='\0'; c++){
        serial_putc(*c);
    }
    while(1){
        io_hlt();
    }
    ktask_exit();
}

void mainloop(void){
    char console_str[64];
    char *cs = console_str;
    *cs = '\0';
    print_asc(0, 0, 7, "Welcome to UroborOS!");
    for(;;){
        io_hlt();
        init_screen(4);
        
        /*
        char str[64];
        sprintf(str, "task_timer count: %d\n", timerctl->t->next->count);
        for(char *c=str; *c!='\0'; c++){
            serial_putc(*c);
        }*/
        
        
        /*
        //console_run(console);
        while(!q8_empty(SYSQ->com1_out)){
            char c = q8_de(console->q_out);
            serial_putc(c);
        }
        */
        
        /*
        char str[64];
        sprintf(str, "0x%08x\n", load_int_flag());
        for(char *c=str; *c!='\0'; c++){
            serial_putc(*c);
        }
        */
    }   
}


void task_console(){
    for(;;){
        io_hlt();

        init_screen(4);
        console_run(console);
        while(!q8_empty(SYSQ->com1_out)){
            char c = q8_de(console->q_out);
            serial_putc(c);
        }
        
    } 
}

