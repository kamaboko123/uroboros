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
        map_memory_4k((PDE *)KERNEL_PDT, KERNEL_ADDR + 4096 * i, alloc_addr);
        
        for(int j = 0; j < 4096; j++){
            ((unsigned char *)alloc_addr)[j] = kernel[4096 * i + j];
        }
    }
    
    //vram
    for(int i = 0; i < 160; i++){
        map_memory_4k((PDE *)KERNEL_PDT, VRAM_ADDR_V + 4096 * i, VRAM_ADDR + 4096 * i);
    }
    
    //stack(とりあえず4K)
    unsigned int new_stack_p = pmalloc_4k();
    map_memory_4k((PDE *)KERNEL_PDT, KERNEL_STACK_V, new_stack_p);
    
    //pmalloc用
    /*
    for(int i = 0; i <= (sizeof(MAX_PHY_MEM_PAGE)/4096); i++){
        map_memory_4k((PDE *)KERNEL_PDT, PHY_MEM_MAN_ADDR_V + (i * 4096), PHY_MEM_MAN_ADDR_P + (i * 4096));
    }
    */
    
    //4KB単位で切り上げて、マッピング
    int n = ((sizeof(PHY_MEMMAN) + 0x1000) & ~0x1000) / 4096;
    for(int i = 0; i < n; i++){
        map_memory_4k((PDE *)KERNEL_PDT, PHY_MEM_MAN_ADDR_V + (i * 4096), PHY_MEM_MAN_ADDR_P + (i * 4096));
    }
    
    map_memory_4k((PDE *)KERNEL_PDT, PHY_MEM_MAN_ADDR_V, PHY_MEM_MAN_ADDR_P);
    map_memory_4k((PDE *)KERNEL_PDT, PHY_MEM_MAN_ADDR_V + 4096, PHY_MEM_MAN_ADDR_P + 4096);
    
    //ページングの有効化
    //CR3にページングテーブルのアドレスをストア
    //CR0のPGフラグを立てる
    enable_paging(KERNEL_PDT, new_stack_p + 4094, KERNEL_STACK_V + 4094);
    
    pmalloc_4k();
    
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

