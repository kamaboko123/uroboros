#ifndef MTASK_H
#define MTASK_H

//#include "graphic.h"
#include "stdbool.h"
#include "mem.h"
#include "asmlib.h"
#include "serial.h"
#include "timer.h"
#include "iframe.h"

#define PROCESS_NAME_LENGTH 16
#define PROCESS_COUNT 16

typedef enum ProcessStatus{
    NOALLOC,
    INIT,
    RUNNABLE,
    RUNNING,
    EXIT
} ProcessStatus;

typedef struct Process{
    IntrFrame *iframe;
    Context *context;
    uint8_t *stack;
    ProcessStatus status;
    V_MEMMAN *memman;
    char name[PROCESS_NAME_LENGTH];
} Process;

typedef struct Scheduler{
    Process *sched_proc;
    Process proc[PROCESS_COUNT];
} Scheduler;

typedef struct Cpu{
    Process *proc;
    Scheduler sched;
} Cpu;


void task_ring3(void);
void init_mtask();


Process *proc_alloc(void);
void ktask_init(Process *proc, char *name, void (*func)(void));
void sched(void);
void sched_handler(void);
void ktask_exit(void);
void ktask_kill(Process *proc);

#endif
