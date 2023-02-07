#include "timer.h"

extern SystemQueue *SYSQ;

void init_pit(uint16_t c0_freq){
    uint16_t c = c0_freq;

    io_out8(PORT_PIT_CONTROL, PIT_CW_MODE2 | PIT_CW_RL_WRITE | PIT_CW_SC_COUNTER0);
    io_out8(PORT_PIT_COUNTER0, c & 0xff);
    io_out8(PORT_PIT_COUNTER0, c >> 8);
}

void int_handler_pit(void){
    q8_in(SYSQ->timer, 1);
    io_out8(IO_PORT_PIC1_OCW2, PIC_OCW2_CMD_EOI);
}
