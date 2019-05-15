#include "kernel.h"

extern PHY_MEMMAN phy_memman;

void Main(void){
    init_phy_memman(0x00300000);
    
    //コピー元
    unsigned char *kernel = (unsigned char *)KERNEL_ADDR;
    
    //KERNEL_ADDRが初期のカーネルの配置場所
    //ページング有効化後、仮想アドレスとしてKERNEL_ADDRを物理アドレスに変換した先にカーネルを再配置する必要がある
    //また、使用するページディレクトリと、ページテーブルを設定する
    
    //ページディレクトリの設定(とりあえず1ページ)
    //ページテーブルのアドレスを設定する
    //(Pフラグを立てて置かないと、物理メモリに存在しないと解釈されてページフォールトが発生するので立てておく)
    set_pde((PDE *)KERNEL_PDT, (PTE *)KERNEL_PT0, PDE_P | PDE_RW);
    
    //ページテーブルの設定
    //ページング有効化後のカーネルの再配置場所をフレームのアドレスに設定する
    for(int i = 0; i < 32; i++){
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
    
    //stack
    unsigned int new_stack_v = 0x00500000;
    unsigned int new_stack_p = pmalloc_4k();
    map_memory_4k((PDE *)KERNEL_PDT, new_stack_v, new_stack_p);
    
    //ページングの有効化
    //CR3にページングテーブルのアドレスをストア
    //CR0のPGフラグを立てる
    enable_paging(KERNEL_PDT, new_stack_p + 4094, new_stack_v + 4094);
    
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

