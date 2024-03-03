#include "serial.h"

extern SystemQueue *SYSQ;

void init_serial_port(){
    //115200を基準にしてこの数で割ったものがボーレート
    //DLABに設定する
    uint16_t baud_divisor = 0x0003;

    //割り込みを無効にする
    io_out8(IO_PORT_COM1 + IO_PORT_COM_OFFSET_IE, 0x00);
    
    //DLABの設定
    //上位と下位をそれぞれ設定
    io_out8(IO_PORT_COM1 + IO_PORT_COM_OFFSET_LINE_CTL, COM_LINE_DLAB);
    io_out8(IO_PORT_COM1 + IO_PORT_COM_OFFSET_DLAB_LO, baud_divisor & 0xff);
    io_out8(IO_PORT_COM1 + IO_PORT_COM_OFFSET_DLAB_HI, (baud_divisor & 0xff00) >> 8);

    //ビット数・パリティの設定
    //8bit + 1(stop bit), no parity
    io_out8(IO_PORT_COM1 + IO_PORT_COM_OFFSET_LINE_CTL, COM_LINE_BIT_8 | COM_LINE_STOP_BIT_1 | COM_LINE_PARITY_NONE);
    
    //FIFO無効
    io_out8(IO_PORT_COM1 + IO_PORT_COM_OFFSET_FIFO_CTL, 0x00);

    //割り込みフラグの設定
    io_out8(IO_PORT_COM1 + IO_PORT_COM_OFFSET_IE, COM_IE_DA);
    
    //TODO: 調べる
    io_out8(IO_PORT_COM1 + IO_PORT_COM_OFFSET_MODEM_CTL, 0x00);
    //io_out8(IO_PORT_COM1 + IO_PORT_COM_OFFSET_MODEM_CTL, 0x0b);
    
    //すでに存在する割り込みをクリアしておく
    //io_in8(IO_PORT_COM1 + 2);
    //io_in8(IO_PORT_COM1 + 0);
    //io_in8(IO_PORT_COM1 + 0);
    //io_in8(IO_PORT_COM1 + 0);
    //io_in8(IO_PORT_COM1 + 0);
    //io_in8(IO_PORT_COM1 + 0);
    
    char str[] = "UroborOS...\r\n";
    for(char *s=str; *s != '\0'; s++){
        serial_putc(*s);
    }
}

bool serial_received(){
    return io_in8(IO_PORT_COM1 + IO_PORT_COM_OFFSET_LINE_STATUS) & COM_LINE_STATUS_READ_READY;
}

bool serial_ready_transmit() {
    return io_in8(IO_PORT_COM1 + IO_PORT_COM_OFFSET_LINE_STATUS) & COM_LINE_STATUS_TRANSMIT_EMPTY;
}

uint8_t read_serial(){
    while(!serial_received());
    return io_in8(IO_PORT_COM1);
}

void int_handler_serial(){
    uint8_t data = read_serial();
    q8_in(SYSQ->com1_in, data);

    pic_eoi(PIC_IRQ4);
}

void serial_putc(char c){
    bool iflg = load_int_flag();
    io_cli();
    while(!serial_ready_transmit());
    io_out8(IO_PORT_COM1, c);
    store_int_flag(iflg);
}
