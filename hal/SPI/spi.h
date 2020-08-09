#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include "stm32f103xb.h"

#ifdef SPI_USE_DMA
#undef SPI_USE_DMA
#define SPI_USE_DMA 1
#define SPI_DMA_PRIO DMA_PRIO_VERY_HIGH
#endif /* SPI_USE_DMA */

typedef struct {
#if SPI_USE_DMA == 1
    reactor_cb_t half_transfer_cb;
    reactor_cb_t full_transfer_cb;
#endif
} spi_config_t;

typedef struct {
    SPI_TypeDef *dev;               /* SPI device */
#if SPI_USE_DMA == 1
    uint32_t dma_channel;           /* DMA channel for this SPI */
#endif
} spi_t;

extern spi_t SPID1;
extern spi_t SPID2;

void spi_init(void);
void spi_start(spi_t *drv, spi_config_t *config);
void spi_stop(spi_t *drv);

#endif
