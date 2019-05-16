#include "kernel.h"

void Main(void){
    init_pmalloc(PMALLOC_START);
    
    //コピー元
    unsigned char *kernel = (unsigned char *)KERNEL_ADDR;
    
    //ページテーブルの設定(4MB)
    //ページング有効化後のカーネルの再配置場所をフレームのアドレスに設定する
    //ページング有効化後、仮想アドレスとしてKERNEL_ADDRを物理アドレスに変換した先にカーネルを再配置する必要がある
    for(int i = 0; i < 1024; i++){
        unsigned int mem = pmalloc_4k();
        map_memory_4k((PDE *)KERNEL_PDT, KERNEL_ADDR + MEM_PAGE_SIZE * i, mem);
        
        for(int j = 0; j < MEM_PAGE_SIZE; j++){
            ((unsigned char *)mem)[j] = kernel[MEM_PAGE_SIZE * i + j];
        }
    }
    
    //vram
    for(int i = 0; i < 16; i++){
        map_memory_4k((PDE *)KERNEL_PDT, VRAM_ADDR_V + MEM_PAGE_SIZE * i, VRAM_ADDR + MEM_PAGE_SIZE * i);
    }
    
    //stack(とりあえず4K)
    unsigned int new_stack_p = pmalloc_4k();
    map_memory_4k((PDE *)KERNEL_PDT, KERNEL_STACK_V, new_stack_p);
    
    //仮想アドレスでも物理メモリ管理情報にアクセスできるようにする
    //4KB単位で切り上げてマッピング
    int n = ((sizeof(P_MEMMAN) + 0x0fff) & 0xfffff000) / MEM_PAGE_SIZE;
    for(int i = 0; i < n; i++){
        map_memory_4k((PDE *)KERNEL_PDT, PMALLOC_MAN_ADDR_V + (i * MEM_PAGE_SIZE), PMALLOC_MAN_ADDR_P + (i * MEM_PAGE_SIZE));
    }
    
    //仮想アドレスでもPDTとPTEにアクセスできるようにマッピング
    map_memory_4k((PDE *)KERNEL_PDT, KERNEL_PDT, KERNEL_PDT);
    for(unsigned int addr = KERNEL_PT0; addr < (KERNEL_PT0 + (4 * 1024 * 1024)); addr += MEM_PAGE_SIZE){
        map_memory_4k((PDE *)KERNEL_PDT, addr, addr);
    }
    
    //ページングの有効化
    //スタックの再配置を行う（コピーもする）
    //CR3にページングテーブルのアドレスをストア
    //CR0のPGフラグを立てる
    enable_paging(KERNEL_PDT, new_stack_p + MEM_PAGE_SIZE - 4, KERNEL_STACK_V + MEM_PAGE_SIZE - 4);
    
    //ページングが有効になったのでここからは仮想アドレス
    
    init_palette();
    init_screen(4);
    
    print_asc(0, 0, 7, "Welcome to UroborOS!");
    
    if(get_paging_status()){
        print_asc(0, 16, 7, "paging is enable!");
    }
    else{
        print_asc(0, 16, 7, "paging is disable!");
    }
    
    //vmallocの準備
    //仮想メモリの管理情報を入れる領域の確保と初期化
    int m = ((sizeof(V_MEMMAN) + 0x0fff) & 0xfffff000) / MEM_PAGE_SIZE;
    for(int i = 0; i < m; i++){
        unsigned int mem = pmalloc_4k();
        map_memory_4k((PDE *)KERNEL_PDT, VMALLOC_MAN_ADDR + (i * MEM_PAGE_SIZE), mem);
    }
    init_vmalloc(VMALLOC_START);
    
    char s[64];
    for(int i = 0; i < 2; i++){
        int mem = vmalloc_4k();
        _sprintf(s, "[vmalloc_4k] alloc : 0x%08x", mem);
        print_asc(0, i * 16 + 48, 7, s);
    }
    
    unsigned int *data = (unsigned int*)vmalloc_4k();
    *data = 0;
    _sprintf(s, "*0x%08x = 0x%08x", data, *data);
    print_asc(0, 96, 7, s);
    *data = 0x200;
    _sprintf(s, "*0x%08x = 0x%08x", data, *data);
    print_asc(0, 112, 7, s);
    
    for(;;) io_hlt();
}

