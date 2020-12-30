#include "spi.h"
#include "gpio.h"
#include "reactor.h"
#include "rcc.h"


spi_t SPID1;
spi_t SPID2;


void spi_init(void) {
    SPID1.dev = SPI1;
    SPID2.dev = SPI2;

#if SPI_USE_DMA == 1
    SPID1.half_transfer_cb = SPID1.full_transfer_cb = 0;
    SPID1.dma_rx_channel = DMA_SPI1_RX_CHANNEL;
    SPID1.dma_tx_channel = DMA_SPI1_TX_CHANNEL;

    SPID2.half_transfer_cb = SPID2.full_transfer_cb = 0;
    SPID2.dma_rx_channel = DMA_SPI2_RX_CHANNEL;
    SPID2.dma_tx_channel = DMA_SPI2_TX_CHANNEL;
#endif
}

/* TODO: For now, only implements the full duplex mode with the NSS
   pin configured for output */
void spi_start(spi_t *drv, spi_config_t *config) {
    RCC_AFIO_CLK_ENABLE();
    if (drv == &SPID1) {
	RCC_SPI1_CLK_ENABLE();
	/* TODO: Implement remap someday */
	if (config->master_mode) {
	    gpio_set_pin_mode(IOPORT1, 5, OUT_AF_PUSHPULL); /* SCK SPI1 */
	    gpio_set_pin_mode(IOPORT1, 7, OUT_AF_PUSHPULL); /* MOSI SPI1 */
	    gpio_set_pin_mode(IOPORT1, 6, IN_PULLUP);       /* MISO SPI1 */
	    gpio_set_pin_mode(IOPORT1, 4, OUT_AF_PUSHPULL); /* NSS SPI1 */
	} else {
	    gpio_set_pin_mode(IOPORT1, 5, IN_FLOATING);     /* SCK SPI1 */
	    gpio_set_pin_mode(IOPORT1, 7, IN_PULLUP);       /* MOSI SPI1 */
	    gpio_set_pin_mode(IOPORT1, 6, OUT_AF_PUSHPULL); /* MISO SPI1 */
	    gpio_set_pin_mode(IOPORT1, 4, IN_PULLUP);       /* NSS SPI1 */
	}
    } else if (drv == &SPID2) {
	RCC_SPI2_CLK_ENABLE();
	/* TODO: Implement remap someday */
	if (config->master_mode) {
	    gpio_set_pin_mode(IOPORT2, 13, OUT_AF_PUSHPULL); /* SCK SPI2 */
	    gpio_set_pin_mode(IOPORT2, 15, OUT_AF_PUSHPULL); /* MOSI SPI2 */
	    gpio_set_pin_mode(IOPORT2, 14, IN_PULLUP);       /* MISO SPI2 */
	    gpio_set_pin_mode(IOPORT2, 12, OUT_AF_PUSHPULL); /* NSS SPI2 */
	} else {
	    gpio_set_pin_mode(IOPORT1, 13, IN_FLOATING);     /* SCK SPI2 */
	    gpio_set_pin_mode(IOPORT1, 15, IN_PULLUP);       /* MOSI SPI2 */
	    gpio_set_pin_mode(IOPORT1, 14, OUT_AF_PUSHPULL); /* MISO SPI2 */
	    gpio_set_pin_mode(IOPORT1, 12, IN_PULLUP);       /* NSS SPI2 */
	}
    }

    if (config->master_mode) {
	drv->dev->CR1 &= ~SPI_CR1_SPE;
	drv->dev->CR2 = SPI_CR2_SSOE;
	drv->dev->CR1 = config->baud_rate | config->clock_polarization |
	    config->clock_phase | config->data_frame_format |
	    config->lsb_first | SPI_CR1_MSTR;
	drv->dev->CR1 |= SPI_CR1_SPE;
    } else {
	drv->dev->CR1 &= ~SPI_CR1_SPE;
	drv->dev->CR1 = config->clock_polarization | config->clock_phase |
	    config->data_frame_format | config->lsb_first;
	drv->dev->CR1 |= SPI_CR1_SPE;
    }

#if SPI_USE_DMA == 1
    dma_start(&DMAD1);
#endif /* SPI_USE_DMA == 1 */

    if (drv == &SPID1) {
	NVIC_EnableIRQ(SPI1_IRQn);
	NVIC_SetPriority(USART1_IRQn, 1); /* High priority */
    } else if (drv == &SPID2) {
	NVIC_EnableIRQ(SPI2_IRQn);
	NVIC_SetPriority(USART2_IRQn, 1); /* High priority */
    }
}

void spi_stop(spi_t *drv) {
    if (drv == &SPID1) {
	NVIC_DisableIRQ(SPI1_IRQn);
	RCC_SPI1_CLK_DISABLE();
    } else if (drv == &SPID2) {
	NVIC_DisableIRQ(SPI2_IRQn);
	RCC_SPI2_CLK_DISABLE();
    }

    /* We avoid turning off the DMA module here since others may be
       using it */
}

/* TODO: Implement callback mechanism to signal when the spi becomes
   idle */
void spi_when_ready(spi_t *drv, reactor_cb_t cb) {
}

int spi_write_read(spi_t *drv, uint8_t* tx_buf, uint8_t* rx_buf, int n,
		   reactor_cb_t rt_cb) {
    if (spi_is_tx_busy(drv))
	return -1;

    drv->tx_buf = tx_buf;
    drv->rx_buf = rx_buf;
    drv->tx_ptr = tx_buf;
    drv->rx_ptr = rx_buf;
    drv->bytes_remaining = n;
    drv->rt_end_cb = rt_cb;

    if (tx_buf)
	drv->dev->CR2 |= SPI_CR2_TXEIE;

    if (rx_buf) {
	(void) drv->dev->DR; /* Clears the RXNE flag */
	drv->dev->CR2 |= SPI_CR2_RXNEIE;
    }

    return 0;
}

#if SPI_USE_DMA == 1
int spi_write_dma(spi_t *drv, uint8_t* tx_buf, int n, reactor_cb_t cb) {
    return 0;
}

int spi_read_dma(spi_t *drv, uint8_t* rx_buf, int n, reactor_cb_t cb) {
    return 0;
}

int spi_write_read_dma(spi_t *drv, uint8_t* tx_buf, uint8_t* rx_buf, int n,
		       reactor_cb_t cb) {
    return 0;
}
#endif /* SPI_USE_DMA == 1 */

void SPI1_IRQHandler(void) {
    uint8_t dr;
    uint32_t sr = SPI1->SR;

    if (sr & SPI_SR_RXNE) {
	/* Handle receiving */
	dr = SPI1->DR;
	if (SPID1.rx_ptr)
	    *SPID1.rx_ptr++ = dr;
    }
    if (sr & SPI_SR_TXE) {
	/* Handle transmitting */
	if (SPID1.tx_ptr)
	    SPI1->DR = *SPID1.tx_ptr++;
	else
	    SPI1->DR = 0xFF;
    }

    if (--SPID1.bytes_remaining <= 0) {
	SPI1->CR2 &= (~SPI_CR2_TXEIE & ~SPI_CR2_RXNEIE & ~SPI_CR2_ERRIE);
	SPID1.tx_ptr = SPID1.rx_ptr = 0;
	if (SPID1.rt_end_cb)
	    SPID1.rt_end_cb(0);
    }
}

void SPI2_IRQHandler(void) {
    /* TODO: Implement an IRQ-based SPI handling */
}
