#include "mem.h"
#include "graphic.h"
#include "stdlibc.h"

//pmalloc
#define PMALLOC_MAN_ADDR_V  0x00901000
#define PMALLOC_MAN_ADDR_P  0x00901000
#define PMALLOC_START       0x01000000

//最大は4GBなので1024 * 1024
//実際にはカーネルコードとかの分があるのでもう少し小さくなる
#define PMALLOC_MAX_PAGE    1024 * 512


//vmalloc
#define VMALLOC_MAN_ADDR    0x00b01000
#define VMALLOC_START       0x03000000

//最大は4GBなので1024 * 1024
#define VMALLOC_MAX_PAGE    1024 * 512


//カーネルの配置アドレス
#define KERNEL_ADDR         0x00100000

//カーネルのスタック(仮想)
#define KERNEL_STACK_V      0x03000000

//カーネルのページディレクトリテーブルを配置するアドレス(物理)
#define KERNEL_PDT          0x00500000

//カーネルのページテーブルを配置するアドレス(物理)
#define KERNEL_PT0          0x00501000

//graphic
#define VRAM_ADDR           0x000A0000
#define VRAM_ADDR_V         0x00000000

#define SCREEN_XSIZE 320
#define SCREEN_YSIZE 200



//CPU
#define CR0_FLAG_PG 0x80000000
