#ifndef FDC_H
#define FDC_H

#include "stdint.h"
#include "stdbool.h"
#include "asmlib.h"
#include "console.h"
#include "timer.h"
#include "dma.h"

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
#define FDC_CMD_CONFIGURE 0x13

#define FDC_CMD_SPECIFY_PARAM_DMA_ENABLE 0x00
#define FDC_CMD_SPECIFY_PARAM_STEP_RATE 0x0f
#define FDC_CMD_SPECIFY_PARAM_HEAD_UNLOAD_TIME 0x0f
#define FDC_CMD_SPECIFY_PARAM_HEAD_LOAD_TIME 0x0f

//設定値をNとして `sector_size = 2 ^ N * 128` となるNを指定する
// 512 = 2 ^ N * 128
// 512 / 128 = 2 ^ N
// 4 = 2 ^ N
// N = 2
#define FDC_CMD_READ_DATA_SECTOR_SIZE_512K 0x02
#define FDC_CMD_READ_DATA_GAP3 27
#define FDC_CMD_READ_DATA_DATA_LENGTH 0xFF  // コマンド内のセクタサイズが0以外の場合は0xFFを指定するらしい

#define FDC_CMD_CONFIGURE_FIFO_THRESHOLD_DEFAULT  0x01       // デフォルト1
#define FDC_CMD_CONFIGURE_POLL_DISABLE    0x01 << 4  // デフォルト0
#define FDC_CMD_CONFIGURE_FIFO_DISABLE    0x01 << 5  // デフォルト1
#define FDC_CMD_CONFIGURE_AUTO_SEEK       0x01 << 6  // デフォルト0
#define FDC_CMD_CONFIGURE_PRETRACK_0      0x00       // デフォルト0


// FDC制御の結果
#define FDC_OK 0x00
#define FDC_ERROR_NOT_READY 0x01
#define FDC_ERROR_RECALIBRATE 0x01

// フロッピーの物理的なパラメータ(3.5inch 2HD 1.44MB)
#define FD_SECTORS 18
#define FD_HEADS 2
#define FD_CYLINDERS 80
#define FD_SECTOR_SIZE 512

#define FD_DRIVE0 0
#define FD_DRIVE1 1

#define FDC_DMA_CHANNEL 2

typedef uint8_t FdcResult;

typedef struct FdcSt0{
    uint8_t ds0:1;
    uint8_t ds1:1;
    uint8_t h:1;
    uint8_t resv0:1;
    uint8_t ec:1;
    uint8_t se:1;
    uint8_t ic0:1;
    uint8_t ic1:1;
} FdcSt0;

typedef uint8_t FdcCmdStatus[7];

typedef struct FdcCmdStatusSenseInterruptStatus{
    union{
        uint8_t st0_data;
        FdcSt0 st0;
    };
    uint8_t pcn; //present cylinder number
} FdcCmdStatusSenseInterruptStatus;

// High level functions
FdcResult init_fdc(void);
FdcResult fdc_cmd_read_data(uint8_t drive, uint16_t phy_addr, uint8_t cylinder, uint8_t head, uint8_t sector);

// FDC controll and general functions
void fdc_motor_on(uint8_t drive);
void fdc_motor_off(uint8_t drive);
bool check_fdc_data_ready(uint8_t direction);
void fdc_read_status(FdcCmdStatus *buf, uint8_t bytes_count);

//FDC commands
FdcResult fdc_cmd_recalibrate(uint8_t drive);
FdcCmdStatusSenseInterruptStatus *fdc_cmd_sense_interrupt_status(FdcCmdStatus *buf);
FdcResult fdc_cmd_specify(uint8_t step_rate, uint8_t head_unload_time, uint8_t head_load_time, uint8_t dma);

FdcResult fdc_enable_auto_seek();
#endif
