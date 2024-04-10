#include "dma.h"

void dma_set_address(uint8_t channel, uint16_t phy_addr){
    if(channel == 2){
        io_out8(IO_PORT_DMA_CH2_ADDR, phy_addr & 0xFF);
        io_out8(IO_PORT_DMA_CH2_ADDR, phy_addr >> 8);
    }
    else{
        //TODO: 他のチャンネルの実装
        return;
    }
}

void dma_set_count(uint8_t channel, uint16_t count){
    if(channel == 2){
        io_out8(IO_PORT_DMA_CH2_COUNT, count & 0xFF);
        io_out8(IO_PORT_DMA_CH2_COUNT, count >> 8);
    }
    else{
        //TODO: 他のチャンネルの実装
        return;
    }
}

void dma_set_mode(uint8_t channel, uint8_t mode){
    if(channel == 2){
        io_out8(IO_PORT_DMA_SLAVE_MODE, mode);
    }
    else{
        return;
    }
}

void dma_set_command(uint8_t channel, uint8_t command){
    if(channel == 2){
        io_out8(IO_PORT_DMA_SLAVE_COMMAND, command);
    }
    else{
        return;
    }
}

void dma_set_single_mask(uint8_t channel){
    if(channel == 2){
        io_out8(IO_PORT_DMA_SLAVE_SINGLE_CHANNEL_MASK, DMA_MASK_CH2 | DMA_MASK_ENABLE);
    }
    else{
        return;
    }
}

void dma_unset_single_mask(uint8_t channel){
    if(channel == 2){
        io_out8(IO_PORT_DMA_SLAVE_SINGLE_CHANNEL_MASK, DMA_MASK_CH2 | DMA_MASK_DISABLE);
    }
    else{
        return;
    }
}

void dma_clear_all_mask(){
    //何か書き込むとすべてのチャネルがマスクされる
    io_out8(IO_PORT_DMA_SLAVE_MASK_RESET, 0xFF);
}

void dma_reset_flipflop(uint8_t channel){
    if(channel == 0){
        io_out8(IO_PORT_DMA_MASTER_CLEAR_BP, 0xFF);
    }
    if(channel == 2){
        io_out8(IO_PORT_DMA_SLAVE_CLEAR_BP, 0xFF);
    }
    else{
        return;
    }
}

void dma_set_page(uint8_t channel, uint8_t page){
    if(channel == 2){
        io_out8(IO_PORT_DMA_CH2_PAGE, page);
    }
    else{
        return;
    }
}

/*
void dma_init_write_mode(uint8_t channel, uint16_t phy_addr, uint16_t count){
    io_out8(0x0a, 0x06);       //mask DMA channel 2 and 0 (assuming 0 is already masked)
    io_out8(0x0c, 0xFF);       //reset the master flip-flop
    io_out8(0x04, 0);          //address to 0 (low byte)
    io_out8(0x04, 0x10);       //address to 0x10 (high byte)
    io_out8(0x0c, 0xFF);       //reset the master flip-flop (again!!!)
    io_out8(0x05, 0xFF);       //count to 0x23ff (low byte)
    io_out8(0x05, 0x23);       //count to 0x23ff (high byte),
    io_out8(0x81, 0);          //external page register to 0 for total address of 00 10 00
    io_out8(0x0a, 0x02);       //unmask DMA channel 2
}
*/
void dma_init_write_mode(uint8_t channel, uint32_t phy_addr, uint16_t count){
    //dma_set_command(channel, DMA_COMMAND_COND_ENABLE);
    dma_clear_all_mask();
    dma_reset_flipflop(0);
    dma_set_address(channel, (uint16_t)phy_addr);
    dma_reset_flipflop(0);
    dma_set_count(channel, count);
    dma_set_page(channel, (phy_addr >> 16) & 0xFF);
    dma_set_mode(channel, DMA_MODE_CH2 | DMA_MODE_WRITE | DMA_MODE_SINGLE | DMA_MODE_AUTO);
    dma_unset_single_mask(channel);
}
