#ifndef SERIAL_H
#define SERIAL_H

#include "stdint.h"
#include "asmlib.h"
#include "queue.h"
#include "intr.h"
#include "global.h"


#define IO_PORT_COM1 0x3f8

#define IO_PORT_COM_OFFSET_IE 1
#define IO_PORT_COM_OFFSET_FIFO_CTL 2
#define IO_PORT_COM_OFFSET_LINE_CTL 3
#define IO_PORT_COM_OFFSET_MODEM_CTL 4
#define IO_PORT_COM_OFFSET_LINE_STATUS 5

//DLABが1に設定されている場合、以下のoffsetアドレスでDLABが設定できる
#define IO_PORT_COM_OFFSET_DLAB_LO 0
#define IO_PORT_COM_OFFSET_DLAB_HI 1

//DLABはIO_PORT_COM_OFFSET_LINE_CTLの最上位bit
#define COM_LINE_DLAB 0x80

#define COM_LINE_BIT_8       3 << 0
#define COM_LINE_STOP_BIT_1  0 << 2
#define COM_LINE_PARITY_NONE 0 << 3

#define COM_LINE_STATUS_READ_READY 1 << 0
#define COM_LINE_STATUS_TRANSMIT_EMPTY 1 << 5

//どの割り込みを有効にするか
#define COM_IE_DA            1 << 0
#define COM_IE_EMPTY         1 << 1
#define COM_IE_BREAK_ERROR   1 << 2
#define COM_IE_STATUS_CHANGE 1 << 3

void init_serial_port();
void int_handler_serial();

bool serial_received();
bool serial_ready_transmit();
uint8_t read_serial();
void serial_putc(char c);

#endif
