#ifndef MEM_H
#define MEM_H

#include "kernel.h"
#include "stdbool.h"

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

#define MEM_PAGE_SIZE 4096

typedef struct P_MEMMAN{
    uint8_t tbl[PMALLOC_MAX_PAGE];
    uint32_t base_addr;
} P_MEMMAN;

typedef struct V_MEMMAN{
    uint8_t tbl[VMALLOC_MAX_PAGE];
    uint32_t base_addr;
} V_MEMMAN;

typedef uint32_t PTE;
typedef uint32_t PDE;
bool get_paging_status(void);
void set_pte(PTE *pte, uint32_t addr, uint32_t flags);
void set_pte_flag(PTE *pte, uint32_t flags);
void set_pde(PDE *pde, PTE *pt, uint32_t flags);
void set_pde_flag(PDE *pde, uint32_t flags);
void map_memory_4k(PDE *pdt, uint32_t virtual_addr, uint32_t physical_addr);

P_MEMMAN *get_phy_memman();
void init_pmalloc(uint32_t start_addr);
void *pmalloc_4k(void);
void pfree(void *addr);

uint32_t mem_npage(uint32_t size);
void init_vmalloc(uint32_t start_addr);
uint32_t vmalloc_4k(void);

#endif
