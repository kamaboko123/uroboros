#ifndef FDC_H
#define FDC_H

#include "stdint.h"
#include "stdbool.h"
#include "asmlib.h"
#include "console.h"
#include "timer.h"


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

#define IO_PORT_FDC_DATA 0x3F5

#define FDC_CMD_SENSE_INTERRUPT_STATUS 0x08
#define FDC_CMD_SPECIFY 0x03
#define FDC_CMD_RECALIBRATE 0x07
#define FDC_CMD_SEEK 0x0F
#define FDC_CMD_READ_DATA 0x06

#define FDC_CMD_SPECIFY_PARAM_DMA_ENABLE 0x00
#define FDC_CMD_SPECIFY_PARAM_STEP_RATE 0x0f
#define FDC_CMD_SPECIFY_PARAM_HEAD_UNLOAD_TIME 0x0f
#define FDC_CMD_SPECIFY_PARAM_HEAD_LOAD_TIME 0x0f

// FDC制御の結果
#define FDC_OK 0x00
#define FDC_ERROR_NOT_READY 0x01
#define FDC_ERROR_RECALIBRATE 0x01

typedef uint8_t FDC_RESULT;


typedef struct FDC_ST0{
    uint8_t ds0:1;
    uint8_t ds1:1;
    uint8_t h:1;
    uint8_t resv0:1;
    uint8_t ec:1;
    uint8_t se:1;
    uint8_t ic0:1;
    uint8_t ic1:1;
} FDC_ST0;

typedef uint8_t FDC_CMD_STATUS[7];

typedef struct FDC_CMD_STATUS_SENSE_INTERRUPT_STATUS{
    union{
        uint8_t st0_data;
        FDC_ST0 st0;
    };
    uint8_t pcn; //present cylinder number
} FDC_CMD_STATUS_SENSE_INTERRUPT_STATUS;

// High level functions
FDC_RESULT init_fdc(void);
void fdc_cmd_read_data();

// FDC controll and general functions
void fdc_motor_on(uint8_t drive);
void fdc_motor_off(uint8_t drive);
bool check_fdc_data_ready(uint8_t direction);
void fdc_read_status(FDC_CMD_STATUS *buf);

//FDC commands
FDC_RESULT fdc_cmd_recalibrate(uint8_t drive);
FDC_CMD_STATUS_SENSE_INTERRUPT_STATUS *fdc_cmd_sense_interrupt_status(FDC_CMD_STATUS *buf);
FDC_RESULT fdc_cmd_specify(uint8_t step_rate, uint8_t head_unload_time, uint8_t head_load_time, uint8_t dma);

void init_fdc_dma(void);
#endif
