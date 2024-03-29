#include "intr.h"

extern SystemQueue *SYSQ;

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

    set_idt(idt0, 0x00, int00_handler);
    set_idt(idt0, 0x01, int01_handler);
    set_idt(idt0, 0x02, int02_handler);
    set_idt(idt0, 0x03, int03_handler);
    set_idt(idt0, 0x04, int04_handler);
    set_idt(idt0, 0x05, int05_handler);
    set_idt(idt0, 0x06, int06_handler);
    set_idt(idt0, 0x07, int07_handler);
    set_idt(idt0, 0x08, int08_handler);
    set_idt(idt0, 0x09, int09_handler);
    set_idt(idt0, 0x0a, int0a_handler);
    set_idt(idt0, 0x0b, int0b_handler);
    set_idt(idt0, 0x0c, int0c_handler);
    set_idt(idt0, 0x0d, int0d_handler);
    set_idt(idt0, 0x0e, int0e_handler);
    set_idt(idt0, 0x0f, int0f_handler);
    set_idt(idt0, 0x10, int10_handler);
    set_idt(idt0, 0x11, int11_handler);
    set_idt(idt0, 0x12, int12_handler);
    set_idt(idt0, 0x13, int13_handler);
    set_idt(idt0, 0x14, int14_handler);
    set_idt(idt0, 0x15, int15_handler);
    set_idt(idt0, 0x16, int16_handler);
    set_idt(idt0, 0x17, int17_handler);
    set_idt(idt0, 0x18, int18_handler);
    set_idt(idt0, 0x19, int19_handler);
    set_idt(idt0, 0x1a, int1a_handler);
    set_idt(idt0, 0x1b, int1b_handler);
    set_idt(idt0, 0x1c, int1c_handler);
    set_idt(idt0, 0x1d, int1d_handler);
    set_idt(idt0, 0x1e, int1e_handler);
    set_idt(idt0, 0x1f, int1f_handler);
    
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

void pic_eoi(uint8_t irq){
    if(irq >= 8){
        // slave側、master側それぞれにEOIを送る(slaveはIRQ2に繋がっている)
        // どちらにも同じIRQ番号を送っているケースも見られたので、CPUかチップセットでうまいことやってくれるのかもしれない
        io_out8(IO_PORT_PIC2_COMMAND, PIC_OCW2_CMD_EOI_BASE % 7);
        io_out8(IO_PORT_PIC1_COMMAND, PIC_OCW2_CMD_EOI_BASE + PIC_IMR_IRQ2);
    }
    else{
        io_out8(IO_PORT_PIC1_COMMAND, PIC_OCW2_CMD_EOI_BASE | irq);
    }
}

void int_handler(IntrFrame iframe){
    if(iframe.intrnum == PIC_INTR_VEC_BASE + PIC_IRQ0){
        // timer
        tick_timer();
        //先にEOIを送っておく
        //スケジューラを呼ぶとコンテキストスイッチが起こってしまうため
        pic_eoi(PIC_IRQ0);
        
        //タスクスイッチ用のタイマが発火したらスケジューラを呼び出してタスクを切り変える
        if(!q8_empty(SYSQ->task_timer)){
            q8_de(SYSQ->task_timer);
            sched_handler();
        }
    }
    else if(iframe.intrnum == PIC_INTR_VEC_BASE + PIC_IRQ4){
        // COM1
        int_handler_serial();
        pic_eoi(PIC_IRQ4);
    }
    else if(iframe.intrnum <= 0x1f){
        char str[64];

        switch(iframe.intrnum){
            case 0x00:
                strcpy(str, "Division Error");
                break;
            case 0x01:
                strcpy(str, "Debug");
                break;
            case 0x02:
                strcpy(str, "Non-maskable Interrupt");
                break;
            case 0x03:
                strcpy(str, "Breakpoint");
                break;
            case 0x04:
                strcpy(str, "Overflow");
                break;
            case 0x05:
                strcpy(str, "Bound Range Exceeded");
                break;
            case 0x06:
                strcpy(str, "Invalid Opcode");
                break;
            case 0x07:
                strcpy(str, "Device Not Available");
                break;
            case 0x08:
                strcpy(str, "Double Fault");
                break;
            case 0x09:
                strcpy(str, "Coprocessor Segment Overrun");
                break;
            case 0x0a:
                strcpy(str, "Invalid TSS");
                break;
            case 0x0b:
                strcpy(str, "Segment Not Present");
                break;
            case 0x0c:
                strcpy(str, "Stack-Segment Fault");
                break;
            case 0x0d:
                strcpy(str, "General Protection Fault");
                break;
            case 0x0e:
                strcpy(str, "Page Fault");
                break;
            case 0x10:
                strcpy(str, "x87 Floating-Point Exception");
                break;
            case 0x11:
                strcpy(str, "Alignment Check");
                break;
            case 0x12:
                strcpy(str, "Machine Check");
                break;
            case 0x13:
                strcpy(str, "SIMD Floating-Point Exception");
                break;
            case 0x14:
                strcpy(str, "Virtualization Exception");
                break;
            case 0x15:
                strcpy(str, "Control Protection Exception");
                break;
            case 0x1c:
                strcpy(str, "Hypervisor Injection Exception");
                break;
            case 0x1d:
                strcpy(str, "VMM Communication Exception");
                break;
            case 0x1e:
                strcpy(str, "Security Exception");
                break;
            default:
                strcpy(str, "");
        }
        serial_putstr(str);
        sprintf(str, "\n!! EXCEPTION INT: 0x%02x (eip: 0x%08x) !!\n", iframe.intrnum, iframe.eip);
        serial_putstr(str);
        for(;;) io_hlt();
    }
    else {
        // それ以外の割り込みは無視
        char str[64];
        sprintf(str, "!! UNKNOWN INTERRUPT: 0x%02x !!\n", iframe.intrnum);
        int_handler_null();
    }
}
