#include "mem.h"

typedef unsigned int PTE;
typedef unsigned int PDE;

void set_pte(PTE *pte, unsigned int addr, unsigned int flags){
    *pte = 0;
    *pte |= (addr & PTE_FRAME_ADDR);
    *pte |= flags;
}
void set_pte_flag(PTE *pte, unsigned int flags){
    *pte &= _PTE_FLAGS;
    *pte |= flags;
}

void set_pde(PDE *pde, PTE *pt, unsigned int flags){
    *pde = 0;
    *pde |= ((unsigned int)pt & PDE_PT_ADDR);
    *pde |= flags;
}
void set_pde_flag(PDE *pde, unsigned int flags){
    *pde &= _PDE_FLAGS;
    *pde |= flags;
}

int get_paging_status(void){
    if((load_cr0() & CR0_FLAG_PG) != 0){
        return 1;
    }
    return 0;
}

P_MEMMAN *get_phy_memman(void){
    if(get_paging_status()){
        return (P_MEMMAN *) PMALLOC_MAN_ADDR_V;
    }
    
    return (P_MEMMAN *) PMALLOC_MAN_ADDR_P;
}

void init_pmalloc(unsigned int start_addr){
    P_MEMMAN *memman = get_phy_memman();
    memman->base_addr = start_addr;
    for(int i = 0; i < PMALLOC_MAX_PAGE; i++){
        memman->tbl[i] = 0;
    }
}

unsigned int pmalloc_4k(void){
    P_MEMMAN *memman = get_phy_memman();
    for(int i = 0; i < PMALLOC_MAX_PAGE; i++){
        if(memman->tbl[i] == 0){
            memman->tbl[i] = 1;
            return(memman->base_addr + MEM_PAGE_SIZE * i);
        }
    }
    return 0;
}

void map_memory_4k(PDE *pdt, unsigned int virtual_addr, unsigned int physical_addr){
    unsigned int pde_index  = (virtual_addr & PD_PDE_INDEX) >> 22;
    unsigned int pte_index  = (virtual_addr & PT_PTE_INDEX) >> 12;
    PTE *pt0 = (PTE *)(KERNEL_PT0 & PDE_PT_ADDR);
    PTE *pt = pt0 + (pde_index * 1024);
    
    //PTE *pt = (PTE *)(((*pdt & PDE_PT_ADDR) >> 22) + (pde_index * (sizeof(PTE) * 1024)));
    
    set_pde(pdt + pde_index, pt, PDE_P | PDE_RW);
    set_pte(pt + pte_index, physical_addr, PTE_P | PTE_RW);
}

void init_vmalloc(unsigned int start_addr){
    V_MEMMAN *memman = (V_MEMMAN *) VMALLOC_MAN_ADDR;
    memman->base_addr = start_addr;
    for(int i = 0; i < 1; i++){
        memman->tbl[i] = 0;
    }
}
