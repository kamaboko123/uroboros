#ifndef ASMLIB_H
#define ASMLIB_H

#include "stdint.h"
#include "stdbool.h"

void lgdt(uint32_t gdtr);
void lidt(uint32_t idtr);
void ltr(uint32_t tss0);
void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_wait(void);
uint32_t io_load_eflags(void);
void io_store_eflags(uint32_t flags);
void io_out8(uint32_t port, uint32_t data);
uint8_t io_in8(uint32_t port);
void enable_paging(uint32_t pdt_ddr, uint32_t new_stack_base_phy, uint32_t new_stack_base_virt);
uint32_t load_cr0(void);
bool load_int_flag(void);
void store_int_flag(bool int_flag);


void int_handler_null(void);
void int20_handler(void);
void int20_ret(void);
void int24_handler(void);
void interrupt(uint32_t num);

void magic_break(void);


typedef struct Context{
    //low address(stack top)
    //eaxやebxはここでは保持不要
    //context_switchが呼ばれる際に（必要であれば）スタックに保存されている
    uint32_t edi;
    uint32_t esi;
    uint32_t ebx;
    uint32_t ebp;
    uint32_t eip;
} Context;

void context_switch(struct Context **old_context, struct Context *new_context);


void int00_handler(void);
void int01_handler(void);
void int02_handler(void);
void int03_handler(void);
void int04_handler(void);
void int05_handler(void);
void int06_handler(void);
void int07_handler(void);
void int08_handler(void);
void int09_handler(void);
void int0a_handler(void);
void int0b_handler(void);
void int0c_handler(void);
void int0d_handler(void);
void int0e_handler(void);
void int0f_handler(void);
void int10_handler(void);
void int11_handler(void);
void int12_handler(void);
void int13_handler(void);
void int14_handler(void);
void int15_handler(void);
void int16_handler(void);
void int17_handler(void);
void int18_handler(void);
void int19_handler(void);
void int1a_handler(void);
void int1b_handler(void);
void int1c_handler(void);
void int1d_handler(void);
void int1e_handler(void);
void int1f_handler(void);

#endif
