#ifndef MTASK_H
#define MTASK_H

#include "global.h"
#include "graphic.h"
#include "stdbool.h"
#include "mem.h"
#include "asmlib.h"
#include "serial.h"
#include "timer.h"

#define PROCESS_NAME_LENGTH 16
#define PROCESS_COUNT 16

typedef struct IntrFrame{
    //low address(stack top)
    
    //pushed by pusha in int_handler
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t org_esp; //original esp
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    //pushed by int_handler
    uint16_t gs;
    uint16_t padding1;
    uint16_t fs;
    uint16_t padding2;
    uint16_t es;
    uint16_t padding3;
    uint16_t ds;
    uint16_t padding4;
    
    //pushed by hardware when interrupt
    //these data are used for return from interrupt handler
    
    //uint32_t error; //一部の割り込み時にはerror codeがpushされるが、今はint20を前提にしておりint20ではerror codeはないので不要
    uint32_t eip; // return address is used by iret
    uint16_t cs;  // return code segment is used by iret
    uint16_t padding5;
    uint32_t eflags; //store flags register

    //these data are pushed if interrupt occred when user mode
    uint32_t esp;
    uint16_t ss;
    uint16_t padding6;
} IntrFrame;

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
    ProcessStatus status;
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

void task_idle(void);
void init_mtask();


Process *proc_alloc(void);
void ktask_init(Process *proc, char *name, void (*func)(void));
void sched(void);
void sched_handler(void);

extern SystemQueue *SYSQ;

#endif
