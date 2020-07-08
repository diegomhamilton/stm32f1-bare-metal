#include <stdint.h>
#include "gpio.h" 


void gpio_init(void) {
    /* Set the clock to all ports (A to E), including the AF clock. */
    /* TODO: Decide later whether optimize power consumption and allow
       user to selectively turn the clock on */
    RCC->APB2ENR |= 0x7D;
}

uint64_t gpio_replicate_mode_for_mask(uint8_t mode, uint16_t bitmask) {
    int i;
    uint64_t all_modes = 0;
    for(i=0; i<16; i++) { /* 16 pins per port */
	all_modes <<= 4; /* Each mode is 4 bits wide */
	if (bitmask & 0x8000)
	    all_modes |= mode;
	bitmask <<= 1;
    }
    return all_modes;
}
