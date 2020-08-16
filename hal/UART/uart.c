#include <stdint.h>
#include "stm32f103xb.h"
#include "dma.h"
#include "uart.h"
#include "buffers.h"
#include "rcc.h"
#include "reactor.h"


uart_pindata_t pin_cfgs[] = {{GPIOA,  9, 10, SPEED_2_MHZ},
			     {GPIOA,  2,  3, SPEED_2_MHZ},
			     {GPIOB, 10, 11, SPEED_2_MHZ}};

uart_t SD1;
uart_t SD2;
uart_t SD3;

void uart_init() {
    SD1.dev = USART1;
    SD1.pins = pin_cfgs;
    SD1.last_error_code = 0;
    SD1.rx_threshold = 0;
    SD1.rx_complete_cb = 0;
    SD1.tx_complete_cb = 0;
    SD1.error_cb = 0;

    SD2.dev = USART2;
    SD2.pins = pin_cfgs + 1;
    SD2.last_error_code = 0;
    SD2.rx_threshold = 0;
    SD2.rx_complete_cb = 0;
    SD2.tx_complete_cb = 0;
    SD2.error_cb = 0;

    SD3.dev = USART3;
    SD3.pins = pin_cfgs + 2;
    SD3.last_error_code = 0;
    SD3.rx_threshold = 0;
    SD3.rx_complete_cb = 0;
    SD3.tx_complete_cb = 0;
    SD3.error_cb = 0;

#if UART_USE_DMA == 1
    SD1.dma_rx_channel = DMA_UART1_RX_CHANNEL;
    SD1.dma_tx_channel = DMA_UART1_TX_CHANNEL;

    SD2.dma_rx_channel = DMA_UART2_RX_CHANNEL;
    SD2.dma_tx_channel = DMA_UART2_TX_CHANNEL;

    SD3.dma_rx_channel = DMA_UART3_RX_CHANNEL;
    SD3.dma_tx_channel = DMA_UART3_TX_CHANNEL;
#endif /* UART_USE_DMA */
}

void uart_start(uart_t* drv, uart_config_t* config) {
    uint16_t cr1;
    uint32_t fck;

    drv->rx_threshold = config->rx_threshold > UART_BUFFER_SIZE - 1 ?
	UART_BUFFER_SIZE - 1 : config->rx_threshold;
    drv->rx_complete_cb = config->rx_complete_cb;
    drv->tx_complete_cb = config->tx_complete_cb;
    drv->error_cb = config->error_cb;

    RCC_AFIO_CLK_ENABLE();
    if (drv == &SD1) {
	RCC_USART1_CLK_ENABLE();
    } else if (drv == &SD2) {
	RCC_USART2_CLK_ENABLE();
    } else if (drv == &SD3) {
	RCC_USART3_CLK_ENABLE();
    }

    buffer_reset(drv->tx_buf);
    buffer_reset(drv->rx_buf);

#if UART_USE_DMA == 1
    dma_start(&DMAD1);
#endif /* UART_USE_DMA */

    gpio_set_pin_mode(drv->pins->port, drv->pins->tx_pin, OUT_AF_PUSHPULL);
    gpio_set_pin_mode(drv->pins->port, drv->pins->rx_pin, IN_PULLUP);

    /* Enable usart module, receiver and receive interrupt */
    cr1 = USART_CR1_UE | USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_TE;

    /* Compute the BRR value based on a APB2 clock speed of 72 MHz */
    /* TODO: write the logic for the other UARTs and other AHB clock
       speeds */
    fck = 72000000 / config->baudrate;

    /* TODO: Configure for parity, word length, and error detection */
    /* TODO: Support for Synchronous mode, half duplex mode and CTS
       and HW flow control */
    /* TODO: Support for LIN, IrDA and smartcard */
    drv->dev->CR1 = cr1;
    drv->dev->BRR = fck;

    (void) drv->dev->SR;  /* Error flags reset step 1.*/
    (void) drv->dev->DR;  /* Error flags reset step 2.*/

    if (drv == &SD1) {
	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn, 7); /* Middle Range priority */
    } else if (drv == &SD2) {
	NVIC_EnableIRQ(USART2_IRQn);
	NVIC_SetPriority(USART2_IRQn, 7); /* Middle Range priority */
    } else if (drv == &SD3) {
	NVIC_EnableIRQ(USART3_IRQn);
	NVIC_SetPriority(USART3_IRQn, 7); /* Middle Range priority */
    }
}

void uart_stop(uart_t* drv) {
    /* We don't disable the AFIO clock since someone else may be using
       it */
    if (drv == &SD1) {
	NVIC_DisableIRQ(USART1_IRQn);
	RCC_USART1_CLK_DISABLE();
    } else if (drv == &SD2) {
	NVIC_DisableIRQ(USART2_IRQn);
	RCC_USART2_CLK_DISABLE();
    } else if (drv == &SD3) {
	NVIC_DisableIRQ(USART3_IRQn);
	RCC_USART3_CLK_DISABLE();
    }

    drv->dev->CR1 = drv->dev->CR2 = drv->dev->CR3 = 0;
}

#define uart_pos_push_action(buf, c, ret_val) {}
#define uart_buffer_push(buf, c, ret_val) buffer_generic_push(buf, c, ret_val,\
							      BUFFER_PUSH_FULL_ACTION, uart_pos_push_action)
uint8_t uart_putc(uart_t* drv, uint8_t c) {
    uint8_t rv;

#if UART_USE_DMA == 1
    if (dma_is_busy(&DMAD1, drv->dma_tx_channel))
	return -2;
#endif

    uart_buffer_push(drv->tx_buf, c, rv);
    drv->dev->CR1 |= USART_CR1_TXEIE;
    return rv;
}

#define uart_pos_pop_action(buf, c) {}
#define uart_buffer_pop(buf, c) buffer_generic_pop(buf, c, BUFFER_POP_EMPTY_ACTION, \
						   uart_pos_pop_action)
uint16_t uart_getc(uart_t* drv) {
    uint16_t c = 0;
    uart_buffer_pop(drv->rx_buf, c);
    return c;
}

uint32_t ll = 1 << 13;
int uart_write(uart_t* drv, const uint8_t *msg, int n) {
    int ctr = 0;
    uint8_t rv = 0;

#if UART_USE_DMA == 1
    if (dma_is_busy(&DMAD1, drv->dma_tx_channel))
	return 0;
#endif

    /* Write characters until either the end of the message or the
       buffer is full */
    while (ctr++ < n && rv != -1) {
	uart_buffer_push(drv->tx_buf, *msg, rv);
	msg++;
    }
    drv->dev->CR1 |= USART_CR1_TXEIE;
    return --ctr;
}

int uart_read(uart_t* drv, uint8_t *buf, int n) {
    int i = 0;
    uint16_t c;
    while (i < n && (c = uart_getc(drv)) != (uint16_t) -1) {
	*buf++ = (uint8_t) c;
	i++;
    }
    return i;
}

#if UART_USE_DMA == 1
void uart_write_dma_cb(hcos_word_t arg) {
    uart_t *drv = (uart_t *) arg;

    drv->dev->CR3 &= ~USART_CR3_DMAT;
}

int uart_write_dma(uart_t* drv, const uint8_t *buf, int n, reactor_cb_t write_end_cb) {
    /* First test if we are already transmitting, in which case we
       return an error */
    dma_bind_config_t cfg = {.peripheral_address = (uint32_t) &drv->dev->DR,
			     .memory_address  = (uint32_t) buf,
			     .nbr_bytes       = n,
			     .mem2mem_flag    = 0,
			     .priority        = UART_DMA_PRIO,
			     .peripheral_size = DMA_PER_SIZE_8BITS,
			     .memory_size     = DMA_MEM_SIZE_8BITS,
			     .peripheral_increment = DMA_NO_PER_INCREMENT_MODE,
			     .memory_increment = DMA_MEM_INCREMENT_MODE,
			     .direction = DMA_FROM_MEM_MODE,
			     .circular_mode = DMA_NO_CIRCULAR_MODE,
			     .half_transfer_cb = 0,
			     .full_transfer_cb = write_end_cb,
			     .error_cb = 0,
			     .half_transfer_args = 0,
			     .full_transfer_args = (hcos_word_t) drv,
			     .error_args = 0,
			     .half_transfer_rt_cb = 0,
			     .full_transfer_rt_cb = uart_write_dma_cb,
			     .error_rt_cb = 0,
			     .half_transfer_rt_args = 0,
			     .full_transfer_rt_args = (hcos_word_t) drv,
			     .error_rt_args = 0,
    };

    if (uart_is_busy_tx(drv) || dma_is_busy(&DMAD1, drv->dma_tx_channel))
	return 1;

    dma_bind(&DMAD1, drv->dma_tx_channel, cfg);
    drv->dev->CR3 |= USART_CR3_DMAT;
    drv->dev->SR &= ~USART_SR_TC;
    dma_enable(&DMAD1, drv->dma_tx_channel);

    return 0;
}

void uart_read_dma_cb(hcos_word_t arg) {
    uart_t *drv = (uart_t *) arg;

    drv->dev->CR3 &= ~USART_CR3_DMAT;
    drv->dev->CR1 |= USART_CR1_RXNEIE;
}

int uart_read_dma(uart_t* drv, uint8_t *buf, int n, reactor_cb_t read_end_cb) {
    dma_bind_config_t cfg = {.peripheral_address = (uint32_t) &drv->dev->DR,
			     .memory_address  = (uint32_t) buf,
			     .nbr_bytes       = n,
			     .mem2mem_flag    = 0,
			     .priority        = UART_DMA_PRIO,
			     .peripheral_size = DMA_PER_SIZE_8BITS,
			     .memory_size     = DMA_MEM_SIZE_8BITS,
			     .peripheral_increment = DMA_NO_PER_INCREMENT_MODE,
			     .memory_increment = DMA_MEM_INCREMENT_MODE,
			     .direction = DMA_FROM_PER_MODE,
			     .circular_mode = DMA_NO_CIRCULAR_MODE,
			     .half_transfer_cb = 0,
			     .full_transfer_cb = read_end_cb,
			     .error_cb = 0,
			     .half_transfer_args = 0,
			     .full_transfer_args = (hcos_word_t) drv,
			     .error_args = 0,
			     .half_transfer_rt_cb = 0,
			     .full_transfer_rt_cb = uart_read_dma_cb,
			     .error_rt_cb = 0,
			     .half_transfer_rt_args = 0,
			     .full_transfer_rt_args = (hcos_word_t) drv,
			     .error_rt_args = 0,
    };

    if (dma_is_busy(&DMAD1, drv->dma_rx_channel))
	return 1;

    drv->dev->CR1 &= ~USART_CR1_RXNEIE;
    dma_bind(&DMAD1, drv->dma_rx_channel, cfg);
    drv->dev->CR3 |= USART_CR3_DMAT;
    dma_enable(&DMAD1, drv->dma_rx_channel);

    return 0;
}
#endif /* UART_USE_DMA */


/*
 * IRQ handler definitions
 */
#define uart_irq_pos_pop_action(buf, c) do {	\
    } while (0)
#define uart_irq_pop_empty_action(buf, c) do {				\
	(USART1->CR1 = cr1 & ~USART_CR1_TXEIE);				\
	if (SD1.tx_complete_cb)						\
	    reactor_add_handler(SD1.tx_complete_cb, (hcos_word_t) &SD1); \
    } while(0)
#define uart_irq_buffer_pop(buf, c) buffer_generic_pop(buf, c, uart_irq_pop_empty_action, \
						   uart_irq_pos_pop_action)
void USART1_IRQHandler() {
    uint16_t st = USART1->SR;
    uint32_t cr1 = USART1->CR1;
    uint32_t errorflags;

    if (((st & USART_SR_TXE) != 0) && ((cr1 & USART_CR1_TXEIE) != 0))
	uart_irq_buffer_pop(SD1.tx_buf, USART1->DR);

    errorflags = (st & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));
    if (!errorflags) {
	/* TODO: Decide whether we only handle a received byte if the
	   interrupt is enabled or not */
	if (((st & USART_SR_RXNE) != 0) && ((cr1 & USART_CR1_RXNEIE) != 0)) {
	    uint8_t rv;
	    uint8_t c = USART1->DR;
	    uart_buffer_push(SD1.rx_buf, c, rv);
	    if (rv != -1 && SD1.rx_complete_cb && buffer_occupancy(SD1.rx_buf) >= SD1.rx_threshold)
		reactor_add_handler(SD1.rx_complete_cb, (hcos_word_t) &SD1);
	}
    }
}
