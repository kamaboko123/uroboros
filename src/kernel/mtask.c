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
    uint8_t *sp = task_stack + KTASK_STACK_SIZE;
    
    //割り込みのフレーム設定
    sp -= sizeof(IntrFrame);
    proc->iframe = (IntrFrame *)sp;
    //interrput frame
    proc->iframe->gs = 1 * 8;
    proc->iframe->fs = 1 * 8;
    proc->iframe->es = 1 * 8;
    proc->iframe->ds = 1 * 8;
    //割り込みからの戻り先は渡された関数
    //プロセスの開始アドレスになる
    proc->iframe->eip = (uint32_t)func;
    proc->iframe->cs = 2 * 8;
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
            if(proc != CPU->proc && proc->status == RUNNABLE){
                proc->status = RUNNING;
                context_switch(&CPU->proc->context, proc->context);
            }
        }
    }
}

void sched_handler(void){
    CPU->proc->status = RUNNABLE;
}
