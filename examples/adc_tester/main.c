#include <stdint.h>
#include "cmsis_gcc.h"
#include "virtual_timer.h"
#include "reactor.h"
#include "gpio.h"
#include "uart.h"
#include "dma.h"
#include "adc.h"


/*
 * Callback to blink the led to show the app is active
 */
uint32_t led_state = 1 << 29;

hcos_base_int_t blink_cb(void) {
    IOPORT3->BSRR = led_state;
    led_state = __ROR(led_state, 16);
    return 0;
}

#define ADC_BUFFER_SIZE 1024
uint16_t adc_buffer[ADC_BUFFER_SIZE];

void full_transfer_cb(hcos_word_t arg) {
    uart_write_dma(&SD1, (uint8_t *) &adc_buffer[ADC_BUFFER_SIZE/2], ADC_BUFFER_SIZE, 0);
}

void half_transfer_cb(hcos_word_t arg) {
    uart_write_dma(&SD1, (uint8_t *) adc_buffer, ADC_BUFFER_SIZE, 0);
}

int main(void) {
    uart_config_t uart_cfg = {.baudrate = 3000000,
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

    adc_group_t adc_groups_config = {.ch = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				     .len = 1,
    };
    adc_config_t adc_cfg = {.group = &adc_groups_config,
			    .sr = {SR_239_5_CYCLES, SR_239_5_CYCLES, SR_239_5_CYCLES, SR_239_5_CYCLES,
				   SR_239_5_CYCLES, SR_239_5_CYCLES, SR_239_5_CYCLES, SR_239_5_CYCLES,
				   SR_239_5_CYCLES, SR_239_5_CYCLES, SR_239_5_CYCLES, SR_239_5_CYCLES,
				   SR_239_5_CYCLES, SR_239_5_CYCLES, SR_239_5_CYCLES, SR_239_5_CYCLES,
				   SR_239_5_CYCLES, SR_239_5_CYCLES},
			    .align = 0,
			    .mode = 1,
			    .trigger = 0,
			    .group_conv_cb = 0,
			    .eoc_injected_cb = 0,
			    .watchdog_cb = 0,
			    .half_transfer_cb = half_transfer_cb,
			    .full_transfer_cb = full_transfer_cb,
    };

    /* TODO: For now, we set the pin mode here.  Do it in adc_start() */
    gpio_set_pin_mode(IOPORT1, 0, IN_ANALOG_MODE);

    vt_add_non_rt_handler(blink_cb, 250, 1);

    uart_start(&SD1, &uart_cfg);
    adc_start(&ADCD1, &adc_cfg);

    adc_start_conversion(&ADCD1, adc_buffer, ADC_BUFFER_SIZE);

    reactor_start();

    return 0;
}
