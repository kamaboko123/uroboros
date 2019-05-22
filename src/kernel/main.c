#include "kernel.h"

void Main(void){
    init_pmalloc(PMALLOC_START);
    
    //コピー元
    uint8_t *kernel = (uint8_t *)KERNEL_ADDR;
    
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
    
    
    //ページングが有効になったのでここからは仮想アドレス
    
    //vmalloc初期化
    init_vmalloc(VMALLOC_START, VMALLOC_INIT_END, VMALLOC_MAX_END);
    
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
    sprintf(s, "[palloc_4k] alloc : 0x%08x", pmalloc_4k());
    print_asc(0, 32, 7, s);
    
    for(int i = 1; i < 5; i++){
        uint32_t mem =(uint32_t) vmalloc(i * 0x1000);
        *(uint32_t *)mem = 1000;
        sprintf(s, "[vmalloc] 0x%08x - 0x%08x", mem, i*0x1000 - 1);
        print_asc(0, i * 16 + 48, 7, s);
    }
    
    for(;;) io_hlt();
}

