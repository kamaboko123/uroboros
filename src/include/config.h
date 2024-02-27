#ifndef CONFIG_H
#define CONFIG_H

//pmalloc
#define PMALLOC_MAN_ADDR_V  0x00901000
#define PMALLOC_MAN_ADDR_P  0x00901000
#define PMALLOC_START       0x01000000

//最大は4GBなので1024 * 1024
//実際にはカーネルコードとかの分があるのでもう少し小さくなる
#define PMALLOC_MAX_PAGE    1024 * 512


//vmalloc
#define VMALLOC_MAN_ADDR    0x00a01000
#define VMALLOC_START       0x03000000
#define VMALLOC_INIT_END    0x03100000
#define VMALLOC_MAX_END     0x04000000

//#define VMALLOC_ALIGNMENT   0x00000010

//カーネルの配置アドレス
#define KERNEL_ADDR         0x00100000

//カーネルのスタック(仮想)
#define KERNEL_STACK_V      0x03000000
#define KERNEL_STACK_TOP_V  0x01000000

//カーネルのスタック(物理、再配置前)
#define ORG_KERNAL_STACK_P  0x00031000

//カーネルのスタックサイズ(とりあえず4MB)
#define KERNEL_STACK_SIZE   (KERNEL_STACK_V - KERNEL_STACK_TOP_V)

//カーネルのページディレクトリテーブルを配置するアドレス(物理)
#define KERNEL_PDT          0x00500000

//カーネルのページテーブルを配置するアドレス(物理)
#define KERNEL_PT0          0x00501000

//ページテーブルのサイズ
#define KERNEL_PT_SIZE      4 * 1024 * 1024

//カーネルのバイナリサイズ
#define KERNEL_SIZE    4 * 1024 * 1024

//graphic
#define VRAM_ADDR           0x000A0000
#define VRAM_ADDR_V         0x00000000

#define SCREEN_XSIZE 320
#define SCREEN_YSIZE 200

#define VRAM_SIZE SCREEN_XSIZE * SCREEN_YSIZE * 1

//GDT
//init_kernel_mem()で初期化する際にはGDTとIDTまとめて1ページで確保するので注意
#define GDT_IDT_HEAD_ADDR   0x00F00000
#define GDT_ADDR            GDT_IDT_HEAD_ADDR
#define GDT_SIZE            1024                        //実際は5つで、5*8=32byteしか使わない
#define GDTR_ADDR           (GDT_ADDR + GDT_SIZE - 16)  //後方に配置

#define GDT_SEGNUM_NULL 0
#define GDT_SEGNUM_KERNEL_DATA 1
#define GDT_SEGNUM_KERNEL_CODE 2
#define GDT_SEGNUM_APP_DATA 3
#define GDT_SEGNUM_APP_CODE 4
#define GDT_COUNT 5


//intr
#define PIC_INTR_VEC_BASE 0x20

//IDT
#define IDT_ADDR            (GDT_ADDR + GDT_SIZE)
#define IDT_SIZE            256 * 8
#define IDTR_ADDR           (IDT_ADDR + IDT_SIZE - 16)


#define KTASK_STACK_SIZE 2048

// debug macro
// magic break for bochs
#define BREAK() __asm__("xchg %bx, %bx")

#endif
