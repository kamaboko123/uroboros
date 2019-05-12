#include "mem.h"
#include "asmlib.h"

typedef unsigned int PTE;
typedef unsigned int PDE;

int get_paging_status(void){
    if((load_cr0() & 0x80000000) != 0){
        return 1;
    }
    return 0;
}

void set_pte(PTE *pte, unsigned int addr, unsigned int flags){
    *pte = 0;
    *pte |= (addr & PTE_FRAME_ADDR);
    *pte |= flags;
}
void set_pte_flag(PTE *pte, unsigned int flags){
    *pte &= _PTE_FLAGS;
    *pte |= flags;
}

void set_pde(PDE *pde, unsigned int addr, unsigned int flags){
    *pde = 0;
    *pde |= (addr & PDE_PT_ADDR);
    *pde |= flags;
}
void set_pde_flag(PDE *pde, unsigned int flags){
    *pde &= _PDE_FLAGS;
    *pde |= flags;
}

void init_phy_memman(unsigned int base_addr){
    PHY_MEMMAN *memman;
    if(get_paging_status()){
        memman = (PHY_MEMMAN *) PHY_MEM_MAN_ADDR;
    }
    else{
        memman = (PHY_MEMMAN *) PHY_MEM_MAN_ADDR_PG;
    }
    
    for(int i; i < MAX_PHY_MEM_PAGE; i++){
        memman->tbl[i] = 0;
    }
}

unsigned int alloc_page(){
    
}
