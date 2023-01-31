#ifndef PIT_H
#define PIT_H

#include "asmlib.h"

//IOポートアドレス
#define PORT_PIT_COUNTER0 0x40
#define PORT_PIT_COUNTER1 0x41
#define PORT_PIT_COUNTER2 0x42
#define PORT_PIT_CONTROL 0x43

// BCDモード有効
#define PIT_CW_BCD 0x01

//カウンタモード
#define PIT_CW_MODE0 0x00 << 1
#define PIT_CW_MODE1 0x01 << 1
#define PIT_CW_MODE2 0x02 << 1
#define PIT_CW_MODE3 0x03 << 1
#define PIT_CW_MODE4 0x04 << 1
#define PIT_CW_MODE5 0x05 << 1
#define PIT_CW_MODE6 0x06 << 1
#define PIT_CW_MODE7 0x07 << 1

//読み込み
#define PIT_CW_RL_LOAD 0x00 << 4
//書き込み下位バイト
#define PIT_CW_RL_WRITE_LSBYTE 0x01 << 4
//書き込み上位バイト
#define PIT_CW_RL_WRITE_MSBYTE 0x02 << 4
//書き込み順番: 下位->上位
#define PIT_CW_RL_WRITE 0x03 << 4

//カウンタセレクト
#define PIT_CW_SC_COUNTER0 0x00 <<  6
#define PIT_CW_SC_COUNTER1 0x01 <<  6
#define PIT_CW_SC_COUNTER2 0x02 <<  6

void init_pit(uint16_t c0_freq);

#endif
