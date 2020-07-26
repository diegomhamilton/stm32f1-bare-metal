#ifndef HAL_H
#define HAL_H

#include "gpio.h"
#include "uart.h"
#include "dma.h"

#define hal_init() do {				\
        gpio_init();				\
	uart_init();				\
	dma_init();				\
    } while(0)

#endif /* HAL_H */
