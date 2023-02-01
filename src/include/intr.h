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


#define IDT_FLAG_INT_GATE 0x6
#define IDT_FLAG_D_32 1 << 3
#define IDT_FLAG_DPL_0 0 << 4
#define IDT_FLAG_DPL_1 1 << 4
#define IDT_FLAG_DPL_2 2 << 4
#define IDT_FLAG_DPL_3 3 << 4
#define IDT_FLAG_P 1 << 7

typedef struct IDT{
    uint16_t base_lo;
    uint16_t sel;
    uint8_t reserved;
    uint8_t flags;
    uint16_t base_hi;
} __attribute__((__packed__)) IDT;

void init_pic(uint16_t imr, uint32_t intr_vec_base);
void init_idt();
void set_idt(uint8_t vec_num, void (*func)(void));

#endif
