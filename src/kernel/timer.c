#include "timer.h"

extern SystemQueue *SYSQ;
extern TIMERCTL *timerctl;

void init_pit(uint16_t c0_freq){
    io_out8(PORT_PIT_CONTROL, PIT_CW_MODE2 | PIT_CW_RL_WRITE | PIT_CW_SC_COUNTER0);
    io_out8(PORT_PIT_COUNTER0, c0_freq & 0xff);
    io_out8(PORT_PIT_COUNTER0, c0_freq >> 8);
}

void init_timer(){
    timerctl =(TIMERCTL *) kvmalloc(sizeof(TIMERCTL));

    // dummy
    timerctl->t = (TIMER *)kvmalloc(sizeof(TIMER));
    timerctl->t->next = NULL;
    timerctl->t->prev = NULL;
    timerctl->t->q = NULL;
    timerctl->t->interval = 0;
    timerctl->t->count = 0;
    timerctl->t->mode = 0;
}

TIMER *alloc_timer(Queue8 *q, uint32_t interval, uint8_t mode){
    //自動的にタイマを作動させたくない場合は、この関数を呼ぶ前にqueueになにかデータを入れておく
    bool iflag = load_int_flag();
    io_cli();
    TIMER *t = timerctl->t;
    
    while(t->next != NULL) t = t->next;
    TIMER *new_timer = (TIMER *)kvmalloc(sizeof(TIMER));
    
    new_timer->next = NULL;
    new_timer->prev = t;
    new_timer->q = q;
    new_timer->interval = interval;
    new_timer->count = interval;
    new_timer->mode = mode;
    t->next = new_timer;
    store_int_flag(iflag);

    return new_timer;
}

void free_timer(TIMER *t){
    bool iflag = load_int_flag();
    io_cli();

    if(t->next == NULL){
        t->prev->next = t->next; //NULL
    }
    else{
        //次がある場合は繋ぎ変え
        t->prev->next = t->next;
        t->next->prev = t->prev;
    }
    //先頭にはdummyのtimerがあるので、前がないケースは考慮不要
    kvfree(t);
    store_int_flag(iflag);
}

void tick_timer(){
    for(TIMER *t = timerctl->t; t != NULL; t = t->next){
        if(t->interval == 0) continue;
        
        if((t->mode == TIMER_MODE_ONESHOT) && !q8_empty(t->q)){
            t->count = t->interval;
            return;
        }

        t->count--;
        if(t->count == 0){
            q8_in(t->q, 1);
            t->count = t->interval;
        }
    }
}
