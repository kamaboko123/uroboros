#include "timer.h"

void init_pit(uint16_t c0_freq){
    uint16_t c = c0_freq;

    io_out8(PORT_PIT_CONTROL, PIT_CW_MODE2 | PIT_CW_RL_WRITE | PIT_CW_SC_COUNTER0);
    io_out8(PORT_PIT_COUNTER0, c & 0xff);
    io_out8(PORT_PIT_COUNTER0, c >> 8);
}
