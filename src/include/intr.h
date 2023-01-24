#ifndef INTR_H
#define INTR_H

#include "config.h"
#include "asmlib.h"

#define IO_PORT_PIC1_COMMAND 0x20
#define IO_PORT_PIC1_DATA 0x21
#define IO_PORT_PIC2_COMMAND 0xA0
#define IO_PORT_PIC2_DATA 0xA1

#define PIC_ICW1_BASE 0x10
#define PIC_ICW1_ICW4 0x01

#define PIC_ICW4_8086 0x01

// Master PICのどこにSlave PICがつながっているか
// Master側とSlave側でフォーマットが異なる
// Master側 00000100 (IR2につながっているので2bit目を立てる)
#define PIC_ICW3_MASTER 0x04
// Slave側 00000010 (Master側の番号をそのまま2進表記)
#define PIC_ICW3_SLAVE  0x02

void init_pic(uint16_t imr, uint32_t intr_vec_base);

#endif
