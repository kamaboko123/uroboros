#ifndef ASMLIB_H
#define ASMLIB_H

#include "stdint.h"

void io_hlt(void);
void io_cli(void);
int io_load_eflags(void);
int io_store_eflags(int32_t flags);
void io_out8(int32_t port, int32_t data);
void enable_paging(uint32_t pdt_ddr, uint32_t new_stack_base_phy, uint32_t new_stack_base_virt);
int load_cr0(void);

#endif
