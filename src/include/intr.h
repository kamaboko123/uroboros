#ifndef INTR_H
#define INTR_H

#include "config.h"
#include "asmlib.h"
#include "timer.h"
#include "serial.h"
#include "iframe.h"

#define IO_PORT_PIC1_COMMAND 0x20
#define IO_PORT_PIC1_DATA 0x21
#define IO_PORT_PIC2_COMMAND 0xA0
#define IO_PORT_PIC2_DATA 0xA1
#define IO_PORT_PIC1_OCW2 0x20

//非選択EOI
#define PIC_OCW2_CMD_EOI 0x20

#define PIC_ICW1_BASE 0x10
#define PIC_ICW1_ICW4 0x01

#define PIC_ICW4_8086 0x01

//PIC
#define PIC_IMR_IRQ0 1 << 0
#define PIC_IMR_IRQ1 1 << 1
#define PIC_IMR_IRQ2 1 << 2
#define PIC_IMR_IRQ3 1 << 3
#define PIC_IMR_IRQ4 1 << 4
#define PIC_IMR_IRQ5 1 << 5
#define PIC_IMR_IRQ6 1 << 6
#define PIC_IMR_IRQ7 1 << 7
#define PIC_IMR_IRQ8 1 << 8
#define PIC_IMR_IRQ9 1 << 9
#define PIC_IMR_IR10 1 << 10
#define PIC_IMR_IR11 1 << 11
#define PIC_IMR_IR12 1 << 12
#define PIC_IMR_IR13 1 << 13
#define PIC_IMR_IR14 1 << 14
#define PIC_IMR_IR15 1 << 15
#define PIC_IMR_IR16 1 << 16

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

typedef struct IDTR{
    uint16_t size;
    uint32_t base;
} __attribute__((__packed__))IDTR;

void init_pic(uint16_t imr, uint32_t intr_vec_base);
void init_idt(IDT *idt0, IDTR *idtr);
void set_idt(IDT *idt0, uint8_t vec_num, void (*handler)(void));


void int_handler(IntrFrame iframe);


#endif
