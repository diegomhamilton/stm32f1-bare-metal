#include "uart.h"
#include "buffer.h"

void uart_init(uart_config_t config) {
}

uint8_t uart_write(uart_t* drv, uint8_t c) {
    return buffer_push(drv->tx_buf, c);
}

uint16_t uart_read(uart_t* drv) {
    return buffer_pop(drv->rx_buf);
}
