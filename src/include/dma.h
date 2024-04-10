#ifndef DMA_H
#define DMA_H

#include "stdint.h"
#include "asmlib.h"
#include "config.h"

#define IO_PORT_DMA_CH2_ADDR  0x04
#define IO_PORT_DMA_CH2_COUNT 0x05
#define IO_PORT_DMA_CH2_PAGE  0x81

#define IO_PORT_DMA_MASTER_CLEAR_BP             0xD8 // (out)

#define IO_PORT_DMA_SLAVE_COMMAND               0x08 // (out)
#define IO_PORT_DMA_SLAVE_STATUS                0x08 // (in)
#define IO_PORT_DMA_SLAVE_SINGLE_CHANNEL_MASK   0x0A // (out)
#define IO_PORT_DMA_SLAVE_CLEAR_BP              0x0C // (out)
#define IO_PORT_DMA_SLAVE_MODE                  0x0B // (out)
#define IO_PORT_DMA_SLAVE_MASK_RESET            0x0E // (out)

// コマンドレジスタで使用できるのはCOND(有効/無効)のみ
#define DMA_COMMAND_COND_ENABLE  0x00 << 2
#define DMA_COMMAND_COND_DISABLE 0x01 << 2

#define DMA_MODE_CH2   0x02
#define DMA_MODE_WRITE 0x01 << 2 // (io->mem)
#define DMA_MODE_READ  0x02 << 2 // (mem->io)
#define DMA_MODE_AUTO  0x01 << 4 // 自動初期化
                                 // DMA終了後に自動的にカレントカウントレジスタと、カレントアドレスレジスタが、前回の値に初期化される
#define DMA_MODE_DOWN  0x01 << 5 // メモリアドレスを減算する
#define DMA_MODE_SINGLE 0x01 << 6 // 1回転送すごとにバスが解放される

#define DMA_MASK_CH2 0x02
#define DMA_MASK_DISABLE 0x00 << 2
#define DMA_MASK_ENABLE 0x01 << 2

void init_fdc_dma(void);

void dma_set_address(uint8_t channel, uint16_t phy_addr);
void dma_set_count(uint8_t channel, uint16_t count);
void dma_set_mode(uint8_t channel, uint8_t mode);
void dma_set_command(uint8_t channel, uint8_t command);
void dma_set_single_mask(uint8_t channel);
void dma_unset_single_mask(uint8_t channel);
void dma_clear_all_mask();

void dma_init_write_mode(uint8_t channel, uint32_t phy_addr, uint16_t count);
#endif
