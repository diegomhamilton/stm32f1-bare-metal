/*
*   This is a simple abstraction layer for ADC1 in STM32F103C8T6.
*/
#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include "stm32f103xb.h"
#include "reactor.h"

#define ADC_MAX_CH_IN_GROUP                     16

typedef enum {
    ADC_RIGHT = 0,
    ADC_LEFT
} adc_config_align_t;

typedef enum {
    SINGLE = 0,
    CONTINUOUS
} adc_config_mode_t;

typedef enum {
    SR_1_5_CYCLES = 0,
    SR_7_5_CYCLES,
    SR_13_5_CYCLES,
    SR_28_5_CYCLES,
    SR_41_5_CYCLES,
    SR_55_5_CYCLES,
    SR_71_5_CYCLES,
    SR_239_5_CYCLES
} adc_config_sr_t;

#define ADC_DEFAULT_SR                          SR_239_5_CYCLES

typedef struct {
    uint8_t ch[ADC_MAX_CH_IN_GROUP];            /* Channels to be converted.
                                                 * Channel mapping
                                                 *    - from 0 to 7: PA_0 to PA_7
                                                 *    - 8 and 9: PB_0 and PB_1
                                                 *    - from 10 to 15: PC_0 to PC_5
                                                 */
    uint8_t len;                                /* Number of active channels in conversion group */
} adc_group_t;

//TODO: add external trigger configuration
typedef uint8_t adc_config_trigger_t;
typedef uint16_t adc_sample_t;

typedef struct {
    adc_group_t *group;                         /* ADC Group configuration */
    adc_config_sr_t sr[18];                     /* Sample Rate of each channel */
    adc_config_align_t align;                   /* Right or Left word alignment */
    adc_config_mode_t mode;                     /* Continuous or Single mode */
    adc_config_trigger_t trigger;               /* Set ADC Trigger mode */

    adc_sample_t *samples;                      /* Storage of ADC readings.
                                                 * Provide a buffer with a minimum size:
                                                 *  sizeof(adc_sample_t) * (conversion group's length)
                                                 */
    reactor_cb_t group_conv_cb;                 /* End of Group conversion callback */
} adc_config_t;

typedef struct {
    ADC_TypeDef *dev;                           /* ADC device */
    adc_config_t *config;                       /* ADC configuration */
} adc_t;

extern adc_t ADC1D;

#if 1
void adc_init(void);
#endif
void adc_config_group_pins(adc_group_t *group);
void adc_config_sample_rates(adc_config_sr_t *sr);
void adc_start(adc_t *drv, adc_config_t *config);
void adc_stop(adc_t *drv);
extern void ADC1_2_IRQHandler(void);

/* General macros */
#define _GPIO_CRL_CNF(n)        (unsigned long) (0x3UL << (2 + (n) * 4))
#define _GPIO_CRL_MODE(n)       (unsigned long) (0x3UL << ((n) * 4))

#define SET_REG_BITS_AT_MASK(REG, MASK, VAL, POS)   ( (REG) = (((REG) & ~(MASK)) | ((VAL) << (POS))) )

/* Macro to Configure ADC pins */
#define adc_set_gpio(n) \
    do {    \
        if (n <= 7) {   \
            RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; \
            GPIOA->CRL &= ~(_GPIO_CRL_CNF(n)) & ~(_GPIO_CRL_MODE(n)); \
        } else if (n <= 9) {    \
            RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; \
            GPIOB->CRL &= ~(_GPIO_CRL_CNF(n-8)) & ~(_GPIO_CRL_MODE(n-8)); \
        } else if (n <= 15) {   \
            RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; \
            GPIOC->CRL &= ~(_GPIO_CRL_CNF(n-10)) & ~(_GPIO_CRL_MODE(n-10)); \
        }   \
    } while(0)

/* ADC Conversion Configuration */
//TODO: implement DMA related configurations
#define ADC_CONV_RIGHT_ALIGN(ADC_DEV)           ( (ADC_DEV)->CR2 &= ~(ADC_CR2_ALIGN) )
#define ADC_CONV_LEFT_ALIGN(ADC_DEV)            ( (ADC_DEV)->CR2 |= ADC_CR2_ALIGN )

#define ADC_CONV_CONT_MODE(ADC_DEV)             ( (ADC_DEV)->CR2 |= ADC_CR2_CONT )
#define ADC_CONV_SINGLE_MODE(ADC_DEV)           ( (ADC_DEV)->CR2 &= ~(ADC_CR2_CONT) )

/* ADC Group Configuration */
#define ADC_GRP_NUM_OF_CONV(ADC_DEV, n)         SET_REG_BITS_AT_MASK((ADC_DEV)->SQR1, ADC_SQR1_L, (n-1), ADC_SQR1_L_Pos)
#define ADC_GRP_1ST_CONV_CHAN(ADC_DEV, n)       SET_REG_BITS_AT_MASK((ADC_DEV)->SQR3, ADC_SQR3_SQ1, (n), ADC_SQR3_SQ1_Pos)
#define ADC_GRP_2ND_CONV_CHAN(ADC_DEV, n)       SET_REG_BITS_AT_MASK((ADC_DEV)->SQR3, ADC_SQR3_SQ2, (n), ADC_SQR3_SQ2_Pos)
#define ADC_GRP_3RD_CONV_CHAN(ADC_DEV, n)       SET_REG_BITS_AT_MASK((ADC_DEV)->SQR3, ADC_SQR3_SQ3, (n), ADC_SQR3_SQ3_Pos)
#define ADC_GRP_4TH_CONV_CHAN(ADC_DEV, n)       SET_REG_BITS_AT_MASK((ADC_DEV)->SQR3, ADC_SQR3_SQ4, (n), ADC_SQR3_SQ4_Pos)
#define ADC_GRP_5TH_CONV_CHAN(ADC_DEV, n)       SET_REG_BITS_AT_MASK((ADC_DEV)->SQR3, ADC_SQR3_SQ5, (n), ADC_SQR3_SQ5_Pos)
#define ADC_GRP_6TH_CONV_CHAN(ADC_DEV, n)       SET_REG_BITS_AT_MASK((ADC_DEV)->SQR3, ADC_SQR3_SQ6, (n), ADC_SQR3_SQ6_Pos)
#define ADC_GRP_7TH_CONV_CHAN(ADC_DEV, n)       SET_REG_BITS_AT_MASK((ADC_DEV)->SQR2, ADC_SQR2_SQ7, (n), ADC_SQR2_SQ7_Pos)
#define ADC_GRP_8TH_CONV_CHAN(ADC_DEV, n)       SET_REG_BITS_AT_MASK((ADC_DEV)->SQR2, ADC_SQR2_SQ8, (n), ADC_SQR2_SQ8_Pos)
#define ADC_GRP_9TH_CONV_CHAN(ADC_DEV, n)       SET_REG_BITS_AT_MASK((ADC_DEV)->SQR2, ADC_SQR2_SQ9, (n), ADC_SQR2_SQ9_Pos)
#define ADC_GRP_10TH_CONV_CHAN(ADC_DEV, n)      SET_REG_BITS_AT_MASK((ADC_DEV)->SQR2, ADC_SQR2_SQ10, (n), ADC_SQR2_SQ10_Pos)
#define ADC_GRP_11TH_CONV_CHAN(ADC_DEV, n)      SET_REG_BITS_AT_MASK((ADC_DEV)->SQR2, ADC_SQR2_SQ11, (n), ADC_SQR2_SQ11_Pos)
#define ADC_GRP_12TH_CONV_CHAN(ADC_DEV, n)      SET_REG_BITS_AT_MASK((ADC_DEV)->SQR2, ADC_SQR2_SQ12, (n), ADC_SQR2_SQ12_Pos)
#define ADC_GRP_13TH_CONV_CHAN(ADC_DEV, n)      SET_REG_BITS_AT_MASK((ADC_DEV)->SQR1, ADC_SQR1_SQ13, (n), ADC_SQR1_SQ13_Pos)
#define ADC_GRP_14TH_CONV_CHAN(ADC_DEV, n)      SET_REG_BITS_AT_MASK((ADC_DEV)->SQR1, ADC_SQR1_SQ14, (n), ADC_SQR1_SQ14_Pos)
#define ADC_GRP_15TH_CONV_CHAN(ADC_DEV, n)      SET_REG_BITS_AT_MASK((ADC_DEV)->SQR1, ADC_SQR1_SQ15, (n), ADC_SQR1_SQ15_Pos)
#define ADC_GRP_16TH_CONV_CHAN(ADC_DEV, n)      SET_REG_BITS_AT_MASK((ADC_DEV)->SQR1, ADC_SQR1_SQ16, (n), ADC_SQR1_SQ16_Pos)

/* Conversion macros */
#define adc_enable_EOC_irq(ADC_DEV)    \
    do {    \
        __NVIC_EnableIRQ(ADC1_2_IRQn);  \
        (ADC_DEV)->CR1 |= ADC_CR1_EOCIE; \
    } while(0);

#define ADC_EVT_EOC(ADC_DEV)                    ( (ADC_DEV)->SR & ADC_SR_EOC )
#define adc_disable_irq()                       ( __NVIC_DisableIRQ(ADC1_2_IRQn) )
#define adc_read(ADC_DEV)                       ( (ADC_DEV)->DR )


#endif