/*
 * This program illustrates a simple usage for the GPIO Hardware Abstraction Layer.
 * 
 * It implements a Continuity Test function (from a Multimeter) using as probes the
 * PA_1 pin and the GND pin. The led will be ON if the continuity is detected and off
 * otherwise.
 */

#include <stdint.h>
#include "virtual_timer.h"
#include "reactor.h"
#include "gpio.h"
#include "uart.h"
#include "cmsis_gcc.h"


hcos_base_int_t read_pin_cb(void) {
    /* If zero voltage is detected at probe (GND is connected), there is continuity */
    if (gpio_read_pin(GPIOA, 1) == 0) {
        /* Let's turn the led ON! */
        gpio_clear_pin(GPIOC, 13);
    } else {
        gpio_set_pin(GPIOC, 13);
    }
    
    return 0;
}

int main(void) {
    gpio_init();
    /* Configure probe pin as input (the second probe should be GND) */
    gpio_set_pin_mode(GPIOA, 1, generate_mode(IN_PULLUP, SPEED_50_MHZ));
    /* Configure led pin as output */
    gpio_set_pin_mode(GPIOC, 13, generate_mode(OUT_GP_PUSHPULL, SPEED_50_MHZ));
    /* Write default value to led (OFF) -> ON (low level on STM32F103 Bluepill) means continuity detected */
    gpio_set_pin(GPIOC, 13);

    /* In this example, we'll use polling to read the state of the probe every 10 ms */
    vt_add_non_rt_handler(read_pin_cb, 10, 1);

    reactor_start();

    return 0;
}
