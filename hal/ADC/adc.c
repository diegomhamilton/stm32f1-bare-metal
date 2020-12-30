#include <stdint.h>
#include "stm32f103xb.h"
#include "adc.h"
#include "rcc.h"
#include "reactor.h"
#include "dma.h"

adc_t ADCD1;

static void _adc_config_sample_rates(adc_t *drv, adc_config_sr_t *sr);
static void _adc_config_group_pins(adc_t *drv, adc_group_t *group);

void adc_init(void) {
    ADCD1.dev = ADC1;

#if ADC_USE_DMA == 1
    ADCD1.dma_channel = DMA_ADC1_CHANNEL;
#endif /* ADC_USE_DMA */
}

int adc_start(adc_t *drv, adc_config_t *config) {
#if ADC_USE_DMA != 1
    /* If we are scanning more than one channel, we must use DMA */
    if (config->group->len > 1)
        return -1;
#endif /* ADC_USE_DMA */

    (config->align == ADC_RIGHT) ? ADC_CONV_RIGHT_ALIGN(ADC1) : ADC_CONV_LEFT_ALIGN(ADC1);
    /* drv->trigger = 0;       //TODO: add support to trigger config */

    drv->group_conv_cb = config->group_conv_cb;
    drv->eoc_injected_cb = config->eoc_injected_cb;
    drv->watchdog_cb = config->watchdog_cb;
#if ADC_USE_DMA == 1
    drv->half_transfer_cb = config->half_transfer_cb;
    drv->full_transfer_cb = config->full_transfer_cb;
#endif

    drv->samples = 0;

    RCC_ADC1_CLK_ENABLE();                      /* Enable ADC1 RCC */
    drv->dev->CR2 |= ADC_CR2_ADON;
    /* Wait two ADC clock cycles before starting calibration. Assumes
       12 MHz ADC clock */
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();

    drv->dev->CR2 |= ADC_CR2_CAL;
    while(drv->dev->CR2 & ADC_CR2_CAL)
	;             /* Wait for ADC1 calibration to finish */

    /* Configure group pins and conversion channels */
    _adc_config_group_pins(drv, config->group);
    _adc_config_sample_rates(drv, config->sr);
    
    drv->dev->CR2 |= (config->mode == SINGLE ? 0 : ADC_CR2_CONT);

#if ADC_USE_DMA == 1
    dma_start(&DMAD1);
#endif /* ADC_USE_DMA */

    return 0;
}

#if ADC_USE_DMA == 1
void adc_full_transfer_cb(hcos_word_t arg) {
    
}
#endif

int adc_start_conversion(adc_t* drv, uint16_t* buf, uint16_t n) {
#if ADC_USE_DMA == 1
    dma_bind_config_t config = {.peripheral_address = (uint32_t) &drv->dev->DR,
				.memory_address = (uint32_t) buf,
				.nbr_transfers = n,
				.mem2mem_flag = 0,
				.priority = DMA_PRIO_VERY_HIGH,
				.peripheral_size = DMA_PER_SIZE_16BITS,
				.memory_size = DMA_MEM_SIZE_16BITS,
				.peripheral_increment = DMA_NO_PER_INCREMENT_MODE,
				.memory_increment = DMA_MEM_INCREMENT_MODE,
				.direction = DMA_FROM_PER_MODE,
				.circular_mode = DMA_CIRCULAR_MODE,
				.half_transfer_cb = drv->half_transfer_cb,
				.full_transfer_cb = drv->full_transfer_cb,
				.error_cb = 0,
				.half_transfer_args = 0,
				.full_transfer_args = 0,
				.error_args = 0,
				.half_transfer_rt_cb = 0,
				.full_transfer_rt_cb = 0,
				.error_rt_cb = 0,
				.half_transfer_rt_args = (hcos_word_t) drv,
				.full_transfer_rt_args = (hcos_word_t) drv,
				.error_rt_args = 0,
    };

    dma_bind(&DMAD1, drv->dma_channel, config);
    drv->dev->CR2 |= ADC_CR2_DMA;
    dma_enable(&DMAD1, drv->dma_channel);
    drv->length = 0;
#else  /* ADC_USE_DMA == FALSE */
    drv->samples = buf;
    drv->length = n;
    adc_enable_EOC_irq(ADC1);
#endif  /* ADC_USE_DMA */

    drv->dev->CR2 |= ADC_CR2_ADON;

    return 0;
}

int adc_stop_conversion(adc_t *drv) {
    drv->dev->CR2 &= ~(ADC_CR2_CONT);   // Stop ADC
    drv->dev->CR1 &= ~ADC_CR1_EOCIE;

    /* TODO:  Check the best value to return */
    /* return drv->dev->SR & ? 1 : 0; */
    return 0;
}

int adc_stop(adc_t *drv) {
    adc_stop_conversion(drv);
    drv->dev->CR2 &= ~(ADC_CR2_ADON);   // Stop ADC
    RCC_ADC1_CLK_DISABLE();             /* Enable ADC1 RCC */

    return 0;
}

static void _adc_config_sample_rate(volatile uint32_t *reg, int n, adc_config_sr_t sr) {
    uint32_t mask;
    n *= 3;
    mask = 0x07 << n;
    *reg = (*reg & ~mask) | ((sr << n) & mask);
}

void adc_config_sample_rate(adc_t *drv, int n, adc_config_sr_t sr) {
    if (n < 10)
        _adc_config_sample_rate(&drv->dev->SMPR1, n, sr);
    else
        _adc_config_sample_rate(&drv->dev->SMPR2, n - 10, sr);
}

static void _adc_config_sample_rates(adc_t *drv, adc_config_sr_t *sr) {
    int i;

    for (i=0; i<10; i++)
        _adc_config_sample_rate(&drv->dev->SMPR1, i, sr[i]);

    for (; i<NBR_ADC_CHANNELS; i++)
        _adc_config_sample_rate(&drv->dev->SMPR2, i - 10, sr[i]);
}

static void _adc_config_group_pins(adc_t *drv, adc_group_t *group) {
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

#if 0
void ADC1_2_IRQHandler(void) {
    static uint8_t conv_count = 0;

    if(ADC_EVT_EOC(ADC1)) {
        ADCD1.samples[conv_count] = adc_read(ADC1);
        conv_count += 1;

        if (conv_count >= active_drv->config->group->len) {
            /* Stop conversion if finished group's conversion
             * and no external trigger is set.
             */
            conv_count = 0;
            if (ADCD1.group_conv_cb != 0) {
                reactor_add_handler(ADCD1.group_conv_cb, (hcos_word_t) &ADCD1);
            }
        } else {
            /* Start group's next conversion if external trigger is not set */
            /* Need to see if the line below is really needed */
            /* if (active_drv->trigger == 0) active_drv->dev->CR2 |= ADC_CR2_ADON; */
        }
    }
    //TODO: Handle errors
}
#endif
