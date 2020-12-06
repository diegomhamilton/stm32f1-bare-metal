#include <stdint.h>
#include "cmsis_gcc.h"
#include "virtual_timer.h"
#include "reactor.h"
#include "gpio.h"
#include "adc.h"

#define ADC_BUFFER_SIZE 1
uint16_t adc_buffer[ADC_BUFFER_SIZE];

#define PIN3	(1 << 3)
#define PIN4	(1 << 4)

reactor_cb_t voltage_to_display(void) {
	adc_sample_t measurement = adc_buffer[0]
	if (measurement < 256) {
		gpio_clear_pin(GPIOB, 3);
		gpio_clear_pin(GPIOB, 4);
	} else if (measurement < 512) {
		gpio_set_pin(GPIOB, 3);
		gpio_clear_pin(GPIOB, 4);
	} else if (measurement < 768) {
		gpio_clear_pin(GPIOB, 3);
		gpio_set_pin(GPIOB, 4);
	} else {
		gpio_set_pin(GPIOB, 3);
		gpio_set_pin(GPIOB, 4);
	}
	
	adc_start_conversion(&ADCD1, adc_buffer, ADC_BUFFER_SIZE);
}

static adc_group_t adc_groups_config = {.ch = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				     .len = 1,
    };

static adc_config_t adc_cfg = {.group = &adc_groups_config,
			.sr = {SR_239_5_CYCLES, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			.align = ADC_RIGHT,
			.mode = SINGLE,
			.trigger = 0,
			.samples = adc_buffer,
			.group_conv_cb = voltage_to_display,
			.eoc_injected_cb = 0,
			.watchdog_cb = 0,
};


int main(void) {
	gpio_init();
    /* Configure port B as output push-pull */
    gpio_set_port_mode(GPIOB, gpio_replicate_mode_for_port(generate_mode(OUT_GP_PUSHPULL, SPEED_50_MHZ)));

    adc_start(&ADCD1, &adc_cfg);
    adc_start_conversion(&ADCD1, adc_buffer, ADC_BUFFER_SIZE);

    reactor_start();

    return 0;
}
