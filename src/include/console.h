#ifndef CONSOLE_H
#define CONSOLE_H

#include "mem.h"
#include "global.h"
#include "queue.h"

#define CONSOLE_LINE_BUF_SIZE 256
typedef struct Console{
    char line[CONSOLE_LINE_BUF_SIZE];
    char *line_p;
    Queue8 *q_in;
    Queue8 *q_out;
} Console;

Console *console_init(Queue8 *q_in, Queue8 *q_out);
void console_run(Console *con);
void console_exec(Console *con, char *cmd);
void console_clear_line(Console *con);
void console_putstr(Console *con, char *str);

#endif
