#include "console.h"

Console *console_init(Queue8 *q_in, Queue8 *q_out){
    Console *con = (Console *)vmalloc(sizeof(Console));
    con->line_p = con->line;
    *con->line_p = '\0';
    con->q_in = q_in;
    con->q_out = q_out;

    return con;
}

void console_run(Console *con){
    while(!q8_empty(con->q_in)){
        char c = q8_de(con->q_in);
        q8_in(con->q_out, c);

        if(c == '\r'){
            console_exec(con, con->line);
        }
    }
    
    //while((c = q8_de(con->q_in)) != con->q_in->default_value){
        //q8_in(con->q_out, c);
        /*
        if(strlen(con->line) >= CONSOLE_LINE_BUF_SIZE - 1){
            //溢れたとき
            //キューをクリアしないといけない
            return;
        }
        *con->line_p = c;
        con->line_p++;
        *con->line_p = '\0';
        */
        /*
        if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'z') || c== '\n'){
            *con->line_p = c;
            con->line_p++;
            *con->line_p = '\0';
            q8_in(con->q_out, c);
            if(c == '\n'){
                //console_exec(con, con->line);
                //con->line_p = con->line;
                char str[] = "UroborOS v0.0.1\r\n";
                for(char *c=str; *c != '\0'; c++){
                    q8_in(con->q_out, *c);
                }
            }
        }
        */
        /*
        if(c == 'a'){
            q8_in(con->q_out, 'A');
            //console_exec(con, con->line);
            //con->line_p = con->line;
        }
        */
    //}
}

void console_exec(Console *con, char *cmd){
    char str[] = "UroborOS v0.0.1\r\n";
    for(char *c=str; *c != '\0'; c++){
        q8_in(con->q_out, *c);
    }
}
