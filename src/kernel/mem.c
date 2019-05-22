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

void *pmalloc_4k(void){
    P_MEMMAN *memman = get_phy_memman();
    for(uint32_t i = 0; i < PMALLOC_MAX_PAGE; i++){
        if(memman->tbl[i] == 0){
            memman->tbl[i] = 1;
            return((void *)memman->base_addr + MEM_PAGE_SIZE * i);
        }
    }
    return 0;
}

void pfree(void *addr){
    P_MEMMAN *memman = get_phy_memman();
    uint32_t page = rounddown((uint32_t)addr, MEM_PAGE_SIZE);
    uint32_t index = (page - memman->base_addr) / MEM_PAGE_SIZE;
    memman->tbl[index] = 0;
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

uint32_t mem_npage(uint32_t size){
    return roundup(size, MEM_PAGE_SIZE) / MEM_PAGE_SIZE;
}




void init_vmem_block(V_MEM_BLOCKINFO *block){
    block->size = 0;
    block->flags = 0;
    block->next = NULL;
    block->prev = NULL;
}


void init_vmalloc(uint32_t extent_start, uint32_t init_extent_end, uint32_t max_extent_end){
    V_MEMMAN *memman = (V_MEMMAN *)VMALLOC_MAN_ADDR;
    
    //領域確保
    for(int i = 0; i < mem_npage(sizeof(V_MEMMAN)); i++){
        uint32_t mem = (uint32_t)pmalloc_4k();
        map_memory_4k((PDE *)KERNEL_PDT, VMALLOC_MAN_ADDR + (i * MEM_PAGE_SIZE), mem);
    }
    
    memman->extent_start = extent_start;
    memman->extent_end = init_extent_end;
    
    for(int i = 0; i < mem_npage(memman->extent_end - memman->extent_start); i++){
        uint32_t mem = (uint32_t)pmalloc_4k();
        map_memory_4k((PDE *)KERNEL_PDT,  memman->extent_start + (i * MEM_PAGE_SIZE), mem);
    }
    
    for(int i = 0; i < VMEM_MAX_UNITS; i++){
        init_vmem_block(&memman->blocks[i]);
    }
    memman->entry =  &memman->blocks[0];
    memman->blocks[0].flags |= VMEM_BLOCKS_USE;
    memman->blocks[0].addr = memman->extent_start;
    memman->blocks[0].size = memman->extent_end - memman->extent_start;
}

V_MEM_BLOCKINFO *get_new_block(void){
    V_MEMMAN *memman = (V_MEMMAN *)VMALLOC_MAN_ADDR;
    
    for(int i = 0; i < VMEM_MAX_UNITS; i++){
        if((memman->blocks[i].flags & VMEM_BLOCKS_USE) == 0){
            memman->blocks[i].flags |=VMEM_BLOCKS_USE;
            return &memman->blocks[i];
        }
    }
    return NULL;
}

void *vmalloc(uint32_t size){
    
    V_MEMMAN *memman = (V_MEMMAN *)VMALLOC_MAN_ADDR;
    V_MEM_BLOCKINFO *p = memman->entry;
    
    while(p->next != NULL){
        //空き領域発見
        if(p->size >= size && (p->flags & VMEM_BLOCKS_ALLOC) == 0) break;
        p = p->next;
    }
    
    //次がある
    if(p->next != NULL){
        if(p->size >= size){
            p->flags |= VMEM_BLOCKS_ALLOC;
            //次のブロックとの間に空きがあるなら、新しいブロックを作る
            if(p->size > size){
                V_MEM_BLOCKINFO *new_block =  get_new_block();
                new_block->next = p->next;
                p->next = new_block;
                new_block->prev = p;
                new_block->size = p->size - size;
                p->size = size;
                new_block->addr = p->addr + p->size;
            }
            return (void *)p->addr;
        }
    }
    //次がない
    else{
        if(p->size >= size){
            p->flags |= VMEM_BLOCKS_ALLOC;
            
            V_MEM_BLOCKINFO *new_block =  get_new_block();
            new_block->next = p->next;
            p->next = new_block;
            new_block->prev = p;
            new_block->size = p->size - size;
            p->size = size;
            new_block->addr = p->addr + p->size;
            
            return (void *)p->addr;
        }
    }
    
    return NULL;
}

void vfree(void *addr){
    V_MEMMAN *memman = (V_MEMMAN *)VMALLOC_MAN_ADDR;
    V_MEM_BLOCKINFO *p = memman->entry;
    
    while(p != NULL){
        if(p->addr == (uint32_t)addr && (p->flags & VMEM_BLOCKS_ALLOC) != 0){
            break;
        }
        p = p->next;
    }
    if(p == NULL) return;
    
    //pは解放対象
    
    p->flags &= ~VMEM_BLOCKS_ALLOC;
    
    //次が空きだったらつなげる
    if(p->next != NULL && (p->next->flags & VMEM_BLOCKS_ALLOC) == 0){
        V_MEM_BLOCKINFO *next_block = (p->next);
        p->next = next_block->next;
        p->size += next_block->size;
        //次の先があれば繋ぎ変え
        if(next_block->next != NULL){
            next_block->next->prev = p;
        }
        //つなげたら要らなくなるので初期化しておく
        init_vmem_block(next_block);
    }
    //前が空きだったらつなげる
    if(p->prev != NULL && (p->prev->flags & VMEM_BLOCKS_ALLOC) == 0){
        V_MEM_BLOCKINFO *prev_block = (p->prev);
        prev_block->next = p->next;
        prev_block->size += p->size;
        
        //先があればprevにつなげておく
        if(p->next != NULL){
            p->next->prev = prev_block;
        }
        
        init_vmem_block(p);
    }
}
