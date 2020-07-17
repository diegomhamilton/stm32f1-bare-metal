#ifndef UART_H
#define UART_H

#include <stdint.h>
#include "gpio.h"
#include "buffers.h"
#include "stm32f103xb.h"
#include "reactor.h"

#define UART_BUFFER_SIZE 16

typedef BYTE_BUFFER_DEF(UART_BUFFER_SIZE) uart_buffer_t;

struct uart_t;
typedef void (*uart_cb_t)(struct uart_t*);

/*
 * Struct definition
 */

typedef struct {
    GPIO_TypeDef* port;
    uint8_t tx_pin;
    uint8_t rx_pin;
    uint8_t tx_pin_speed;
} uart_pindata_t;

typedef struct {
    uint32_t baudrate;
    uint32_t word_length;
    uint32_t stop_bits;
    uint32_t parity;
    uint32_t over_sampling;
    uint32_t hw_flow_ctl;

    reactor_cb_t rx_complete_cb;
    reactor_cb_t tx_complete_cb;
    reactor_cb_t error_cb;
} uart_config_t;

typedef struct {
    uart_buffer_t rx_buf;
    uart_buffer_t tx_buf;
    USART_TypeDef* dev;
    uart_pindata_t* pins;
    uart_config_t* config;
    uint32_t last_error_code;
    uint32_t rx_threshold;

    /* Callbacks used by the IRQ handlers */
    reactor_cb_t rx_complete_cb;
    reactor_cb_t tx_complete_cb;
    reactor_cb_t error_cb;
} uart_t;


/*
 * Device definitions
 */

extern uart_t SD1;
extern uart_t SD2;
extern uart_t SD3;


/*
 * Functions
 */

void uart_init();
void uart_start(uart_t* drv, uart_config_t* config);

uint8_t uart_putc(uart_t* drv, uint8_t c);
uint16_t uart_getc(uart_t* drv);
int uart_write(uart_t* drv, const uint8_t *buf, int n);
int uart_read(uart_t* drv, uint8_t *buf, int n);

#define uart_tx_free_space(drv) buffer_free_space((drv)->tx_buf)
#define uart_rx_used_space(drv) buffer_occupancy((drv)->rx_buf)

#endif /* UART_H */
