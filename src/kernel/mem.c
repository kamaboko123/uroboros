#include "mem.h"
#include "serial.h"

typedef uint32_t PTE;
typedef uint32_t PDE;

void init_pdt(PTE *pde){
    for(int i = 0; i < 1024; i++){
        *(pde + i) = 0;
    }
}

void init_pt(PTE *pte){
    for(int i = 0; i < 1024; i++){
        *(pte + i) = 0;
    }
}

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
        memman->tbl[i].flags = 0;
        memman->tbl[i].addr = memman->base_addr + (i * MEM_PAGE_SIZE);
        memman->tbl[i].next = NULL;
    }
}

void *pmalloc_4k(void){
    return (void *)pmalloc_4k_page()->addr;
}

P_MEM_PAGE *pmalloc_4k_page(void){
    P_MEMMAN *memman = get_phy_memman();
    for(uint32_t i = 0; i < PMALLOC_MAX_PAGE; i++){
        if(memman->tbl[i].flags == 0){
            memman->tbl[i].flags = 1;
            return (memman->tbl + i);
        }
    }
    return 0;
}

void pfree(void *addr){
    P_MEMMAN *memman = get_phy_memman();
    uint32_t page = rounddown((uint32_t)addr, MEM_PAGE_SIZE);
    uint32_t index = (page - memman->base_addr) / MEM_PAGE_SIZE;
    memman->tbl[index].flags = 0;
}

void map_memory_4k(PDE *pdt, uint32_t virtual_addr, uint32_t physical_addr){
    uint32_t pde_index  = (virtual_addr & PD_PDE_INDEX) >> 22;
    uint32_t pte_index  = (virtual_addr & PT_PTE_INDEX) >> 12;
    PTE *pt0 = (PTE *)(KERNEL_PT0 & PDE_PT_ADDR);
    PTE *pt = pt0 + (pde_index * 1024);
    
    set_pde(pdt + pde_index, pt, PDE_P | PDE_RW | PDE_US);
    set_pte(pt + pte_index, physical_addr, PTE_P | PTE_RW | PTE_US);
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


void init_kvmalloc(uint32_t extent_start, uint32_t init_extent_end, uint32_t max_extent_end){
    V_MEMMAN *memman = (V_MEMMAN *)VMALLOC_MAN_ADDR;
    init_vmalloc(memman, extent_start, init_extent_end, max_extent_end);
}

void init_vmalloc(V_MEMMAN *memman, uint32_t extent_start, uint32_t init_extent_end, uint32_t max_extent_end){
    //管理用の領域確保
    for(int i = 0; i < mem_npage(sizeof(V_MEMMAN)); i++){
        uint32_t mem = (uint32_t)pmalloc_4k();
        map_memory_4k((PDE *)KERNEL_PDT, VMALLOC_MAN_ADDR + (i * MEM_PAGE_SIZE), mem);
    }

    //アラインメントを揃える
    //memman->extent_start = roundup(extent_start, VMALLOC_ALIGNMENT);
    //memman->extent_end = roundup(init_extent_end, VMALLOC_ALIGNMENT);
    //memman->extent_max = roundup(max_extent_end, VMALLOC_ALIGNMENT);
    memman->extent_start = extent_start;
    memman->extent_end = init_extent_end;
    memman->extent_max = max_extent_end;

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

void *kvmalloc(uint32_t size){
    V_MEMMAN *memman = (V_MEMMAN *)VMALLOC_MAN_ADDR;
    return vmalloc(memman, size);
}

void *vmalloc(V_MEMMAN *memman, uint32_t size){
    
    V_MEM_BLOCKINFO *p = memman->entry;
    
    //アラインメントを揃える
    size = roundup(size, MEM_PAGE_SIZE);
    
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

                P_MEM_PAGE *p_mem = pmalloc_4k_page();
                p->p_mem = p_mem;
                map_memory_4k((PDE *)KERNEL_PDT,  p->addr, p_mem->addr);
                for(int i = 1; i < mem_npage(size); i++){
                    P_MEM_PAGE *new_page= pmalloc_4k_page();
                    p_mem->next = new_page;
                    p_mem = new_page;
                    map_memory_4k((PDE *)KERNEL_PDT,  p->addr + (MEM_PAGE_SIZE * i), p_mem->addr);
                }
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
            
            P_MEM_PAGE *p_mem = pmalloc_4k_page();
            p->p_mem = p_mem;
            map_memory_4k((PDE *)KERNEL_PDT, p->addr, p_mem->addr);
            for(int i = 1; i < mem_npage(size); i++){
                P_MEM_PAGE *new_page= pmalloc_4k_page();
                p_mem->next = new_page;
                p_mem = new_page;
                map_memory_4k((PDE *)KERNEL_PDT,  p->addr + (MEM_PAGE_SIZE * i), p_mem->addr);
            }
            return (void *)p->addr;
        }
    }
    
    /*
    //空き領域がないので、要求サイズの分だけエクステントを拡張する
    for(int i = 0; i < mem_npage(size - p->size); i++){
        //拡張上限なら確保失敗
        if(memman->extent_end >= memman->extent_max) return NULL;
        uint32_t mem = (uint32_t)pmalloc_4k();
        //物理メモリがなければ確保失敗
        if(mem == 0) return NULL;
        p->size += MEM_PAGE_SIZE;
        map_memory_4k((PDE *)KERNEL_PDT, p->addr + (i * MEM_PAGE_SIZE), mem);
    }
    */
    
    return vmalloc(memman, size);
}

void kvfree(void *addr){
    V_MEMMAN *memman = (V_MEMMAN *)VMALLOC_MAN_ADDR;
    vfree(memman, addr);
}

void vfree(V_MEMMAN *memman, void *addr){
    V_MEM_BLOCKINFO *p = memman->entry;
    
    while(p != NULL){
        if((p->addr == (uint32_t)addr) && ((p->flags & VMEM_BLOCKS_ALLOC) != 0)){
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

void init_kernel_mem(void){
    init_pmalloc(PMALLOC_START);
    
    //カーネルイメージのコピー元
    uint8_t *kernel = (uint8_t *)KERNEL_ADDR;
    
    //ページテーブルの初期化
    // page directory table
    init_pdt((PDE *)KERNEL_PDT);
    // page table
    for(int i = 0; i < 1024; i++){
        // 1つのPTはPTE 1024個分
        PTE *pte = ((PTE*)KERNEL_PT0) + (i * 1024);
        init_pt(pte);
    }

    //ページテーブルの設定(4MB)
    //ページング有効化後のカーネルの再配置場所をフレームのアドレスに設定する
    //ページング有効化後、仮想アドレスとしてKERNEL_ADDRを物理アドレスに変換した先にカーネルを再配置する必要がある
    for(int i = 0; i < mem_npage(KERNEL_SIZE); i++){
        uint32_t mem = (uint32_t)pmalloc_4k();
        map_memory_4k((PDE *)KERNEL_PDT, KERNEL_ADDR + MEM_PAGE_SIZE * i, mem);
        
        for(int j = 0; j < MEM_PAGE_SIZE; j++){
            ((uint8_t *)mem)[j] = kernel[MEM_PAGE_SIZE * i + j];
        }
    }
    
    //vram
    for(int i = 0; i < mem_npage(VRAM_SIZE); i++){
        map_memory_4k((PDE *)KERNEL_PDT, VRAM_ADDR_V + MEM_PAGE_SIZE * i, VRAM_ADDR + MEM_PAGE_SIZE * i);
    }
    
    //仮想アドレスでも物理メモリ管理情報にアクセスできるようにする
    //4KB単位で切り上げてマッピング
    for(int i = 0; i < mem_npage(sizeof(P_MEMMAN)); i++){
        map_memory_4k((PDE *)KERNEL_PDT, PMALLOC_MAN_ADDR_V + (i * MEM_PAGE_SIZE), PMALLOC_MAN_ADDR_P + (i * MEM_PAGE_SIZE));
    }
    
    //仮想アドレスでもPDTとPTEにアクセスできるようにマッピング
    map_memory_4k((PDE *)KERNEL_PDT, KERNEL_PDT, KERNEL_PDT);
    for(int i = 0; i < mem_npage(KERNEL_PT_SIZE); i++){
        uint32_t addr = KERNEL_PDT + i * MEM_PAGE_SIZE;
        map_memory_4k((PDE *)KERNEL_PDT, addr, addr);
    }

    //正式なGDTとIDTをページテーブルにマッピング(今は4K超えないので1ページでOK)
    map_memory_4k((PDE *)KERNEL_PDT, GDT_IDT_HEAD_ADDR, GDT_IDT_HEAD_ADDR);
}

void init_gdt(GDT_SEG_DESC *gdt, GDTR *gdtr){
    //null descripter
    gdt[GDT_SEGNUM_NULL].limit_low = 0;
    gdt[GDT_SEGNUM_NULL].limit_high = 0;
    gdt[GDT_SEGNUM_NULL].base_low = 0;
    gdt[GDT_SEGNUM_NULL].base_mid = 0;
    gdt[GDT_SEGNUM_NULL].base_high = 0;
    gdt[GDT_SEGNUM_NULL].type = 0;
    gdt[GDT_SEGNUM_NULL].s = 0;
    gdt[GDT_SEGNUM_NULL].dpl = 0;
    gdt[GDT_SEGNUM_NULL].p = 0;
    gdt[GDT_SEGNUM_NULL].avl = 0;
    gdt[GDT_SEGNUM_NULL].db = 0;
    gdt[GDT_SEGNUM_NULL].g = 0;

    //kernel data segment
    gdt[GDT_SEGNUM_KERNEL_DATA].limit_low = 0xffff;
    gdt[GDT_SEGNUM_KERNEL_DATA].limit_high = 0xf;
    gdt[GDT_SEGNUM_KERNEL_DATA].base_low = 0;
    gdt[GDT_SEGNUM_KERNEL_DATA].base_mid = 0;
    gdt[GDT_SEGNUM_KERNEL_DATA].base_high = 0;
    gdt[GDT_SEGNUM_KERNEL_DATA].type = GDT_TYPE_DATA | GDT_TYPE_DATA_RW; //(0010) ビットフィールド内はMSBを左として書けば良い
    gdt[GDT_SEGNUM_KERNEL_DATA].s = 1;
    gdt[GDT_SEGNUM_KERNEL_DATA].dpl = 0;
    gdt[GDT_SEGNUM_KERNEL_DATA].p = 1;
    gdt[GDT_SEGNUM_KERNEL_DATA].avl = 0;
    gdt[GDT_SEGNUM_KERNEL_DATA].db = 1;
    gdt[GDT_SEGNUM_KERNEL_DATA].g = 1;

    //kernel code segment
    gdt[GDT_SEGNUM_KERNEL_CODE].limit_low = 0xffff;
    gdt[GDT_SEGNUM_KERNEL_CODE].limit_high = 0xf;
    gdt[GDT_SEGNUM_KERNEL_CODE].base_low = 0;
    gdt[GDT_SEGNUM_KERNEL_CODE].base_mid = 0;
    gdt[GDT_SEGNUM_KERNEL_CODE].base_high = 0;
    gdt[GDT_SEGNUM_KERNEL_CODE].type = GDT_TYPE_CODE | GDT_TYPE_CODE_RE;
    gdt[GDT_SEGNUM_KERNEL_CODE].s = 1;
    gdt[GDT_SEGNUM_KERNEL_CODE].dpl = 0;
    gdt[GDT_SEGNUM_KERNEL_CODE].p = 1;
    gdt[GDT_SEGNUM_KERNEL_CODE].avl = 0;
    gdt[GDT_SEGNUM_KERNEL_CODE].db = 1;
    gdt[GDT_SEGNUM_KERNEL_CODE].g = 1;
    
    //user data
    gdt[GDT_SEGNUM_APP_DATA].limit_low = 0xffff;
    gdt[GDT_SEGNUM_APP_DATA].limit_high = 0xf;
    gdt[GDT_SEGNUM_APP_DATA].base_low = 0;
    gdt[GDT_SEGNUM_APP_DATA].base_mid = 0;
    gdt[GDT_SEGNUM_APP_DATA].base_high = 0;
    gdt[GDT_SEGNUM_APP_DATA].type = GDT_TYPE_DATA | GDT_TYPE_DATA_RW;
    gdt[GDT_SEGNUM_APP_DATA].s = 1;
    gdt[GDT_SEGNUM_APP_DATA].dpl = 3;
    gdt[GDT_SEGNUM_APP_DATA].p = 1;
    gdt[GDT_SEGNUM_APP_DATA].avl = 0;
    gdt[GDT_SEGNUM_APP_DATA].db = 1;
    gdt[GDT_SEGNUM_APP_DATA].g = 1;

    //user code segment
    gdt[GDT_SEGNUM_APP_CODE].limit_low = 0xffff;
    gdt[GDT_SEGNUM_APP_CODE].limit_high = 0xf;
    gdt[GDT_SEGNUM_APP_CODE].base_low = 0;
    gdt[GDT_SEGNUM_APP_CODE].base_mid = 0;
    gdt[GDT_SEGNUM_APP_CODE].base_high = 0;
    gdt[GDT_SEGNUM_APP_CODE].type = GDT_TYPE_CODE | GDT_TYPE_CODE_RE | GDT_TYPE_CODE_CONFORM;
    gdt[GDT_SEGNUM_APP_CODE].s = 1;
    gdt[GDT_SEGNUM_APP_CODE].dpl = 3;
    gdt[GDT_SEGNUM_APP_CODE].p = 1;
    gdt[GDT_SEGNUM_APP_CODE].avl = 0;
    gdt[GDT_SEGNUM_APP_CODE].db = 1;
    gdt[GDT_SEGNUM_APP_CODE].g = 1;

    //GDTR
    gdtr->size = GDT_SEG_COUNT * sizeof(GDT_SEG_DESC);
    gdtr->base = (uint32_t)gdt;

    //作ったGDTを読み込む
    lgdt((uint32_t)gdtr);
}

void init_tss(TSS32 *tss0, GDT_SEG_DESC *tss0_desc, GDTR *gdtr){
    tss0_desc->limit_low =  sizeof(TSS32)  & 0x0000ffff;
    tss0_desc->limit_high = (sizeof(TSS32) & 0x000f0000) >> 16;
    tss0_desc->base_low =  (uint32_t)tss0  & 0x0000ffff;
    tss0_desc->base_mid =  ((uint32_t)tss0 & 0x00ff0000) >> 16;
    tss0_desc->base_high = ((uint32_t)tss0 & 0xff000000) >> 24;
    tss0_desc->type = GDT_TYPE_TSS32;
    tss0_desc->s = 0;   // 0=システムセグメント
    tss0_desc->dpl = 3;
    tss0_desc->p = 1;
    tss0_desc->avl = 0;
    tss0_desc->db = 0;  //TSSの場合はビジーフラグ(このTSSが選択されていると立つ？)
    tss0_desc->g = 0;   //0=limitはbyte単位、1=limitは4KB単位

    tss0->esp0 = KERNEL_STACK_V;
    tss0->ss0 = GDT_SEGNUM_KERNEL_DATA << 3;
    
    //GDTRの更新
    gdtr->size = (GDT_SEGNUM_TSS0 + 1) * sizeof(GDT_SEG_DESC);
    lgdt((uint32_t)gdtr);

    //tss読み込み
    //GDTのオフセットで指定
    ltr(GDT_SEGNUM_TSS0 << 3);
}
