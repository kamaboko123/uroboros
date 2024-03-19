#ifndef CONSOLE_H
#define CONSOLE_H

#include "mem.h"
#include "global.h"
#include "queue.h"
#include "mtask.h"
#include "timer.h"

#define CONSOLE_LINE_BUF_SIZE 256
#define MAX_ARG_LEN 16
#define MAX_COMMAND_ARGS_COUNT 16
typedef struct Console{
    char line[CONSOLE_LINE_BUF_SIZE];
    char *line_p;
    Queue8 *q_in;
    Queue8 *q_out;
} Console;

typedef struct Command{
    char *command;
    char args[MAX_COMMAND_ARGS_COUNT][MAX_ARG_LEN];
    uint8_t args_count;
} Command;

Console *console_init(Queue8 *q_in, Queue8 *q_out);
void console_run(Console *con);
void console_exec(Console *con, char *line);
void console_clear_line(Console *con);
void console_putstr(Console *con, char *str);
bool parse_command(Command *cmd, char *line);

#endif
