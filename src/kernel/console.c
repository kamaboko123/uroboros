#include "console.h"

Console *console_init(Queue8 *q_in, Queue8 *q_out){
    Console *con = (Console *)vmalloc(sizeof(Console));
    *con->line = '\0';
    con->line_p = con->line;
    con->q_in = q_in;
    con->q_out = q_out;
    return con;
}

void console_run(Console *con){
    while (!q8_empty(con->q_in)){
        char c = q8_de(con->q_in);
        if(strlen(con->line) >= CONSOLE_LINE_BUF_SIZE - 1){
            //溢れたとき
            //キューをクリアしないといけない
            continue;
        }
        if(c == 0x0A){
            //LF
            con->line_p = con->line;
            *con->line_p = '\0';
            
            q8_in(con->q_out, c);
            
            continue;
        }
        *con->line_p = c;
        con->line_p++;
        *con->line_p = '\0';

        q8_in(con->q_out, c);
    }
}
