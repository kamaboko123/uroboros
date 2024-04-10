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

void dma_set_command(uint8_t dmac, uint8_t command){
    if(dmac == DMAC_SLAVE){
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

void dma_reset_flipflop(uint8_t dmac){
    if(dmac == DMAC_MASTER){
        io_out8(IO_PORT_DMA_MASTER_CLEAR_BP, 0xFF);
    }
    else if(dmac == DMAC_SLAVE){
        io_out8(IO_PORT_DMA_SLAVE_CLEAR_BP, 0xFF);
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

void dma_init_for_fd_read(uint8_t channel, uint32_t phy_addr, uint16_t count){
    // enable slave DMAC
    dma_set_command(DMAC_SLAVE, DMA_COMMAND_COND_ENABLE);
    // mask all channels
    dma_clear_all_mask();

    // set address, count, page
    // TODO: MASTERのリセット必要らしいけど本当？
    // bochではなくても動くので、とりあえずコメントアウト
    //dma_reset_flipflop(DMAC_MASTER);
    dma_set_address(channel, (uint16_t)phy_addr);
    //dma_reset_flipflop(DMAC_MASTER);
    dma_set_count(channel, count);
    dma_set_page(channel, (phy_addr >> 16) & 0xFF);

    // set mode(write: io -> mem)
    dma_set_mode(channel, DMA_MODE_CH2 | DMA_MODE_WRITE | DMA_MODE_SINGLE | DMA_MODE_AUTO);

    //enable selected channel
    dma_unset_single_mask(channel);
}
