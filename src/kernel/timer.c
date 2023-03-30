#include "timer.h"

extern SystemQueue *SYSQ;
extern TIMERCTL *timerctl;

void init_pit(uint16_t c0_freq){
    uint16_t c = c0_freq;

    io_out8(PORT_PIT_CONTROL, PIT_CW_MODE2 | PIT_CW_RL_WRITE | PIT_CW_SC_COUNTER0);
    io_out8(PORT_PIT_COUNTER0, c & 0xff);
    io_out8(PORT_PIT_COUNTER0, c >> 8);
}

void int_handler_pit(void){
    io_out8(IO_PORT_PIC1_OCW2, PIC_OCW2_CMD_EOI);
    tick_timer();

    if(!q8_empty(SYSQ->task_timer)){
        q8_de(SYSQ->task_timer);
        sched_handler();
    }
}


void init_timer(){
    timerctl =(TIMERCTL *) vmalloc(sizeof(TIMERCTL));

    // dummy
    timerctl->t = (TIMER *)vmalloc(sizeof(TIMER));
    timerctl->t->next = NULL;
    timerctl->t->prev = NULL;
    timerctl->t->q = NULL;
    timerctl->t->interval = 0;
    timerctl->t->count = 0;
}

TIMER *alloc_timer(Queue8 *q, uint32_t interval){
    TIMER *t = timerctl->t;
    
    while(t->next != NULL);
    TIMER *new_timer = (TIMER *)vmalloc(sizeof(TIMER));
    
    new_timer->next = NULL;
    new_timer->prev = t;
    new_timer->q = q;
    new_timer->interval = interval;
    new_timer->count = 0;
    t->next = new_timer;

    return new_timer;
}

void tick_timer(){
    for(TIMER *t = timerctl->t; t != NULL; t = t->next){
        if(t->interval == 0) continue;
        t->count++;
        if(t->interval == t->count){
            q8_in(t->q, 1);
            t->count = 0;
        }
    }
}
