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
    
    //unsigned int kernel_pde_index = (KERNEL_ADDR & PD_PDE_INDEX) >> 22;
    unsigned int kernel_pte_index = (KERNEL_ADDR & PT_PTE_INDEX) >> 12;
    
    PTE *page_table = (PTE *)PT0_ADDR;
    
    //ページディレクトリの設定(とりあえず1ページ)
    //ページテーブルのアドレスを設定する
    //(Pフラグを立てて置かないと、物理メモリに存在しないと解釈されてページフォールトが発生するので立てておく)
    set_pde((PDE *)PDT_ADDR, PT0_ADDR, PDE_P | PDE_RW);
    
    //ページテーブルの設定
    //ページング有効化後のカーネルの再配置場所をフレームのアドレスに設定する
    for(int i = 0; i < 32; i++){
        unsigned int alloc_addr = alloc_phy_mem_4k();
        set_pte(((PTE *)PT0_ADDR) + kernel_pte_index + i, alloc_addr, PTE_P | PTE_RW);
        for(int j = 0; j < 4096; j++){
            ((unsigned char *)alloc_addr)[j] = kernel[4096 * i + j];
        }
    }
    
    unsigned int vram_pte_index = (VRAM_ADDR & PT_PTE_INDEX) >> 12;
    for(int i = 0; i < 160; i++){
        set_pte(((PTE *)PT0_ADDR) + vram_pte_index + i, VRAM_ADDR + (0x1000 * i), PTE_P | PTE_RW);
    }
    
    //ページングの有効化
    //CR3にページングテーブルのアドレスをストア
    //CR0のPGフラグを立てる
    enable_paging(PDT_ADDR);
    
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

