#ifndef MEM_H
#define MEM_H

#include "kernel.h"

#define PTE_P           0x00000001
#define PTE_RW          0x00000002
#define PTE_US          0x00000004
#define PTE_PWT         0x00000008
#define PTE_PCD         0x00000010
#define PTE_A           0x00000020
#define PTE_D           0x00000040
#define PTE_PAT         0x00000080
#define PTE_G           0x00000100
#define PTE_IGNORE      0x00000E00
#define PTE_FRAME_ADDR  0xFFFFF000
#define _PTE_FLAGS      0x000001FF

#define PDE_P           0x00000001
#define PDE_RW          0x00000002
#define PDE_US          0x00000004
#define PDE_PWT         0x00000008
#define PDE_PCD         0x00000010
#define PDE_A           0x00000020
#define PDE_PS          0x00000080
#define PDE_G           0x00000100
#define PDE_IGNORE      0x00000E00
#define PDE_PT_ADDR     0xFFFFF000
#define _PDE_FLAGS      0x000001BF

#define PD_PDE_INDEX    0xFFC00000
#define PT_PTE_INDEX    0x003FF000

typedef struct PHY_MEMMAN{
    char tbl[MAX_PHY_MEM_PAGE];
    unsigned int base_addr;
} PHY_MEMMAN;

typedef unsigned int PTE;
typedef unsigned int PDE;
int get_paging_status(void);
void set_pte(PTE *pte, unsigned int addr, unsigned int flags);
void set_pte_flag(PTE *pte, unsigned int flags);
void set_pde(PDE *pde, PTE *pt, unsigned int flags);
void set_pde_flag(PDE *pde, unsigned int flags);

PHY_MEMMAN *get_phy_memman();
void init_phy_memman(unsigned int base_addr);
unsigned int pmalloc_4k(void);
void map_memory_4k(PDE *pdt, unsigned int virtual_addr, unsigned int physical_addr);

#endif
