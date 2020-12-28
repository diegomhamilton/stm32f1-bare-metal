#include <stdint.h>
#include "virtual_timer.h"
#include "reactor.h"
#include "gpio.h"
#include "uart.h"
#include "cmsis_gcc.h"


/*
 * Callback to blink the led to show the app is active
 */
uint32_t led_state = 1 << 29;

hcos_base_int_t blink_cb(void) {
    IOPORT3->BSRR = led_state;
    led_state = __ROR(led_state, 16);
    return 0;
}

/*
 * The rx callback.  We read the data and echo it in the tx side
 */
void echo_cb(hcos_word_t arg) {
    uint8_t n = 0;
    uart_t* drv = (uart_t *) arg;;
    static uint8_t buf[UART_BUFFER_SIZE + 1];

    n = uart_read(drv, buf, UART_BUFFER_SIZE);
    uart_write(drv, buf, n);
}

int main(void) {
    uart_config_t cfg = {.baudrate = 500000,
			 .word_length = 8,
			 .stop_bits = 1,
			 .parity = 0,
			 .over_sampling = 0,
			 .hw_flow_ctl = 0,
			 .rx_threshold = UART_BUFFER_SIZE/2, /* Threshold to call the rx callback */
			 .rx_complete_cb = echo_cb,
			 .tx_complete_cb = 0,
			 .error_cb = 0
    };

    vt_add_non_rt_handler(blink_cb, 250, 1);

    uart_start(&SD1, &cfg);
    reactor_start();

    return 0;
}
