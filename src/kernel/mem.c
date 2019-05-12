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

void set_pde(PDE *pde, unsigned int addr, unsigned int flags){
    *pde = 0;
    *pde |= (addr & PDE_PT_ADDR);
    *pde |= flags;
}
void set_pde_flag(PDE *pde, unsigned int flags){
    *pde &= _PDE_FLAGS;
    *pde |= flags;
}

