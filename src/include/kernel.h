#include "mem.h"
#include "graphic.h"

#define PMALLOC_MAN_ADDR_V  0x00902000
#define PMALLOC_MAN_ADDR_P  0x00901000
#define PMALLOC_START       0x00a00000
#define PMALLOC_MAX_PAGE    4096

#define VMALLOC_START       0x03000000
#define VMALLOC_MAX_PAGE    4096

//カーネルの配置アドレス
#define KERNEL_ADDR         0x00100000

//カーネルのスタック(仮想)
#define KERNEL_STACK_V      0x03000000

//カーネルのページディレクトリテーブルを配置するアドレス(物理)
#define KERNEL_PDT          0x00050000

//カーネルのページテーブルを配置するアドレス(物理)
#define KERNEL_PT0          0x00051000

//graphic
#define VRAM_ADDR           0x000A0000
#define VRAM_ADDR_V         0x00000000

#define SCREEN_XSIZE 320
#define SCREEN_YSIZE 200



//CPU
#define CR0_FLAG_PG 0x80000000
