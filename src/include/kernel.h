#include "mem.h"
#include "graphic.h"

#define PHY_MEM_MAN_ADDR_V 0x00000000
#define PHY_MEM_MAN_ADDR_P 0x00A00000
#define PHY_ADDR_START 0x00500000
#define MAX_PHY_MEM_PAGE 1024

//カーネルの配置アドレス
#define KERNEL_ADDR 0x00100000

//カーネルのページディレクトリテーブルを配置するアドレス(物理)
#define KERNEL_PDT 0x00050000

//カーネルのページテーブルを配置するアドレス(物理)
#define KERNEL_PT0 0x00051000

//graphic
#define VRAM_ADDR 0x000A0000
#define VRAM_ADDR_V 0x00000000

#define SCREEN_XSIZE 320
#define SCREEN_YSIZE 200

