#include "mem.h"

typedef uint32_t PTE;
typedef uint32_t PDE;

void set_pte(PTE *pte, uint32_t addr, uint32_t flags){
    *pte = 0;
    *pte |= (addr & PTE_FRAME_ADDR);
    *pte |= flags;
}
void set_pte_flag(PTE *pte, uint32_t flags){
    *pte &= _PTE_FLAGS;
    *pte |= flags;
}

void set_pde(PDE *pde, PTE *pt, uint32_t flags){
    *pde = 0;
    *pde |= ((uint32_t)pt & PDE_PT_ADDR);
    *pde |= flags;
}
void set_pde_flag(PDE *pde, uint32_t flags){
    *pde &= _PDE_FLAGS;
    *pde |= flags;
}

bool get_paging_status(void){
    if((load_cr0() & CR0_FLAG_PG) != 0){
        return true;
    }
    return false;
}

P_MEMMAN *get_phy_memman(void){
    if(get_paging_status()){
        return (P_MEMMAN *) PMALLOC_MAN_ADDR_V;
    }
    
    return (P_MEMMAN *) PMALLOC_MAN_ADDR_P;
}

void init_pmalloc(uint32_t start_addr){
    P_MEMMAN *memman = get_phy_memman();
    memman->base_addr = start_addr;
    for(uint32_t i = 0; i < PMALLOC_MAX_PAGE; i++){
        memman->tbl[i] = 0;
    }
}

uint32_t pmalloc_4k(void){
    P_MEMMAN *memman = get_phy_memman();
    for(uint32_t i = 0; i < PMALLOC_MAX_PAGE; i++){
        if(memman->tbl[i] == 0){
            memman->tbl[i] = 1;
            return(memman->base_addr + MEM_PAGE_SIZE * i);
        }
    }
    return 0;
}

void map_memory_4k(PDE *pdt, uint32_t virtual_addr, uint32_t physical_addr){
    uint32_t pde_index  = (virtual_addr & PD_PDE_INDEX) >> 22;
    uint32_t pte_index  = (virtual_addr & PT_PTE_INDEX) >> 12;
    PTE *pt0 = (PTE *)(KERNEL_PT0 & PDE_PT_ADDR);
    PTE *pt = pt0 + (pde_index * 1024);
    
    //PTE *pt = (PTE *)(((*pdt & PDE_PT_ADDR) >> 22) + (pde_index * (sizeof(PTE) * 1024)));
    
    set_pde(pdt + pde_index, pt, PDE_P | PDE_RW);
    set_pte(pt + pte_index, physical_addr, PTE_P | PTE_RW);
}

void init_vmalloc(uint32_t start_addr){
    V_MEMMAN *memman = (V_MEMMAN *) VMALLOC_MAN_ADDR;
    memman->base_addr = start_addr;
    for(uint32_t i = 0; i < VMALLOC_MAX_PAGE; i++){
        memman->tbl[i] = 0;
    }
}

uint32_t vmalloc_4k(void){
    V_MEMMAN *memman = (V_MEMMAN *) VMALLOC_MAN_ADDR;
    for(uint32_t i = 0; i < PMALLOC_MAX_PAGE; i++){
        if(memman->tbl[i] == 0){
            memman->tbl[i] = 1;
            uint32_t v = memman->base_addr + MEM_PAGE_SIZE * i;
            uint32_t p = pmalloc_4k();
            map_memory_4k((PDE *)KERNEL_PDT, v, p);
            return v;
        }
    }
    return 0;
}

uint32_t mem_npage(uint32_t size){
    return roundup(size, MEM_PAGE_SIZE) / MEM_PAGE_SIZE;
}
