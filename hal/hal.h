#ifndef HAL_H
#define HAL_H

#include "gpio.h"
#include "uart.h"

#define hal_init() do { \
        gpio_init(); \
    } while(0)

#endif /* HAL_H */
