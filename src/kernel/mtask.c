#include "mtask.h"

Cpu *CPU;

void task_idle(void){
    while(true);
}

void init_mtask(void){
    CPU = (Cpu*)vmalloc(sizeof(Cpu));
    for(int i = 0; i < PROCESS_COUNT; i++){
        CPU->sched.proc[i].status = NOALLOC;
    }
}

Process *proc_alloc(void){
    io_cli();
    Process *proc = NULL;
    for(int i = 0; i < PROCESS_COUNT; i++){
        proc = &CPU->sched.proc[i];
        if(proc->status == NOALLOC){
            proc->status = INIT;
            break;
        }
    }
    io_sti();
    return proc;
}

void ktask_init(Process *proc, char *name, void (*func)(void)){

    strncpy(proc->name, name, PROCESS_NAME_LENGTH);

    //サイズこれでいい？
    uint8_t *task_stack = vmalloc(KTASK_STACK_SIZE);

    //スタックポインタ計算用
    uint8_t *sp = task_stack + KTASK_STACK_SIZE;
    
    //割り込みのフレーム設定
    sp -= sizeof(IntrFrame);
    proc->iframe = (IntrFrame *)sp;
    //interrput frame
    proc->iframe->gs = GDT_SEGNUM_KERNEL_DATA << 3;
    proc->iframe->fs = GDT_SEGNUM_KERNEL_DATA << 3;
    proc->iframe->es = GDT_SEGNUM_KERNEL_DATA << 3;
    proc->iframe->ds = GDT_SEGNUM_KERNEL_DATA << 3;
    //割り込みからの戻り先は渡された関数
    //プロセスの開始アドレスになる
    proc->iframe->eip = (uint32_t)func;
    proc->iframe->cs = GDT_SEGNUM_KERNEL_CODE << 3;
    //新しいスタックの底
    proc->iframe->ebp = (uint32_t)task_stack + KTASK_STACK_SIZE;
    
    sp -= sizeof(Context);
    proc->context = (Context *)sp;
    
    //まずは割り込みからの戻り処理に行く
    proc->context->eip = (uint32_t)int20_ret;

    proc->status = RUNNABLE;
}

void sched(void){
    Process *proc = NULL;
    while(true){
        for(int i = 0; i < PROCESS_COUNT; i++){
            proc = &CPU->sched.proc[i];
            if(proc != CPU->sched.sched_proc && proc != CPU->proc && proc->status == RUNNABLE){
                /*
                for(char *c=proc->name; *c != '\0'; c++){
                    serial_putc(*c);
                }
                serial_putc('\n');
                */
                CPU->proc = proc;
                proc->status = RUNNING;
                BREAK();
                context_switch(&CPU->sched.sched_proc->context, proc->context);
            }
        }
    }
}

void sched_handler(void){
    io_out8(IO_PORT_PIC1_OCW2, PIC_OCW2_CMD_EOI);
    CPU->proc->status = RUNNABLE;
    /*
    for(char *c="task switch! "; *c != '\0'; c++){
        serial_putc(*c);
    }
    for(char *c=CPU->proc->name; *c != '\0'; c++){
        serial_putc(*c);
    }
    serial_putc('>');
    */
    context_switch(&CPU->proc->context, CPU->sched.sched_proc->context);
}
