#ifndef MEM_H
#define MEM_H

#include "stdint.h"
#include "stdbool.h"
#include "config.h"
#include "stdlibc.h"
#include "asmlib.h"

//paging enable flag
#define CR0_FLAG_PG 0x80000000

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

typedef struct P_MEM_PAGE{
    uint8_t flags;
    uint32_t addr;
    //vmallocで使用される
    struct P_MEM_PAGE *next;
} P_MEM_PAGE;

typedef struct P_MEMMAN{
    P_MEM_PAGE tbl[PMALLOC_MAX_PAGE];
    uint32_t base_addr;
} P_MEMMAN;

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
P_MEM_PAGE *pmalloc_4k_page(void);
void pfree(void *addr);

uint32_t mem_npage(uint32_t size);

#define VMEM_MAX_UNITS 256
#define VMEM_BLOCKS_USE 0x01
#define VMEM_BLOCKS_ALLOC 0x02


typedef struct V_MEM_BLOCKINFO{
    uint8_t flags;
    uint32_t addr;
    uint32_t size;
    P_MEM_PAGE *p_mem;
    struct V_MEM_BLOCKINFO *next;
    struct V_MEM_BLOCKINFO *prev;
}V_MEM_BLOCKINFO;

typedef struct V_MEMMAN{
    uint32_t extent_start;
    uint32_t extent_end;
    uint32_t extent_max;
    uint32_t pdt;
    V_MEM_BLOCKINFO blocks[VMEM_MAX_UNITS];
    V_MEM_BLOCKINFO *entry;
}V_MEMMAN;



void init_vmem_block(V_MEM_BLOCKINFO *block);
void init_kvmalloc(uint32_t extent_start, uint32_t init_extend_end, uint32_t max_extent_end);
void init_vmalloc(V_MEMMAN *memman, uint32_t extent_start, uint32_t init_extend_end, uint32_t max_extent_end, uint32_t pdt);
void *vmalloc(V_MEMMAN *memman, uint32_t size);
void vfree(V_MEMMAN *memman, void *addr);
void *kvmalloc(uint32_t size);
void kvfree(void *addr);

void init_kernel_mem(void);


//segment descripter
typedef struct GDT_SEG_DESC{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint16_t type: 4;
    uint16_t s: 1;
    uint16_t dpl: 2;
    uint16_t p: 1;
    uint16_t limit_high: 4;
    uint16_t avl: 1;
    uint16_t zero: 1;
    uint16_t db: 1;
    uint16_t g: 1;
    uint8_t base_high;
} __attribute__((__packed__))GDT_SEG_DESC;

typedef struct GDTR{
    uint16_t size;
    uint32_t base;
} __attribute__((__packed__))GDTR;

#define GDT_TYPE_DATA 0 << 3
#define GDT_TYPE_CODE 1 << 3
#define GDT_TYPE_CODE_CONFORM 1 << 2
#define GDT_TYPE_DATA_DOWN 1 << 2
#define GDT_TYPE_CODE_RE 1 << 1
#define GDT_TYPE_DATA_RW 1 << 1

// system segment
#define GDT_TYPE_TSS32 9

void init_gdt(GDT_SEG_DESC *gdt, GDTR *gdtr);


typedef struct TSS32{
    uint16_t link;
    uint16_t reserved0;
    
    uint32_t esp0;
    uint16_t ss0;
    uint16_t reserved1;

    uint32_t esp1;
    uint16_t ss1;
    uint16_t reserved2;

    uint32_t esp2;
    uint16_t ss2;
    uint16_t reserved3;

    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;

    uint16_t es;
    uint16_t reserved4;
    uint16_t cs;
    uint16_t reserved5;
    uint16_t ss;
    uint16_t reserved6;
    uint16_t ds;
    uint16_t reserved7;
    uint16_t fs;
    uint16_t reserved8;
    uint16_t gs;

    uint16_t ldtr;
    uint16_t reserved9;
    uint16_t reserved10;
    uint16_t iopb;
    uint32_t ssp;
} __attribute__((__packed__)) TSS32;

void init_tss(TSS32 *tss0, GDT_SEG_DESC *tss0_desc, GDTR *gdtr);

#endif
