#ifndef ASMLIB_H
#define ASMLIB_H

#include "stdint.h"

void io_hlt(void);
void io_cli(void);
uint32_t io_load_eflags(void);
void io_store_eflags(uint32_t flags);
void io_out8(uint32_t port, uint32_t data);
void enable_paging(uint32_t pdt_ddr, uint32_t new_stack_base_phy, uint32_t new_stack_base_virt);
int load_cr0(void);

#endif
