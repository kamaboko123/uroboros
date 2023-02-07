#include "intr.h"

void init_pic(uint16_t imr, uint32_t intr_vec_base){
    // imr: PICに設定するマスク(master=>下位8bit, slave=>上位8bit)
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
    
    //IMR設定(Master側は下位8bit)
    io_out8(IO_PORT_PIC1_DATA, imr & 0x00FF);
    //IMR設定(Slave側は上位8bit)
    io_out8(IO_PORT_PIC2_DATA, (imr & 0xFF00) >> 8);
    
    io_wait();
}

void init_idt(IDT *idt0, IDTR *idtr){
    void *handler = (void *)int_handler_null;
    for(int i = 0; i < 256; i++){
        set_idt(idt0, i, handler);
    }
    
    idtr->size = 256 * sizeof(IDT);
    idtr->base = (uint32_t)idt0;

    //作ったGDTを読み込む
    lidt((uint32_t)idtr);
}

void set_idt(IDT *idt0, uint8_t vec_num, void (*handler)(void)){
    IDT *idt = idt0 + vec_num;
    idt->base_lo = (uint16_t)((uint32_t)handler & 0xffff);
    idt->base_hi = (uint16_t)((uint32_t)handler >> 16);
    idt->sel = GDT_SEGNUM_KERNEL_CODE * 8; //kernel code segment
    idt->reserved = 0;
    idt->flags = IDT_FLAG_INT_GATE | IDT_FLAG_D_32 | IDT_FLAG_DPL_0 | IDT_FLAG_P;
}


