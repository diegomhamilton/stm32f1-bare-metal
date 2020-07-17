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
    uart_config_t cfg = {115200,  /* UART rate */
			 8,  /* Number of bits in a word */
			 1,  /* Number of stop bits */
			 0,  /* Parity flag */
			 0,  /* Over_sampling flag */
			 0,  /* Hw flow control (CTS, RTS) flag */
			 echo_cb,  /* rx_complete cb */
			 0,  /* tx_complete cb */
			 0   /* error cb */
    };

    vt_add_non_rt_handler(blink_cb, 250, 1);

    uart_start(&SD1, &cfg);
    reactor_start();

    return 0;
}
