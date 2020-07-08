#include <stdint.h>
#include <stm32f103xb.h>


uint32_t led_state;

void
user_init(void)
{
	/* Set bit 13 of the GPIOC port as output at 50 MHz */
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIOC->CRH = GPIO_CRH_CNF8_0 | GPIO_CRH_CNF9_0 | GPIO_CRH_CNF10_0 | GPIO_CRH_CNF11_0 |
		GPIO_CRH_CNF12_0 | GPIO_CRH_CNF14_0 | GPIO_CRH_CNF15_0 |
		GPIO_CRH_MODE13_0 | GPIO_CRH_MODE13_1;

	/* Start the blinking of the built-in led */
	led_state = GPIO_BSRR_BR13;
	GPIOC->BSRR = led_state;
}
