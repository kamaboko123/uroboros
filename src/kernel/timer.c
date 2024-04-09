#include "timer.h"

void init_pit(uint16_t c0_freq){
    io_out8(PORT_PIT_CONTROL, PIT_CW_MODE2 | PIT_CW_RL_WRITE | PIT_CW_SC_COUNTER0);
    io_out8(PORT_PIT_COUNTER0, c0_freq & 0xff);
    io_out8(PORT_PIT_COUNTER0, c0_freq >> 8);
}

void timer_init(){
    sys->timerctl = (TimerCtl *) kvmalloc(sizeof(TimerCtl));

    // dummy
    sys->timerctl->t = (Timer *)kvmalloc(sizeof(Timer));
    sys->timerctl->t->next = NULL;
    sys->timerctl->t->prev = NULL;
    sys->timerctl->t->q = NULL;
    sys->timerctl->t->interval = 0;
    sys->timerctl->t->count = 0;
    sys->timerctl->t->mode = 0;
}

Timer *timer_alloc(Queue8 *q, uint32_t interval, uint8_t mode){
    //自動的にタイマを作動させたくない場合は、この関数を呼ぶ前にqueueになにかデータを入れておく
    bool iflag = load_int_flag();
    io_cli();
    Timer *t = sys->timerctl->t;
    
    while(t->next != NULL) t = t->next;
    Timer *new_timer = (Timer *)kvmalloc(sizeof(Timer));
    
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

void timer_reset(Timer *t){
    uint32_t interval = t->interval;
    t->interval = 0;
    t->count = interval;

    while(!q8_empty(t->q)){
        q8_de(t->q);
    }
    t->interval = interval;
}

void sleep(uint32_t tick){
    Queue8 *q = q8_make(10, 0);
    Timer *t = timer_alloc(q, tick, TIMER_MODE_ONESHOT);
    timer_reset(t);
    while(q8_empty(q)) io_hlt();
    timer_free(t);
    q8_free(q);
}


void timer_free(Timer *t){
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

void timer_tick(){
    for(Timer *t = sys->timerctl->t; t != NULL; t = t->next){
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
