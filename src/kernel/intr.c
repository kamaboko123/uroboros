#include "intr.h"

void init_pic(uint16_t imr, uint32_t intr_vec_base){
    // imr: PICに設定するマスク(master=>上位8bit, slave=>下位8bit)
    // intr_vec_base: 割り込みベクタ番号のベース

    // ICW1(初期化開始、ICW4あり)
    io_out8(IO_PORT_PIC1_COMMAND, PIC_ICW1_BASE | PIC_ICW1_ICW4);
    io_out8(IO_PORT_PIC2_COMMAND, PIC_ICW1_BASE | PIC_ICW1_ICW4);
    
    io_wait();

    // ICW2(割り込み番号のベースを設定)
    io_out8(IO_PORT_PIC1_DATA, intr_vec_base);
    io_out8(IO_PORT_PIC2_DATA, intr_vec_base + 8);
    
    io_wait();
    
    // ICW3(MasterのどのIRにSlaveがつながっているか設定)
    io_out8(IO_PORT_PIC1_DATA, PIC_ICW3_MASTER);
    io_out8(IO_PORT_PIC2_DATA, PIC_ICW3_SLAVE);
    
    io_wait();

    // ICW4(8086モードに設定)
    io_out8(IO_PORT_PIC1_DATA, PIC_ICW4_8086);
    io_out8(IO_PORT_PIC2_DATA, PIC_ICW4_8086);
    
    io_wait();
    
    //IMR設定(Master側は上位8bit)
    io_out8(IO_PORT_PIC1_DATA, imr & 0x00FF);
    //IMR設定(Master側は下位8bit)
    io_out8(IO_PORT_PIC2_DATA, (imr & 0xFF00) >> 8);
    
    io_wait();
}

void init_idt(){
    void *handler = (void *)0;

    IDT idt;
    idt.base_lo = (uint16_t)((uint32_t)handler & 0xffff);
    idt.base_hi = (uint16_t)((uint32_t)handler >> 16);
    idt.sel = 0x10; //kernel code segment
    idt.reserved = 0;
    //idt.flags = 0;
}
void set_idt(uint8_t vec_num, void (*func)(void)){}
