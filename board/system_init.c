#include <stm32f103xb.h>
#include <core_cm3.h>
#include <cmsis_gcc.h>

#include "reactor.h"
#include "virtual_timer.h"
#include "hal.h"

void
system_init(void) {
    /* Enable Prefetch buffer and set flash latency to 2 wait states */
    FLASH->ACR = FLASH_ACR_LATENCY_1 | FLASH_ACR_PRFTBE;

    /* Turn on HSE clock */
    RCC->CR |= RCC_CR_HSEON;
    while(!(RCC->CR & RCC_CR_HSERDY))
	;

    /* Set the APB1 prescaler to HCLK/2 and configure the PLL to use */
    /* multiplier 9 and HSE as source. ADC Clk is divided by 6 */
    RCC->CFGR = RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_PLLMULL9 | RCC_CFGR_PLLSRC |
	RCC_CFGR_ADCPRE_DIV6;

    /* Turn on the PLL */
    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY))
	;

    /* Make the PLL the clock source */
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Turn off the HSI */
    RCC->CR &= ~(RCC_CR_HSION);

    /* Set the tick handler to aproximately 1 tick per ms */
    SysTick_Config(72000);

    /* Initiates the reactor scheduler and the virtual timers*/
    reactor_init();
    vt_init();
    hal_init();

    __enable_irq();
}
