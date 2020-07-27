#include <stdint.h>
#include "stm32f103xb.h"
#include "adc.h"
#include "rcc.h"
#include "reactor.h"

adc_t ADC1D;
static adc_t *active_drv;

#if 1
void adc_init(void) {
    ADC1D.dev = ADC1;
    ADC1D.config = 0;

    RCC_ADC1_CLK_ENABLE();                      /* Enable ADC1 RCC */
    ADC1->CR2 |= ADC_CR2_ADON;
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();   /* Wait two ADC clock cycles before starting calibration */
    ADC1->CR2 |= ADC_CR2_CAL;                   /* Start calibration */
    
    while(ADC1->CR2 & ADC_CR2_CAL);             /* Wait for ADC1 calibration to finish */
    adc_stop(&ADC1D);
}
#endif

void adc_start(adc_t *drv, adc_config_t *config) {
    drv->config = config;

    adc_config_group_pins(drv->config->group);
    adc_config_sample_rates(drv->config->sr);
    
    (drv->config->align == ADC_RIGHT) ? ADC_CONV_RIGHT_ALIGN(ADC1) : ADC_CONV_LEFT_ALIGN(ADC1);
    ADC_CONV_SINGLE_MODE(ADC1);     //TODO: add support to cont mode
    drv->config->trigger = 0;       //TODO: add support to trigger config

    adc_enable_EOC_irq(ADC1);
    drv->dev->CR2 |= ADC_CR2_ADON;
    active_drv = drv;
}

void adc_stop(adc_t *drv) {
    // drv->dev->CR2 &= ~(ADC_CR2_ADON);   // Stop ADC
    drv->config = 0;                    // Reset configs
}

void adc_config_group_pins(adc_group_t *group) {
    ADC_GRP_NUM_OF_CONV(ADC1, group->len);
    if (group->len >= 1) {
        adc_set_gpio(group->ch[0]);
        ADC_GRP_1ST_CONV_CHAN(ADC1, group->ch[0]);
    }
    if (group->len >= 2) {
        adc_set_gpio(group->ch[1]);
        ADC_GRP_2ND_CONV_CHAN(ADC1, group->ch[1]);
    }
    if (group->len >= 3) {
        adc_set_gpio(group->ch[2]);
        ADC_GRP_3RD_CONV_CHAN(ADC1, group->ch[2]);
    }
    if (group->len >= 4) {
        adc_set_gpio(group->ch[3]);
        ADC_GRP_4TH_CONV_CHAN(ADC1, group->ch[3]);
    }
    if (group->len >= 5) {
        adc_set_gpio(group->ch[4]);
        ADC_GRP_5TH_CONV_CHAN(ADC1, group->ch[4]);
    }
    if (group->len >= 6) {
        adc_set_gpio(group->ch[5]);
        ADC_GRP_6TH_CONV_CHAN(ADC1, group->ch[5]);
    }
    if (group->len >= 7) {
        adc_set_gpio(group->ch[6]);
        ADC_GRP_7TH_CONV_CHAN(ADC1, group->ch[6]);
    }
    if (group->len >= 8) {
        adc_set_gpio(group->ch[7]);
        ADC_GRP_8TH_CONV_CHAN(ADC1, group->ch[7]);
    }
    if (group->len >= 9) {
        adc_set_gpio(group->ch[8]);
        ADC_GRP_9TH_CONV_CHAN(ADC1, group->ch[8]);
    }
    if (group->len >= 10) {
        adc_set_gpio(group->ch[9]);
        ADC_GRP_10TH_CONV_CHAN(ADC1, group->ch[9]);
    }
    if (group->len >= 11) {
        adc_set_gpio(group->ch[10]);
        ADC_GRP_11TH_CONV_CHAN(ADC1, group->ch[10]);
    }
    if (group->len >= 12) {
        adc_set_gpio(group->ch[11]);
        ADC_GRP_12TH_CONV_CHAN(ADC1, group->ch[11]);
    }
    if (group->len >= 13) {
        adc_set_gpio(group->ch[12]);
        ADC_GRP_13TH_CONV_CHAN(ADC1, group->ch[12]);
    }
    if (group->len >= 14) {
        adc_set_gpio(group->ch[13]);
        ADC_GRP_14TH_CONV_CHAN(ADC1, group->ch[13]);
    }
    if (group->len >= 15) {
        adc_set_gpio(group->ch[14]);
        ADC_GRP_15TH_CONV_CHAN(ADC1, group->ch[14]);
    }
    if (group->len >= 16) {
        adc_set_gpio(group->ch[15]);
        ADC_GRP_16TH_CONV_CHAN(ADC1, group->ch[15]);
    }
}

void adc_config_sample_rates(adc_config_sr_t *sr) {
    //TODO: configure sample rates
    /*
    *   Use same logic as adc_config_group_pins
    */
}

#if 1
void ADC1_2_IRQHandler(void) {
    static uint8_t conv_count = 0;

    if(ADC_EVT_EOC(ADC1)) {
        (active_drv->config->samples)[conv_count] = adc_read(ADC1);
        conv_count += 1;

        if (conv_count == active_drv->config->group->len) {
            /* Stop conversion if finished group's conversion
             * and no external trigger is set.
             */
            conv_count = 0;
            if (active_drv->config->group_conv_cb != 0) {
                reactor_add_handler(active_drv->config->group_conv_cb, (hcos_word_t) active_drv);
            }
        } else {
            /* Start group's next conversion if external trigger is not set */
            if (active_drv->config->trigger == 0) active_drv->dev->CR2 |= ADC_CR2_ADON;
        }
    }
    //TODO: Handle errors
}
#endif