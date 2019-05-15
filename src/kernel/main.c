#include "kernel.h"

extern PHY_MEMMAN phy_memman;

void Main(void){
    init_phy_memman(PMALLOC_START);
    
    //コピー元
    unsigned char *kernel = (unsigned char *)KERNEL_ADDR;
    
    //ページテーブルの設定(4MB)
    //ページング有効化後のカーネルの再配置場所をフレームのアドレスに設定する
    //ページング有効化後、仮想アドレスとしてKERNEL_ADDRを物理アドレスに変換した先にカーネルを再配置する必要がある
    for(int i = 0; i < 1024; i++){
        unsigned int alloc_addr = pmalloc_4k();
        map_memory_4k((PDE *)KERNEL_PDT, KERNEL_ADDR + MEM_PAGE_SIZE * i, alloc_addr);
        
        for(int j = 0; j < MEM_PAGE_SIZE; j++){
            ((unsigned char *)alloc_addr)[j] = kernel[MEM_PAGE_SIZE * i + j];
        }
    }
    
    //vram
    for(int i = 0; i < 160; i++){
        map_memory_4k((PDE *)KERNEL_PDT, VRAM_ADDR_V + MEM_PAGE_SIZE * i, VRAM_ADDR + MEM_PAGE_SIZE * i);
    }
    
    //stack(とりあえず4K)
    unsigned int new_stack_p = pmalloc_4k();
    map_memory_4k((PDE *)KERNEL_PDT, KERNEL_STACK_V, new_stack_p);
    
    //4KB単位で切り上げて、マッピング
    int n = ((sizeof(PHY_MEMMAN) + 0x0fff) & 0xfffff000) / MEM_PAGE_SIZE;
    for(int i = 0; i < n; i++){
        map_memory_4k((PDE *)KERNEL_PDT, PMALLOC_MAN_ADDR_V + (i * MEM_PAGE_SIZE), PMALLOC_MAN_ADDR_P + (i * MEM_PAGE_SIZE));
    }
    
    //ページングの有効化
    //CR3にページングテーブルのアドレスをストア
    //CR0のPGフラグを立てる
    enable_paging(KERNEL_PDT, new_stack_p + MEM_PAGE_SIZE - 4, KERNEL_STACK_V + MEM_PAGE_SIZE - 4);
    
    //pmalloc_4k();
    
    //for(;;) io_hlt();
    
    //ページングが有効になったのでここから仮想アドレスを使用していく
    init_palette();
    init_screen(0);
    
    //white
    //put_font_asc(0, 0, 7, 'H');
    print_asc(0, 0, 7, "Welcome to UroborOS!");
    
    if(get_paging_status()){
        print_asc(0, 16, 7, "paging is enable!");
    }
    else{
        print_asc(0, 16, 7, "paging is disable!");
    }
    
    for(;;) io_hlt();
}

