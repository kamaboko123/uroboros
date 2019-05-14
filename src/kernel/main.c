#include "mem.h"
#include "graphic.h"

//初期のカーネル配置
#define KERNEL_ADDR     0x00100000

//ページディレクトリテーブルを配置するアドレス
#define PDT_ADDR        0x00031000

//最初のページテーブルを配置するアドレス
#define PT0_ADDR        0x00032000

//ページング有効化後のカーネル配置（ページング有効化前にここにコピーする）
//#define PG_KERNEL_ADDR  0x00300000

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
    set_pde((PDE *)PDT_ADDR, (PTE *)PT0_ADDR, PDE_P | PDE_RW);
    
    //ページテーブルの設定
    //ページング有効化後のカーネルの再配置場所をフレームのアドレスに設定する
    for(int i = 0; i < 32; i++){
        unsigned int alloc_addr = alloc_phy_mem_4k();
        map_memory_4k((PDE *)PDT_ADDR, KERNEL_ADDR + 4096 * i, alloc_addr);
        
        for(int j = 0; j < 4096; j++){
            ((unsigned char *)alloc_addr)[j] = kernel[4096 * i + j];
        }
    }
    
    //vram
    for(int i = 0; i < 160; i++){
        map_memory_4k((PDE *)PDT_ADDR, VRAM_ADDR_VIRTUAL + 4096 * i, VRAM_ADDR + 4096 * i);
    }
    
    //stack
    unsigned int new_stack_v = 0x00500000;
    unsigned int new_stack_p = alloc_phy_mem_4k();
    map_memory_4k((PDE *)PDT_ADDR, new_stack_v, new_stack_p);
    
    //ページングの有効化
    //CR3にページングテーブルのアドレスをストア
    //CR0のPGフラグを立てる
    enable_paging(PDT_ADDR, new_stack_p + 4094, new_stack_v + 4094);
    
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

