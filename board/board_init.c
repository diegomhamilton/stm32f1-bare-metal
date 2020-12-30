#include "stm32f103xb.h"
#include "gpio.h"
#include "rcc.h"


void
board_init(void) {
    /* Enable PortC clock */
    RCC_GPIOC_CLK_ENABLE();

    /* Set bit 13 of the GPIOC port as output at 10 MHz (default) */
    gpio_set_pin_mode(IOPORT3, 13, OUT_GP_PUSHPULL);
}
