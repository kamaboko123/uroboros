#include "fdc.h"

void calc_chs(uint32_t lba, uint16_t *c, uint16_t *h, uint16_t *s){
    *c = lba / (2 * 18);
    *h = (lba / 18) % 2;
    *s = (lba % 18) + 1;
}

void reset_fdc(){
    if(check_fdc_data_ready(FDC_MSR_DIO_WRITE)){
        serial_putstr("FDC is ready\n");
    }
    else{
        serial_putstr("FDC is not ready\n");
    }

    io_out8(IO_PORT_FDC_CCR, FDC_CCR_500K);
    io_out8(IO_PORT_FDC_DOR, 0x00);
    io_out8(IO_PORT_FDC_DOR, FDC_DOR_RESET);
    //io_out8(IO_PORT_FDC_DOR, FDC_DOR_RESET);
    //io_out8(IO_PORT_FDC_DOR, 0);
}

void fdc_motor_on(uint8_t drive){
    if(drive == 0){
        io_out8(IO_PORT_FDC_DOR, FDC_DOR_DRIVE0 | FDC_DOR_MOTOR0);
    }
    else if(drive == 1){
        io_out8(IO_PORT_FDC_DOR, FDC_DOR_DRIVE1 | FDC_DOR_MOTOR1);
    }
}

bool check_fdc_data_ready(uint8_t direction){
    uint8_t msr = io_in8(IO_PORT_FDC_MSR);
    return (msr & FDC_MSR_RQM) != 0 && (msr & FDC_MSR_DIO) == direction;
}

