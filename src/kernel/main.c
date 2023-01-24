#include "kernel.h"

void Main(uint64_t *gdt0){
    //ページテーブルの初期化と、ページング移行、vmallocの初期化
    init_kernel_mem();
    //stack
    uint32_t new_stack_p;
    for(int i = 0; i < mem_npage(KERNEL_STACK_SIZE); i++){
        uint32_t mem = (uint32_t)pmalloc_4k();
        if(i == 0){
            new_stack_p = mem;
        }
        map_memory_4k((PDE *)KERNEL_PDT, KERNEL_STACK_TOP_V + i * MEM_PAGE_SIZE, mem);
    }
    
    //ページングの有効化
    //スタックの再配置を行う（コピーもする）
    //CR3にページングテーブルのアドレスをストア
    //CR0のPGフラグを立てる
    enable_paging(KERNEL_PDT, new_stack_p + KERNEL_STACK_SIZE - 4, KERNEL_STACK_TOP_V + KERNEL_STACK_SIZE - 4);
    
    //vmalloc初期化
    init_vmalloc(VMALLOC_START, VMALLOC_INIT_END, VMALLOC_MAX_END);
    
    //割り込み
    io_cli();
    init_pic(0xFFFF, PIC_INTR_VEC_BASE);
    init_pit();
    io_sti();

    //グラフィック初期化
    init_palette();
    init_screen(4);
    print_asc(0, 0, 7, "Welcome to UroborOS!");
    
    if(get_paging_status()){
        print_asc(0, 16, 7, "paging is enable!");
    }
    else{
        print_asc(0, 16, 7, "paging is disable!");
    }
    
    char s[64];
    sprintf(s, "gdt: 0x%08x", (uint32_t)gdt0);
    print_asc(0, 32, 7, s);
    
    sprintf(s, "[palloc_4k] alloc");
    
    char *p = pmalloc_4k();
    sprintf(s, "[palloc_4k] alloc : 0x%08x", p);
    print_asc(0, 48, 7, s);
    for(int i = 1; i < 5; i++){
        uint32_t mem =(uint32_t) vmalloc(i * 0x1000+3);
        *(uint32_t *)mem = 1000;
        sprintf(s, "[vmalloc] 0x%08x - 0x%08x", mem, i*0x1000 - 1);
        print_asc(0, i * 16 + 64, 7, s);
    }
    
    for(;;) io_hlt();
}

