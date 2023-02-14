#include "console.h"

Console *console_init(Queue8 *q_in, Queue8 *q_out){
    Console *con = (Console *)vmalloc(sizeof(Console));
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

void console_exec(Console *con, char *cmd){
    if(strcmp(cmd, "uname\r") == 0){
        char str[] = "UroborOS v0.0.1\r\n";
        console_putstr(con, str);
    }
    else if(strcmp(cmd, "vmemtable\r") == 0){
        char str[128];
        V_MEMMAN *memman = (V_MEMMAN *)VMALLOC_MAN_ADDR;
        V_MEM_BLOCKINFO *p = memman->entry;
        
        sprintf(str, "[virtual memory allocation table]\n");
        console_putstr(con, str);
        
        sprintf(str, "[extent] 0x%08x - 0x%08x (max: 0x%08x)\n\n", memman->extent_start, memman->extent_end, memman->extent_max);
        console_putstr(con, str);

        while(p->next != NULL){
            if((p->flags & VMEM_BLOCKS_USE) != 0){
                sprintf(str, "[block] 0x%08x - 0x%08x (size: %d byte)\n", p->addr, p->addr + p->size -1, p->size);
                console_putstr(con, str);
            }
            p = p->next;
        }
    }
    else{
        char str[] = "[Error] command not found\r\n";
        console_putstr(con, str);
    }
}
