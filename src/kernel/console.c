#include "console.h"

extern void task_a();
extern void task_b();
extern SystemQueue *SYSQ;
extern TIMERCTL *timerctl;
extern Cpu *CPU;

Console *console_init(Queue8 *q_in, Queue8 *q_out){
    Console *con = (Console *)kvmalloc(sizeof(Console));
    con->line_p = con->line;
    *con->line_p = '\0';
    con->q_in = q_in;
    con->q_out = q_out;
    
    console_clear_line(con);
    return con;
}

void console_clear_line(Console *con){
    con->line_p = con->line;
    con->line[0] = '\0';

    char str[] = "\r\n> ";
    console_putstr(con, str);
}

void console_run(Console *con){
    while(!q8_empty(con->q_in)){
        char c = q8_de(con->q_in);
        
        if(!(c == '\r' || c == '\n')){
            q8_in(con->q_out, c);
        }

        *con->line_p = c;
        con->line_p++;
        *con->line_p = '\0';
    }
    
    for(char *c=con->line; *c!='\0'; c++){
        if(*c == '\r'){
            q8_in(con->q_out, '\r');
            q8_in(con->q_out, '\n');
            console_exec(con, con->line);
            console_clear_line(con);
        }
    }

    //TODO: lineを溢れたときの実装
    //キューを空にしたほうが良いかも
}

void console_putstr(Console *con, char *str){
    for(char *c=str; *c != '\0'; c++){
        q8_in(con->q_out, *c);
    }
}

void console_exec(Console *con, char *line){
    Command cmd;
    parse_command(&cmd, line);

    //console_putstr(con, cmd.command);

    if(strcmp(cmd.command, "uname") == 0){
        char str[] = "UroborOS v0.0.1\r\n";
        console_putstr(con, str);
    }
    else if(strcmp(cmd.command, "vmem") == 0){
        char str[128];
        V_MEMMAN *memman = (V_MEMMAN *)VMALLOC_MAN_ADDR;
        V_MEM_BLOCKINFO *p = memman->entry;
        
        sprintf(str, "[virtual memory allocation table]\n");
        console_putstr(con, str);
        
        sprintf(str, "[extent] 0x%08x - 0x%08x (max: 0x%08x)\n\n", memman->extent_start, memman->extent_end, memman->extent_max);
        console_putstr(con, str);

        while(p->next != NULL){
            if((p->flags & VMEM_BLOCKS_ALLOC) != 0){
                sprintf(str, "[block] [virt: 0x%08x - 0x%08x] [phys: 0x%08x - 0x%08x] (size: %d byte, %d 4k-pages)\n", p->addr, p->addr + p->size -1, p->p_mem->addr, p->p_mem->addr - p->size - 1, p->size, mem_npage(p->size));
                console_putstr(con, str);
            }
            p = p->next;
        }
    }
    else if(strcmp(cmd.command, "proc") == 0){
        bool iflag = load_int_flag();
        io_cli();
        
        char str[128];
        for(int i = 0; i < PROCESS_COUNT; i++){
            Process *proc = &CPU->sched.proc[i];
            if(proc->status == NOALLOC) continue;
            sprintf(str, "[%d] %s (%d) (eip: 0x%08x)\n", i, proc->name, proc->status, proc->iframe->eip);
            console_putstr(con, str);
        }
        store_int_flag(iflag);
    }
    else if(strcmp(cmd.command, "task_a") == 0){
        Process *p = proc_alloc();
        ktask_init(p, "task_a", task_a);
    }
    else if(strcmp(cmd.command, "task_b") == 0){
        Process *p = proc_alloc();
        ktask_init(p, "task_b", task_b);
    }
    else if(strcmp(cmd.command, "timers") == 0){
        for(TIMER *t = timerctl->t; t != NULL; t=t->next){
            if(t->interval == 0) continue;
            char str[128];
            sprintf(str, "interval: %4d, count: %4d\n", t->interval, t->count);
            console_putstr(con, str);
        }
    }
    else if(strcmp(cmd.command, "kill") == 0){
        if(cmd.args_count != 1){
            console_putstr(con, "invalid argument");
            return;
        }
        
        // TODO: support multiple digits
        uint8_t kpid = cmd.args[1][0] - '0';
        if(kpid < 0 || kpid > 9){
            console_putstr(con, "invalid argument");
            return;
        }

        Process *p = &CPU->sched.proc[kpid];
        ktask_kill(p);
    }

    /*
    else if(strcmp(cmd, "pmemtable\r") == 0){
        char str[128];
        P_MEMMAN *memman = get_phy_memman();

        bool flg = false;
        uint32_t start_addr = 0;
        uint32_t end_addr = 0;
        for(int i = 0; i < PMALLOC_MAX_PAGE; i++){
            if(memman->tbl[i] == 1){
                if(!flg){
                    start_addr = memman->base_addr + (i * MEM_PAGE_SIZE);
                }
                end_addr = memman->base_addr + ((i + 1) * MEM_PAGE_SIZE) - 1;
                flg = true;
            }
            else{
                if(end_addr != 0){
                    sprintf(str, "[page] 0x%08x - 0x%08x\n", start_addr, end_addr);
                    console_putstr(con, str);
                }
                start_addr = 0;
                end_addr = 0;
                flg = false;
            }
        }
    }*/
    else{
        char str[] = "[Error] command not found\r\n";
        console_putstr(con, str);
    }
}


bool parse_command(Command *cmd, char *line){
    memset((char *)cmd, 0, sizeof(Command));
    
    cmd->args_count = 0;
    char *p = line;
    char *q = cmd->args[cmd->args_count];
    while((*p != '\0') && (*p != '\r') && (*p != '\n')){
        //区切り文字を見つけたら
        if(*p == ' '){
            // 引数を終端
            *q = '\0';
            //引数の数を増やす
            cmd->args_count++;
            //次の引数に入れていくようにする
            q = cmd->args[cmd->args_count];
            //区切り文字ではないところまでポインタを進める
            while(*p ==  ' ') p++;
            continue;
        }
        *q = *p;
        q++;
        p++;
    }
    cmd->command = cmd->args[0];
    return true;
}
