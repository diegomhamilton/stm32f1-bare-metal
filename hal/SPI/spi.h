#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include "stm32f103xb.h"
#include "reactor.h"

#ifdef SPI_USE_DMA
#undef SPI_USE_DMA
#define SPI_USE_DMA 1
#define SPI_DMA_PRIO DMA_PRIO_VERY_HIGH
#endif /* SPI_USE_DMA */

#define SPI_DATAFRAME_8BIT 0
#define SPI_DATAFRAME_16BIT SPI_CR1_DFF

#define SPI_MSBFIRST 0
#define SPI_LSBFIRST SPI_CR1_LSBFIRST
/* SPI_CR1_SPE */

#define SPI_BAUD_RATE_DIV2   0
#define SPI_BAUD_RATE_DIV4   (1 << SPI_CR1_BR_Pos)
#define SPI_BAUD_RATE_DIV8   (2 << SPI_CR1_BR_Pos)
#define SPI_BAUD_RATE_DIV16  (3 << SPI_CR1_BR_Pos)
#define SPI_BAUD_RATE_DIV32  (4 << SPI_CR1_BR_Pos)
#define SPI_BAUD_RATE_DIV64  (5 << SPI_CR1_BR_Pos)
#define SPI_BAUD_RATE_DIV128 (6 << SPI_CR1_BR_Pos)
#define SPI_BAUD_RATE_DIV256 (7 << SPI_CR1_BR_Pos)

#define SPI_CPOL_IDLE_LOW 0
#define SPI_CPOL_IDLE_HIGH SPI_CR1_CPOL

#define SPI_CPHA_FIRST  0
#define SPI_CPHA_SECOND SPI_CR1_CPHA

typedef struct {
    uint32_t data_frame_format;
    uint32_t lsb_first;
    uint32_t baud_rate;
    uint32_t clock_polarization;
    uint32_t clock_phase;
#if SPI_USE_DMA == 1
    reactor_cb_t half_transfer_cb;
    reactor_cb_t full_transfer_cb;
#endif
    uint8_t master_mode;
} spi_config_t;

typedef struct {
    SPI_TypeDef *dev;               /* SPI device */
    uint8_t *tx_buf;
    uint8_t *rx_buf;
    uint8_t *tx_ptr;
    uint8_t *rx_ptr;
    int bytes_remaining;
    reactor_cb_t rt_end_cb;
#if SPI_USE_DMA == 1
    uint32_t dma_rx_channel;
    uint32_t dma_tx_channel;
    reactor_cb_t half_transfer_cb;
    reactor_cb_t full_transfer_cb;
#endif
} spi_t;

extern spi_t SPID1;
extern spi_t SPID2;

/*
 * Macros definitions
 */
#define spi_crc_enable(drv) (drv->dev->CR1 |= SPI_CR1_CRCEN)
#define spi_is_tx_busy(drv) (drv->dev->SR & SPI_SR_TXE)
#define spi_putc(drv, b) do {				\
	while ((drv->dev->SR & SPI_SR_TXE) == 0)	\
	    ;						\
	drv->dev->DR = b;				\
    } while(0)
#define spi_getc(drv, b) do {				\
	while ((drv->dev->SR & SPI_SR_RXNE) == 0)	\
	    ;						\
	b = drv->dev->DR;				\
    } while(0)


/*
 * Functions definitions
 */
void spi_init(void);
void spi_start(spi_t *drv, spi_config_t *config);
void spi_stop(spi_t *drv);

void spi_when_ready(spi_t *drv, reactor_cb_t cb);

#define spi_write(drv, tx_buf, n, cb) spi_write_read(drv, tx_buf, 0, n, cb)
#define spi_read(drv, rx_buf, n, cb) spi_write_read(drv, 0, rx_buf, n, cb)
int spi_write_read(spi_t *drv, uint8_t* tx_buf, uint8_t* rx_buf, int n, reactor_cb_t cb);

#if SPI_USE_DMA == 1
int spi_write_dma(spi_t *drv, uint8_t* tx_buf, int n, reactor_cb_t cb);
int spi_read_dma(spi_t *drv, uint8_t* rx_buf, int n, reactor_cb_t cb);
int spi_write_read_dma(spi_t *drv, const uint8_t* tx_buf, uint8_t* rx_buf, int n, reactor_cb_t cb);
#endif /* SPI_USE_DMA == 1 */

#endif
