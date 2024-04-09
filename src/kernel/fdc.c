#include "fdc.h"

void calc_chs(uint32_t lba, uint16_t *c, uint16_t *h, uint16_t *s){
    *c = lba / (2 * 18);
    *h = (lba / 18) % 2;
    *s = (lba % 18) + 1;
}

void init_fdc_dma(void){}

FDC_RESULT init_fdc(){
    //割り込みフラグを戻しておく
    sys->fdc_intr = false;

    //コマンドを送信する前にFDCがデータを受け付ける状態になっているか確認
    if(!check_fdc_data_ready(FDC_MSR_DIO_WRITE)){
        serial_putstr("FDC is not ready\n");
        return FDC_ERROR_NOT_READY;
    }

    //リセット(リセット後は割り込みが発生する)
    io_out8(IO_PORT_FDC_DOR, 0x00);
    io_out8(IO_PORT_FDC_DOR, FDC_DOR_RESET);

    //TODO: timeout
    while(!sys->fdc_intr);
    sys->fdc_intr = false;

    //SENSE_INTERRUPT_STATUSを4回送信する
    //これによりFDCの割り込みフラグ(ST1)がクリアされる
    //4回叩くのは各ドライブに対して行われるかららしい(ドライブ指定しなくて良い？自動？)
    FDC_CMD_STATUS status_buf;
    for(int i = 0; i < 4; i++){
        fdc_cmd_sense_interrupt_status(&status_buf);
    }

    //FDCの設定をしていく
    //データレート
    io_out8(IO_PORT_FDC_CCR, FDC_CCR_500K);

    //物理的な動作時間の設定なので、qemuなら何でも良い？
    fdc_cmd_specify(
        FDC_CMD_SPECIFY_PARAM_STEP_RATE,
        FDC_CMD_SPECIFY_PARAM_HEAD_UNLOAD_TIME,
        FDC_CMD_SPECIFY_PARAM_HEAD_LOAD_TIME,
        FDC_CMD_SPECIFY_PARAM_DMA_ENABLE
    );

    //最初にRECALIBRATEを実施する必要がある(ヘッドをトラック0に戻す)
    //使用するドライブのモータをONにしておく必要がある
    for(int i = 0; i < 2; i++){
        FDC_RESULT result;
        fdc_motor_on(i);
        result = fdc_cmd_recalibrate(i);
        if(result != FDC_OK) return result;
        fdc_motor_off(i);
    }

    fdc_motor_on(0);
    return FDC_OK;
}

//TODO: impl
void fdc_cmd_read_data(){
    sys->fdc_intr = false;
    io_out8(IO_PORT_FDC_DATA, FDC_CMD_READ_DATA | 0x40); //multi track
    io_out8(IO_PORT_FDC_DATA, 0x00);//drive0
    io_out8(IO_PORT_FDC_DATA, 0x00);//cyliner0
    io_out8(IO_PORT_FDC_DATA, 0x00);//head0
    io_out8(IO_PORT_FDC_DATA, 0x00);//sector0
    io_out8(IO_PORT_FDC_DATA, 0x02);//sector_size(512KB)
    io_out8(IO_PORT_FDC_DATA, 18);//sector per track(18)
    io_out8(IO_PORT_FDC_DATA, 27);//gap3
    io_out8(IO_PORT_FDC_DATA, 0xff);//data length

    while(!sys->fdc_intr);
    sys->fdc_intr = false;

    FDC_CMD_STATUS status_buf;
    fdc_read_status(&status_buf);
    for(int i = 0; i < 7; i++){
        char str[64];
        sprintf(str, "status_buf[%d]: %x\n", i, ((uint8_t *)&status_buf)[i]);
        serial_putstr(str);
    }
}

FDC_CMD_STATUS_SENSE_INTERRUPT_STATUS *fdc_cmd_sense_interrupt_status(FDC_CMD_STATUS *buf){
    io_out8(IO_PORT_FDC_DATA, FDC_CMD_SENSE_INTERRUPT_STATUS);
    
    fdc_read_status(buf);
    return (FDC_CMD_STATUS_SENSE_INTERRUPT_STATUS *)buf;
}

FDC_RESULT fdc_cmd_recalibrate(uint8_t drive){
    if(!check_fdc_data_ready(FDC_MSR_DIO_WRITE)) return FDC_ERROR_NOT_READY;

    FDC_CMD_STATUS status_buf;
    FDC_CMD_STATUS_SENSE_INTERRUPT_STATUS *status;

    io_out8(IO_PORT_FDC_DATA, FDC_CMD_RECALIBRATE);
    io_out8(IO_PORT_FDC_DATA, drive);

    // TODO: timeout
    while(!sys->fdc_intr);
    sys->fdc_intr = false;
    status = fdc_cmd_sense_interrupt_status(&status_buf);
    if(((status->st0_data & 0x03) != drive) || (status->st0.se != 1) || ((status->st0_data & 0x0c) != 0)){
        return FDC_ERROR_RECALIBRATE;
    }

    return FDC_OK;
}

FDC_RESULT fdc_cmd_specify(uint8_t step_rate, uint8_t head_unload_time, uint8_t head_load_time, uint8_t dma){
    if(!check_fdc_data_ready(FDC_MSR_DIO_WRITE)) return FDC_ERROR_NOT_READY;
    io_out8(IO_PORT_FDC_DATA, FDC_CMD_SPECIFY);
    io_out8(IO_PORT_FDC_DATA, (step_rate << 4) | head_unload_time);
    io_out8(IO_PORT_FDC_DATA, head_load_time << 1 | dma);
    
    //cmd command doesn't return status
    FDC_CMD_STATUS buf;
    fdc_read_status(&buf);
    return FDC_OK;
}

void fdc_read_status(FDC_CMD_STATUS *buf){
    uint8_t *p = (uint8_t *)buf;
    for(int i = 0; i < 7; i++){
        *(p + i) = io_in8(IO_PORT_FDC_DATA);
    }
}

void fdc_motor_on(uint8_t drive){
    if(drive == 0){
        io_out8(IO_PORT_FDC_DOR, FDC_DOR_DRIVE0 | FDC_DOR_MOTOR0 | FDC_DOR_RESET | FDC_DOR_DMA);
    }
    else if(drive == 1){
        io_out8(IO_PORT_FDC_DOR, FDC_DOR_DRIVE1 | FDC_DOR_MOTOR1 | FDC_DOR_RESET | FDC_DOR_DMA);
    }
    sleep(300);
}

void fdc_motor_off(uint8_t drive){
    if(drive == 0){
        io_out8(IO_PORT_FDC_DOR, FDC_DOR_DRIVE0 | FDC_DOR_RESET | FDC_DOR_DMA);
    }
    else if(drive == 1){
        io_out8(IO_PORT_FDC_DOR, FDC_DOR_DRIVE1 | FDC_DOR_RESET | FDC_DOR_DMA);
    }
}

bool check_fdc_data_ready(uint8_t direction){
    uint8_t msr = io_in8(IO_PORT_FDC_MSR);
    return (msr & FDC_MSR_RQM) != 0 && (msr & FDC_MSR_DIO) == direction;
}

