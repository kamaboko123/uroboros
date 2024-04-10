#ifndef GLOBAL_H
#define GLOBAL_H

#include "stdint.h"
#include "stdbool.h"

typedef struct Queue8 Queue8;
typedef struct TimerCtl TimerCtl;
typedef struct Cpu Cpu;
typedef struct Console Console;
typedef struct Timer Timer;

typedef struct System{
    //コンソール(Serial port)
    Queue8 *com1_in;
    Queue8 *com1_out;
    //タスクスイッチ用タイマ
    Queue8 *task_timer;
    
    //Console
    Console *console1;

    //タイマ管理
    TimerCtl *timerctl;

    //CPU構造体(multitask)
    Cpu *cpu;

    //FDCの割り込みフラグ
    bool fdc_intr;

    uint8_t *vram;
} System;

extern System *sys;


void task_ring3(void);
void task_a(void);
void task_b(void);
void task_fdc(void);
void test_task(char *str, int interval);
void task_console(void);



#endif
