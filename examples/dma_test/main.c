#include "uart.h"
#include <stdint.h>
#include "cmsis_gcc.h"
#include "virtual_timer.h"
#include "reactor.h"
#include "gpio.h"
#include "dma.h"


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
 * 
 */
/* void test1_cb(hcos_word_t arg) { */

/*
 * The test callback.  We just try to use memset() and send a message
 * by UART depending on the test outcome
 */
#define BUFSZ 1024
static uint8_t buf1[BUFSZ];
static uint8_t buf2[BUFSZ];
static const uint8_t msg1[] = "Test of memset worked!\n\r";
static const uint8_t msg2[] = "Test of memset DID NOT worked!\n\r";

void test1b_cb(hcos_word_t arg);
hcos_word_t test1a_cb(void) {
    dma_memset(&DMAD1, buf1, 1, BUFSZ, test1b_cb);
    /* dma_memcpy(&DMAD1, buf1, buf2, 12, test1b_cb); */

    return 0;
}

void test1b_cb(hcos_word_t arg) {
    int i;
    int lim = BUFSZ;

    for (i=0; i<lim; i++)
    	if (buf1[i] != 1)
    	    break;
    if (i == lim)
    	uart_write(&SD1, msg1, sizeof(msg1)/sizeof(msg1[0]));
    else
	uart_write(&SD1, msg2, sizeof(msg2)/sizeof(msg2[0]));
    vt_add_non_rt_handler(test1a_cb, 250, 0);
}

int main(void) {
    uart_config_t cfg = {.baudrate = 3000000,
			 .word_length = 8,
			 .stop_bits = 1,
			 .parity = 0,
			 .over_sampling = 0,
			 .hw_flow_ctl = 0,
			 .rx_threshold = UART_BUFFER_SIZE/2, /* Threshold to call the rx callback */
			 .rx_complete_cb = 0,
			 .tx_complete_cb = 0,
			 .error_cb = 0
    };

    vt_add_non_rt_handler(blink_cb, 250, 1);
    vt_add_non_rt_handler(test1a_cb, 250, 0);

    uart_start(&SD1, &cfg);
    dma_start(&DMAD1);
    reactor_start();

    return 0;
}
