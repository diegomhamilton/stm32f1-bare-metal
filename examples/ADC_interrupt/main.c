#include "stm32f103xb.h"
#include "adc.h"
#include "reactor.h"
#include "virtual_timer.h"


void adc_conv_callback(hcos_word_t arg) {
	adc_t *drv = (adc_t *) arg;

	if (drv->config->samples[0] < 1024) {
		GPIOC->BSRR = GPIO_BSRR_BS13;
	} else {
		GPIOC->BRR = GPIO_BRR_BR13;
	}
}

adc_group_t adc_group = {
	.ch = {0, 1, 2, 3, 4, 0, 0, 0, 0, 1, 2, 3, 4, 0, 0, 0},
	.len = 5
};

adc_sample_t conv_result[5] = {0, 0, 0, 0, 0};

adc_config_t adc_config = {
	.group = &adc_group,
	.sr = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	.align = ADC_RIGHT,
	.mode = SINGLE,
	.trigger = 0,
	.group_conv_cb = adc_conv_callback,
	.samples = conv_result
};

timer_cb_t adc_start_conv_cb(void) {
	adc_start(&ADC1D, &adc_config);
	GPIOC->BSRR = GPIO_BSRR_BS13;

	return 0;
}

int
main(void) {
	GPIOC->BRR = GPIO_BRR_BR13;

	vt_add_non_rt_handler((timer_cb_t) adc_start_conv_cb, 250, 1);
	reactor_start();
}
