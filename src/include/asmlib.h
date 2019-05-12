#ifndef ASMLIB_H
#define ASMLIB_H

void io_hlt(void);
void io_cli(void);
int io_load_eflags(void);
int io_store_eflags(int flags);
void io_out8(int port, int data);
void enable_paging(unsigned int pdt_ddr);
int load_cr0(void);

#endif