#include <stdio.h>
#include <stdint.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#define VMEM_MAX_UNITS 256
#define VMEM_BLOCKS_USE 0x01
#define VMEM_BLOCKS_ALLOC 0x02

typedef struct V_MEM_BLOCKINFO{
    uint8_t flags;
    uint32_t addr;
    uint32_t size;
    struct V_MEM_BLOCKINFO *next;
    struct V_MEM_BLOCKINFO *prev;
}V_MEM_BLOCKINFO;

typedef struct V_MEMMAN{
    uint32_t start_addr;
    V_MEM_BLOCKINFO blocks[VMEM_MAX_UNITS];
    V_MEM_BLOCKINFO *entry;
}V_MEMMAN;


V_MEMMAN _memman;
V_MEMMAN *memman = &_memman;

void init_mem_block(V_MEM_BLOCKINFO *block){
    block->size = 0;
    block->flags = 0;
    block->next = NULL;
    block->prev = NULL;
}


void init_vmalloc(uint32_t start_addr, uint32_t end_addr){
    memman->start_addr = start_addr;
    for(int i = 0; i < VMEM_MAX_UNITS; i++){
        init_mem_block(&memman->blocks[i]);
    }
    memman->entry =  &memman->blocks[0];
    memman->blocks[0].flags |= VMEM_BLOCKS_USE;
    memman->blocks[0].addr = start_addr;
    memman->blocks[0].size = end_addr - start_addr;
}

V_MEM_BLOCKINFO *get_new_block(void){
    for(int i = 0; i < VMEM_MAX_UNITS; i++){
        if((memman->blocks[i].flags & VMEM_BLOCKS_USE) == 0){
            memman->blocks[i].flags |=VMEM_BLOCKS_USE;
            return &memman->blocks[i];
        }
    }
    return NULL;
}

void *vmalloc(uint32_t size){
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
    
    /*
    //pが割当可能
    while((p->flags & VMEM_BLOCKS_ALLOC) != 0 || p->next != NULL){
        p = p->next;
    }
    if(p->size < size){
        return 0;
    }
    
    if(p->next == NULL){
        //ブロックを挿入してリスト繋ぎ変え
        V_MEM_BLOCKINFO *new_block =  get_new_block();
        init_mem_block(new_block);
        
        if(p->next != NULL){
            p->next->prev = new_block;
        }
        new_block->next = p->next;
        p->next = new_block;
        new_block->prev = p;
        
        p->flags |= VMEM_BLOCKS_ALLOC;
        
        new_block->flags |= VMEM_BLOCKS_USE;
        new_block->size = p->size - size;
        p->size = size;
        new_block->addr = p->addr + p->size;
        
        return (void *)p->addr;
    }*/
}

void _dump_vmem(void){
    V_MEM_BLOCKINFO *p = memman->entry;
    printf("[ENTRY]\n");
    for(int i = 0; p != NULL; i++){
        printf("block %02d : [base : 0x%04x] [size : 0x%04x] [flag : ", i, p->addr, p->size);
        if((p->flags & VMEM_BLOCKS_USE) != 0){
            printf("U");
        }
        else{
            printf(" ");
        }
        if((p->flags & VMEM_BLOCKS_ALLOC) != 0){
            printf("A");
        }
        else{
            printf(" ");
        }
        printf("]\n");
        
        p = p->next;
    }
    printf("[END]\n");
}

void vfree(void *addr){
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
        init_mem_block(next_block);
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
        
        init_mem_block(p);
    }
}

void test_vmalloc(void);

int main(void){
    CU_pSuite testSuite;
    
    CU_initialize_registry();
    testSuite = CU_add_suite("VmallocTest", NULL, NULL);
    
    CU_add_test(testSuite, "test of vmalloc", test_vmalloc);
    
    CU_basic_run_tests();
    CU_cleanup_registry();
    
    return 0;
}


#define START_ADDR 0x10000
#define END_ADDR 0x15000
/*
void test_block_list(void){
    V_MEM_BLOCKINFO *p = memman->entry;
    while(p != NULL){
        if(p->prev != NULL){
            CU_ASSERT(p->prev->addr + p->prev->size == p->addr);
        }
        p = p->next;
    }
}
*/
void test_vmalloc(void){
    init_vmalloc(START_ADDR, END_ADDR);
    
    uint32_t m1 = (uint32_t)vmalloc(0x1000);
    CU_ASSERT(m1 == START_ADDR + 0x0000);
    uint32_t m2 = (uint32_t)vmalloc(0x1000);
    CU_ASSERT(m2 == START_ADDR + 0x1000);
    uint32_t m3 = (uint32_t)vmalloc(0x1000);
    CU_ASSERT(m3 == START_ADDR + 0x2000);
    uint32_t m4 = (uint32_t)vmalloc(0x1000);
    CU_ASSERT(m4 == START_ADDR + 0x3000);
    uint32_t m5 = (uint32_t)vmalloc(0x1000);
    CU_ASSERT(m5 == START_ADDR + 0x4000);
    
    uint32_t m6 = (uint32_t)vmalloc(0x1000);
    CU_ASSERT(m6 == NULL);
    
    
}
