#include "stm32f103xb.h"
#include "adc.h"

uint8_t adc_conv_ready = 0;
uint16_t conversion_result = 0;

void ADC1_2_IRQHandler(void) {
	if (ADC1->SR & ADC_SR_EOC) {
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
	ADC1->CR2 &= ~(ADC_CR2_CONT) & ~(ADC_CR2_ALIGN);	// Disable Continuous mode and set right alignment
	ADC1->SMPR2 &= ~(ADC_SMPR2_SMP4);					// Set 1.5 cycle of sample time (SMP0 = 000)
	ADC1->SQR1 &= ~(ADC_SQR1_L);						// Only 1 conversion set.
	ADC1->SQR3 &= ~(ADC_SQR3_SQ1);						// Channel 0 set as first conversion
	ADC1->SQR3 |= 0x4UL << ADC_SQR3_SQ1_Pos;						// Channel 0 set as first conversion

	/* Enable EOC interrupt */
	__NVIC_EnableIRQ(ADC1_2_IRQn);
	ADC1->CR1 |= ADC_CR1_EOCIE;

	while(1) {
		ADC1->CR2 |= ADC_CR2_ADON;
		while(adc_conv_ready == 0);
		adc_conv_ready = 0;

		if (conversion_result > 2048) {
			GPIOC->BSRR |= GPIO_BSRR_BR13;
		} else {
			GPIOC->BSRR |= GPIO_BRR_BR13;
		}
	}
}
