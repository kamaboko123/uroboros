#ifndef FDC_H
#define FDC_H

#include "stdint.h"
#include "stdbool.h"
#include "asmlib.h"
#include "console.h"


#define IO_PORT_FDC_DOR 0x3F2
#define FDC_DOR_DRIVE0 0x00
#define FDC_DOR_DRIVE1 0x01
#define FDC_DOR_RESET  0x04
#define FDC_DOR_DMA    0x08 // PS/2では無効にできない(設定不要)
#define FDC_DOR_MOTOR0 0x10
#define FDC_DOR_MOTOR1 0x20

#define IO_PORT_FDC_MSR 0x3F4
#define FDC_MSR_DIO 0x40
#define FDC_MSR_RQM 0x80
#define FDC_MSR_DIO_READ 0x40
#define FDC_MSR_DIO_WRITE 0x00

#define IO_PORT_FDC_CCR 0x3F7
#define FDC_CCR_500K    0x00

#define FDC_COMMAND_SENSE_INT 0x08

void reset_fdc(void);
void fdc_motor_on(uint8_t drive);
bool check_fdc_data_ready(uint8_t direction);
void wait_fdc_command_ready();

#endif
