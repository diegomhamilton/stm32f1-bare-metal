#ifndef UART_H
#define UART_H

#include <stdint.h>
#include "buffers.h"
#include "stm32f103xb.h"

#define UART_BUFFER_SIZE 16

/*
 * Struct definition
 */

typedef struct {
} uart_config_t;

typedef struct {
    BUFFER_DECL(rx_buf, UART_BUFFER_SIZE);
    BUFFER_DECL(tx_buf, UART_BUFFER_SIZE);  /* Declaring the buffers */
    USART_TypeDef* dev;
} uart_t;


/*
 * Device definitions
 */

uart_t SD1 = {0, 0, USART1};


/*
 * Functions
 */

void uart_init(uart_config_t config);

uint8_t uart_write(uart_t* drv, uint8_t c);
uint8_t uart_read(uart_t* drv);

#endif /* UART_H */

