#ifndef ASMLIB_H
#define ASMLIB_H

#include "stdint.h"

void lgdt(uint32_t gdtr);
void lidt(uint32_t idtr);
void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_wait(void);
uint32_t io_load_eflags(void);
void io_store_eflags(uint32_t flags);
void io_out8(uint32_t port, uint32_t data);
uint8_t io_in8(uint32_t port);
void enable_paging(uint32_t pdt_ddr, uint32_t new_stack_base_phy, uint32_t new_stack_base_virt);
int load_cr0(void);

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
#endif
