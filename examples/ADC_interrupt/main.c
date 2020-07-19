#include "stm32f103xb.h"
#include "adc.h"

volatile uint8_t adc_conv_ready = 0;
volatile uint16_t conversion_result = 0;

void ADC1_2_IRQHandler(void) {
	if (ADC_EVT_EOC) {
		adc_conv_ready = 1;
		conversion_result = adc_read();
	}
}

int
main(void) {
	adc_init();

	/* GPIO settings */
	adc_set_gpio(4);
	
	/* Channel 0 Single Conversion */
	ADC_CONV_SINGLE_MODE();
	ADC_CONV_RIGHT_ALIGN();
	// ADC1->SMPR2 &= ~(ADC_SMPR2_SMP0);					// Set 1.5 cycle of sample time (SMP0 = 000)
	ADC_GRP_NUM_OF_CONV(1);
	ADC_GRP_1ST_CONV_CHAN(4);

	/* Enable EOC interrupt */
	adc_enable_EOC_irq();
	GPIOC->BSRR = GPIO_BSRR_BS13;

	while(1) {
		ADC1->CR2 |= ADC_CR2_ADON;
		while(adc_conv_ready == 0);
		adc_conv_ready = 0;

		if (conversion_result > 2048) {
			GPIOC->BRR = GPIO_BRR_BR13;
		} else {
			GPIOC->BSRR = GPIO_BSRR_BS13;
		}
	}
}
