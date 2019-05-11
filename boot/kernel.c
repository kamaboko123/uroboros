#include "graphic.h"

#define PTE_P           0x00000001
#define PTE_RW          0x00000002
#define PTE_US          0x00000004
#define PTE_PWT         0x00000008
#define PTE_PCD         0x00000010
#define PTE_A           0x00000020
#define PTE_D           0x00000040
#define PTE_PAT         0x00000080
#define PTE_G           0x00000100
#define PTE_IGNORE      0x00000E00
#define PTE_FRAME_ADDR  0xFFFFF000
#define _PTE_FLAGS      0x000001FF

#define PDE_P           0x00000001
#define PDE_RW          0x00000002
#define PDE_US          0x00000004
#define PDE_PWT         0x00000008
#define PDE_PCD         0x00000010
#define PDE_A           0x00000020
#define PDE_PS          0x00000080
#define PDE_G           0x00000100
#define PDE_IGNORE      0x00000E00
#define PDE_PT_ADDR     0xFFFFF000
#define _PDE_FLAGS      0x000001BF

#define PD_PDE_INDEX    0xFFC00000
#define PT_PTE_INDEX    0x003FF000

//初期のカーネル配置
#define KERNEL_ADDR     0x00100000

//ページディレクトリテーブルを配置するアドレス
#define PDT_ADDR        0x00031000

//最初のページテーブルを配置するアドレス
#define PT0_ADDR        0x00032000

//ページング有効化後のカーネル配置（ページング有効化前にここにコピーする）
#define PG_KERNEL_ADDR  0x00300000

typedef unsigned int PTE;
typedef unsigned int PDE;
void set_pte(PTE *pte, unsigned int addr, unsigned int flags);
void set_pte_flag(PTE *pte, unsigned int flags);
void set_pde(PDE *pde, unsigned int addr, unsigned int flags);
void set_pde_flag(PDE *pde, unsigned int flags);
void enable_paging(unsigned int pdt_ddr);
int load_cr0(void);

void Main(void){
    //コピー元
    unsigned char *kernel = (unsigned char *)KERNEL_ADDR;
    //コピー先
    unsigned char *pg_kernel = (unsigned char *)PG_KERNEL_ADDR;
    
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
    
    /*
    //ページテーブルの設定
    //ページング有効化後のカーネルの再配置場所をフレームのアドレスに設定する
    for(int i = 0; i < 1024; i++){
        //set_pte(((PTE *)PT0_ADDR) + i, PG_KERNEL_ADDR + (0x1000 * i), PTE_P | PTE_RW);
        set_pte(&page_table[i], PG_KERNEL_ADDR + (0x1000 * i), PTE_P | PTE_RW);
    }
    */
    set_pte(((PTE *)PT0_ADDR) + kernel_pte_index, PG_KERNEL_ADDR, PTE_P | PTE_RW);
    set_pte(((PTE *)PT0_ADDR) + kernel_pte_index + 1, PG_KERNEL_ADDR + 0x1000, PTE_P | PTE_RW);
    //カーネルの再配置
    for(int i = 0; i < (1024 * 4 * 1024); i++){
        pg_kernel[i] = kernel[i];
    }
    
    //ページングの有効化
    //CR3にページングテーブルのアドレスをストア
    //CR0のPGフラグを立てる
    enable_paging(PDT_ADDR);
    
    for(;;) io_hlt();
    
    //ページングが有効になったのでここから仮想アドレスを使用していく
    init_palette();
    init_screen(0);
    
    //white
    //put_font_asc(0, 0, 7, 'H');
    //print_asc(0, 0, 7, "Hello, World! [Uroboros]");
    
    if((load_cr0() & 0x80000000) != 0){
        print_asc(0, 0, 7, "paging is enable!");
    }
    else{
        print_asc(0, 0, 7, "paging is disable!");
    }
}

void set_pte(PTE *pte, unsigned int addr, unsigned int flags){
    *pte = 0;
    *pte |= (addr & PTE_FRAME_ADDR);
    *pte |= flags;
}

void set_pde(PDE *pde, unsigned int addr, unsigned int flags){
    *pde = 0;
    *pde |= (addr & PDE_PT_ADDR);
    *pde |= flags;
}

