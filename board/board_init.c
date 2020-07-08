#include "stm32f103xb.h"
#include "gpio.h"


void
board_init(void) {
    /* Enable PortC clock */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    /* Set bit 13 of the GPIOC port as output at 50 MHz */
    gpio_set_pin_mode(IOPORT3, 13, OUT_GP_PUSHPULL | SPEED_50_MHZ);
}
