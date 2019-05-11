#include "graphic.h"
#include "mem.h"

//初期のカーネル配置
#define KERNEL_ADDR     0x00010000

//ページディレクトリテーブルを配置するアドレス
#define PDT_ADDR        0x00031000

//最初のページテーブルを配置するアドレス
#define PT0_ADDR        0x00032000

//ページング有効化後のカーネル配置（ページング有効化前にここにコピーする）
#define PG_KERNEL_ADDR  0x00030000

typedef unsigned int PTE;
typedef unsigned int PDE;

void set_pte(PTE *pte, unsigned int addr, unsigned int flags){
    *pte = 0;
    *pte |= (addr & PTE_FRAME_ADDR);
    *pte |= flags;
}
void set_pte_flag(PTE *pte, unsigned int flags){
    *pte &= _PTE_FLAGS;
    *pte |= flags;
}

void set_pde(PDE *pde, unsigned int addr, unsigned int flags){
    *pde = 0;
    *pde |= flags;
    *pde |= (addr & PDE_PT_ADDR);
    *pde |= flags;
}
void set_pde_flag(PDE *pde, unsigned int flags){
    *pde &= _PDE_FLAGS;
    *pde |= flags;
}

void enable_paging(unsigned int pdt_ddr);

void Main(void){
    for(;;) io_hlt();
    //コピー元
    unsigned int *kernel = (unsigned int *)KERNEL_ADDR;
    //コピー先
    unsigned int *pg_kernel = (unsigned int *)PG_KERNEL_ADDR;
    
    //KERNEL_ADDRが初期のカーネルの配置場所
    //ページングを有効化すると、仮想アドレスは以下に変換される
    //31-22 : ページディレクトリのindex
    //21-12 : ページテーブルのindex
    //12 : フレームのindex
    //ページング有効化後、仮想アドレスとしてKERNEL_ADDRを物理アドレスに変換した先にカーネルを再配置する必要がある
    //また、使用するページディレクトリと、ページテーブルを設定する
    
    //とりあえず決め打ち
    //unsigned int kernel_pde_index = KARNEL_ADDR & PD_PDE_INDEX;
    //unsigned int kernel_pte_index = KARNEL_ADDR & PT_PTE_INDEX;
    
    //ページディレクトリの設定(とりあえず1ページ)
    //ページテーブルのアドレスを設定する
    //(Pフラグを立てて置かないと、物理メモリに存在しないと解釈されてページフォールトが発生するので立てておく)
    set_pde((unsigned int *)PDT_ADDR, PT0_ADDR, PDE_P);
    
    //ページテーブルの設定(とりあえず1ページ)
    //ページング有効化後のカーネルの再配置場所をフレームのアドレスに設定する
    set_pte((unsigned int *)PT0_ADDR, PG_KERNEL_ADDR, PTE_P);
    
    //カーネルの再配置
    for(unsigned int i = 0; i < 1024 * 4; i++){
        pg_kernel[i] = kernel[i];
    }
    
    //ページングの有効化
    //CR3にページングテーブルのアドレスをストア
    //CR0のPGフラグを立てる
    //enable_paging(PDT_ADDR);
    
    for(;;) io_hlt();
    
    //ページングが有効になったのでここから仮想アドレスを使用していく
    
    init_palette();
    init_screen(0);
    
    //white
    //put_font_asc(0, 0, 7, 'H');
    print_asc(0, 0, 7, "Hello, World! [Uroboros]");
    
    for(;;){
        io_hlt();
    }
}
