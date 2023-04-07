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

void ktask_exit(){
    //実行中のタスクを終了させる
    
    //切り替え中にタスクスイッチしないように割り込みを無効化
    //TODO: （cli命令が実行される前にタスクスイッチが起きる可能性は...？）
    bool iflag = load_int_flag();
    io_cli();
    /*
    for(char *c=CPU->proc->name; *c != '\0'; c++){
        serial_putc(*c);
    }*/

    //Processを解放、スタックで割り当てていたメモリも解放
    //タスク内でvmallocしたメモリはタスク内で片付ける
    //ユーザーランドプロセスではなく、カーネルプロセスなのでこれでOK
    CPU->proc->status = NOALLOC;
    vfree(CPU->proc->stack);
    
    store_int_flag(iflag);
    context_switch(&CPU->proc->context, CPU->sched.sched_proc->context);
}

void ktask_kill(Process *proc){
    //proc
    bool iflag = load_int_flag();
    io_cli();

    if((proc->status != RUNNING) || (proc->status != RUNNABLE)){
        store_int_flag(iflag);
        return;
    }

    //現在実行中のプロセスが指定された
    if(proc == CPU->proc){
        ktask_exit();
    }
    else{
        proc->status = NOALLOC;
        vfree(proc->stack);
        store_int_flag(iflag);
    }
}

void ktask_init(Process *proc, char *name, void (*func)(void)){
    strncpy(proc->name, name, PROCESS_NAME_LENGTH);

    //サイズこれでいい？
    uint8_t *task_stack = vmalloc(KTASK_STACK_SIZE);

    //スタックポインタ計算用
    uint8_t *sp = task_stack + KTASK_STACK_SIZE;
    
    //確保したスタック用メモリを記録しておく(終了時に解放する)
    proc->stack = task_stack;
    
    char str[64];
    sprintf(str, "%s (stack: 0x%08x)\n", proc->name, task_stack);
    for(char *c=str; *c!='\0'; c++){
        serial_putc(*c);
    }
    
    
    //割り込みのフレーム設定
    sp -= sizeof(IntrFrame);
    proc->iframe = (IntrFrame *)sp;
    //interrput frame
    proc->iframe->gs = GDT_SEGNUM_KERNEL_DATA << 3;
    proc->iframe->fs = GDT_SEGNUM_KERNEL_DATA << 3;
    proc->iframe->es = GDT_SEGNUM_KERNEL_DATA << 3;
    proc->iframe->ds = GDT_SEGNUM_KERNEL_DATA << 3;
    //デフォルトで割り込みは有効にしておく
    proc->iframe->eflags = 1 << 9;
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
            //次に実行するタスクを決定する
            proc = &CPU->sched.proc[i];
            //スケジューラ・直前に実行したタスクを避けて、RUNNABLEなプロセスを選ぶ
            if(proc != CPU->sched.sched_proc && proc != CPU->proc && proc->status == RUNNABLE){
                /*
                for(char *c=proc->name; *c != '\0'; c++){
                    serial_putc(*c);
                }
                serial_putc('\n');
                */
                //プロセスの状態を切り替える
                CPU->proc = proc;
                proc->status = RUNNING;
                BREAK();
                //コンテキストスイッチ
                //このスケジューラ自体もタスクの1つなので、ここまでのコンテキストは保存される
                //(次のコンテキストスイッチでは、この後から復帰し、再びタスクの選択を行うところから）
                context_switch(&CPU->sched.sched_proc->context, proc->context);
                
            }
            /*
            char str[64];
            sprintf(str, "0x%08x\n", str);
            for(char *c=str; *c!='\0'; c++){
                serial_putc(*c);
            }
            */
        }
    }
}

void sched_handler(void){
    /*
    for(char *c="task switch! "; *c != '\0'; c++){
        serial_putc(*c);
    }
    for(char *c=CPU->proc->name; *c != '\0'; c++){
        serial_putc(*c);
    }
    serial_putc('>');
    */
    //現在動作中のタスクの状態を変更(このあと別のタスクに切り替えるので、RUNNABLEに)
    CPU->proc->status = RUNNABLE;
    //スケジューラにコンテキストスイッチ
    //スケジューラが次のタスクを決定して切り替える
    context_switch(&CPU->proc->context, CPU->sched.sched_proc->context);
}
