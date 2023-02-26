#ifndef MTASK_H
#define MTASK_H

#include "global.h"
#include "graphic.h"
#include "stdbool.h"
#include "mem.h"

#define PROCESS_COUNT 16

typedef struct Context{
    //low address(stack top)
    //eaxやebxはここでは保持不要
    //context_switchが呼ばれる際に（必要であれば）スタックに保存されている
    uint32_t edi;
    uint32_t esi;
    uint32_t ebx;
    uint32_t ebp;
    uint32_t eip;
} Context;

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
    uint32_t error;
    uint32_t eip; // return address is used by iret
    uint16_t cs;  // return code segment is used by iret
    uint16_t padding5;
    uint32_t eflags; //store flags register

    //these data are pushed if interrupt occred when user mode
    uint32_t esp;
    uint16_t ss;
    uint16_t padding6;
} __attribute__((__packed__)) IntrFrame;

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
} Process;

typedef struct Scheduler{
    Process proc[PROCESS_COUNT];
} Scheduler;

typedef struct Cpu{
    Process *proc;
    Scheduler sched;
} Cpu;

void task_idle(void);
void init_mtask();

#endif
