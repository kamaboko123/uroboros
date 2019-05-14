#include "mem.h"
#include "asmlib.h"

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
    if((load_cr0() & 0x80000000) != 0){
        return 1;
    }
    return 0;
}

PHY_MEMMAN *get_phy_memman(void){
    if(get_paging_status()){
        return (PHY_MEMMAN *) PHY_MEM_MAN_ADDR_PG;
    }
    
    return (PHY_MEMMAN *) PHY_MEM_MAN_ADDR;
}

void init_phy_memman(unsigned int base_addr){
    PHY_MEMMAN *memman = get_phy_memman();
    memman->base_addr = base_addr;
    for(int i = 0; i < MAX_PHY_MEM_PAGE; i++){
        memman->tbl[i] = 0;
    }
}

unsigned int alloc_phy_mem_4k(void){
    PHY_MEMMAN *memman = get_phy_memman();
    for(int i = 0; i < MAX_PHY_MEM_PAGE; i++){
        if(memman->tbl[i] == 0){
            memman->tbl[i] = 1;
            return(memman->base_addr + 4096 * i);
        }
    }
    return 0;
}

void map_memory_4k(PDE *pdt, unsigned int virtual_addr, unsigned int physical_addr){
    unsigned int pde_index  = (virtual_addr & PD_PDE_INDEX) >> 22;
    unsigned int pte_index  = (virtual_addr & PT_PTE_INDEX) >> 12;
    PTE *pt = (PTE *)(((*pdt & PDE_PT_ADDR) >> 22) + (pde_index * (sizeof(PTE) * 1024)));
    
    set_pde(pdt + pde_index, pt, PDE_P | PDE_RW);
    set_pte(pt + pte_index, physical_addr, PTE_P | PTE_RW);
}
