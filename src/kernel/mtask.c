#include "mtask.h"

void task_ring3(void){
    while(true){
        //io_cli();
        //serial_putc('a');
        //BREAK();
        //q8_in(sys->com1_out, 'a');
    }
}

void init_mtask(void){
    sys->cpu = (Cpu*)kvmalloc(sizeof(Cpu));
    for(int i = 0; i < PROCESS_COUNT; i++){
        sys->cpu->sched.proc[i].status = NOALLOC;
    }

    init_sched_proc();
}

void init_sched_proc(){
    Process *p;
    p = proc_alloc();
    ktask_init(p, "sched", (uint32_t)sched, 0);
    //スケジューラ内で割り込みが入るのを避ける
    p->iframe->eflags = 0;
    //スケジューラはスケジューラ自身によって選択されないようにする
    p->status = NOSCHED;
    //sys->cpu構造体でスケジューラのタスクを管理する
    sys->cpu->sched.sched_proc = p;
}


Process *proc_alloc(void){
    bool iflag = load_int_flag();
    io_cli();
    Process *proc = NULL;
    for(int i = 0; i < PROCESS_COUNT; i++){
        proc = &sys->cpu->sched.proc[i];
        if(proc->status == NOALLOC){
            proc->status = INIT;
            break;
        }
    }
    store_int_flag(iflag);
    return proc;
}

void ktask_exit(){
    //実行中のタスクを終了させる
    
    //切り替え中にタスクスイッチしないように割り込みを無効化
    bool iflag = load_int_flag();
    io_cli();

    //Processを解放、スタックで割り当てていたメモリも解放
    //タスク内でvmallocしたメモリはタスク内で片付ける
    //ユーザーランドプロセスではなく、カーネルプロセスなのでこれでOK
    sys->cpu->proc->status = NOALLOC;
    kvfree(sys->cpu->proc->stack);
    
    store_int_flag(iflag);
    context_switch(&sys->cpu->proc->context, sys->cpu->sched.sched_proc->context);
}

void ktask_kill(Process *proc){
    //proc
    bool iflag = load_int_flag();
    io_cli();

    if((proc->status != RUNNING) && (proc->status != RUNNABLE)){
        store_int_flag(iflag);
        return;
    }

    //現在実行中のプロセスが指定された
    if(proc == sys->cpu->proc){
        ktask_exit();
    }
    else{
        proc->status = NOALLOC;
        kvfree(proc->stack);
        store_int_flag(iflag);
    }
}

void ktask_init(Process *proc, char *name, uint32_t task_entry_addr, uint32_t arg_size, ...){
    bool iflag = load_int_flag();
    io_cli();
    
    //context_switchでprocが指定された場合に、あたかもタイマ割り込み処理から復帰するかのようにメモリを設定する
    //割り込み発生時には、IntrFrameで示すようにレジスタが退避される
    //そのあとcontext_swicthが呼ばれ、コンテキストの切り替えが行われる
    //すでに稼働しているプロセスでは上記のようにタスクスイッチを行うが、
    //これから稼働させるプロセスは上記の処理によってタスクスイッチが行えるようにメモリを整える必要がある
    //ここでは割り込みフレームや、コンテキストの初期化を行い、コンテキストスイッチにより、指定された関数から処理を開始できるようにする

    strncpy(proc->name, name, PROCESS_NAME_LENGTH);

    //サイズは仮
    uint8_t *task_stack = kvmalloc(KTASK_STACK_SIZE);

    //スタックポインタ計算用
    uint32_t sp = (uint32_t)task_stack + KTASK_STACK_SIZE;
    
    //確保したスタック用メモリを記録しておく(終了時に解放する)
    proc->stack = task_stack;

    // 引数をタスクのスタックにコピーする
    // 最初の引数のアドレスを取得
    uint8_t *arg_head = (uint8_t *)(&arg_size + 1);
    // タスクのスタックに引数をコピーする
    // (タスク側も引数として受け取るのでスタックポインタも更新する)
    memcpy((char *)sp - arg_size, (char *)arg_head, arg_size);
    sp -= arg_size;

    sp -= 4;
    *(uint32_t *)sp = 0x0000; // fake return address for task

    //割り込みのフレーム設定
    // ユーザーランドからの復帰ではないので、ssとespはスタック上に積まない(その分確保する領域を縮小)
    sp -= sizeof(IntrFrame) - 8;
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
    proc->iframe->eip = task_entry_addr;
    proc->iframe->cs = GDT_SEGNUM_KERNEL_CODE << 3;
    //新しいスタックの底
    proc->iframe->ebp = (uint32_t)task_stack + KTASK_STACK_SIZE;
    
    //コンテキストの設定
    //espは実質的にここで設定される
    //context_switch内でespはproc->contextに切り替えられる
    //その後Context内に含まれるレジスタをpopして、iret20_retへ復帰する
    //iret20_retではproc->iframeで設定したように割り込みからの復帰が行われる
    sp -= sizeof(Context);
    proc->context = (Context *)sp;
    
    //まずは割り込みからの戻り処理に行く
    proc->context->eip = (uint32_t)all_interrupt_ret;

    proc->status = RUNNABLE;
    
    store_int_flag(iflag);
}

void utask_init(Process *proc, char *name, void (*entry)(void)){
    strncpy(proc->name, name, PROCESS_NAME_LENGTH);

    //サイズこれでいい？
    uint8_t *task_stack = kvmalloc(KTASK_STACK_SIZE);

    //スタックポインタ計算用
    uint8_t *sp = task_stack + KTASK_STACK_SIZE;
    
    //確保したスタック用メモリを記録しておく(終了時に解放する)
    proc->stack = task_stack;
    
    char str[64];
    sprintf(str, "[u]%s (stack: 0x%08x)\n", proc->name, task_stack);
    for(char *c=str; *c!='\0'; c++){
        serial_putc(*c);
    }
    
    
    //割り込みのフレーム設定
    sp -= sizeof(IntrFrame);
    proc->iframe = (IntrFrame *)sp;
    //interrput frame
    //ring3として動かしたいので、セグメントディスクリプタ指定の下2bitをセットしておく
    proc->iframe->gs = (GDT_SEGNUM_APP_DATA << 3) | 3;
    proc->iframe->fs = (GDT_SEGNUM_APP_DATA << 3) | 3;
    proc->iframe->es = (GDT_SEGNUM_APP_DATA << 3) | 3;
    proc->iframe->ds = (GDT_SEGNUM_APP_DATA << 3) | 3;
    //デフォルトで割り込みは有効にしておく
    proc->iframe->eflags = 1 << 9;
    //割り込みからの戻り先は渡された関数
    //プロセスの開始アドレスになる
    proc->iframe->eip = (uint32_t)entry;
    //proc->iframe->cs = (GDT_SEGNUM_APP_CODE << 3) | 3;
    proc->iframe->cs = (GDT_SEGNUM_APP_CODE << 3) | 3;
    //新しいスタックの底
    proc->iframe->ebp = (uint32_t)task_stack + KTASK_STACK_SIZE;

    sp -= sizeof(Context);
    proc->context = (Context *)sp;
    
    //まずは割り込みからの戻り処理に行く
    proc->context->eip = (uint32_t)all_interrupt_ret;

    // ring3への遷移の場合にはssとespもpopされる
    // ユーザランド用に確保したスタック領域を設定しておく
    proc->iframe->ss = (GDT_SEGNUM_APP_DATA << 3) | 3;
    proc->iframe->esp = proc->iframe->ebp;

    proc->status = RUNNABLE;
}

void sched(void){
    Process *proc = NULL;
    while(true){
        for(int i = 0; i < PROCESS_COUNT; i++){
            //次に実行するタスクを決定する
            proc = &sys->cpu->sched.proc[i];
            //RUNNABLEなプロセスを選ぶ
            //スケジューラ自体もプロセスだが、スケジューラはNOSCHEDという特殊な状態を持つのでここでは選ばれない
            if((proc->status == RUNNABLE) && (proc != sys->cpu->proc)){
                //プロセスの状態を切り替える
                sys->cpu->proc = proc;
                proc->status = RUNNING;
                //コンテキストスイッチ
                //このスケジューラ自体もタスクの1つなので、ここまでのコンテキストは保存される
                //(次のコンテキストスイッチでは、この後から復帰し、再びタスクの選択を行うところから）
                if(strcmp(sys->cpu->proc->name, "task_timer") != 0) BREAK();
                context_switch(&sys->cpu->sched.sched_proc->context, proc->context);
                
            }
        }
    }
}

void sched_handler(void){
    //現在動作中のタスクの状態を変更(このあと別のタスクに切り替えるので、RUNNABLEに)
    sys->cpu->proc->status = RUNNABLE;
    //スケジューラにコンテキストスイッチ
    //スケジューラが次のタスクを決定して切り替える
    context_switch(&sys->cpu->proc->context, sys->cpu->sched.sched_proc->context);
}

