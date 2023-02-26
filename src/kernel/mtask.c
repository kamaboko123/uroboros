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
        }
    }
    io_sti();
    return proc;
}

void ktask_init(Process *proc, void (*func)(void)){
    //サイズこれでいい？
    uint8_t *task_stack = vmalloc(KTASK_STACK_SIZE);

    //スタックポインタ計算用
    uint8_t *sp = task_stack + KTASK_STACK_SIZE + 4;
    
    proc->context = (Context *)sp;
    sp -= sizeof(Context);

    proc->context->eip = (uint32_t)int20_ret;

    //割り込みのフレーム設定
    proc->iframe = (IntrFrame *)sp;
    sp -= sizeof(IntrFrame);
    //interrput frame
    proc->iframe->gs = 1 * 8;
    proc->iframe->fs = 1 * 8;
    proc->iframe->es = 1 * 8;
    proc->iframe->ds = 1 * 8;
    proc->iframe->eip = (uint32_t)func;
    proc->iframe->cs = 2 * 8;
    
    //エントリポイント(割り込みからの戻り先)
    sp -= 4;
    *sp = (uint32_t)func;
    
}
