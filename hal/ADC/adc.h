/*
*   This is a simple abstraction layer for ADC1 in STM32F103C8T6.
*/
#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include "stm32f103xb.h"

/* General macros */
#define _GPIO_CRL_CNF(n)        (unsigned long) (0x3UL << (2 + (n) * 4))
#define _GPIO_CRL_MODE(n)       (unsigned long) (0x3UL << ((n) * 4))

#define SET_REG_BITS_AT_MASK(REG, MASK, VAL, POS)   ( (REG) = (((REG) & ~(MASK)) | ((VAL) << (POS))) )

/* Enable ADC1 clock and performs calibration */
#define adc_init() \
    do {    \
        RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; \
        ADC1->CR2 |= ADC_CR2_ADON | ADC_CR2_CAL;    \
        while(ADC1->CR2 & ADC_CR2_CAL); \
    } while(0)

/* Macro to Configure ADC pins */
#define adc_set_gpio(n) \
    do {    \
        if (n <= 7) {   \
            RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; \
            GPIOA->CRL &= ~(_GPIO_CRL_CNF(n)) & ~(_GPIO_CRL_MODE(n)); \
        } else if (n <= 9) {    \
            RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; \
            GPIOB->CRL &= ~(_GPIO_CRH_CNF(n-8)) & ~(_GPIO_CRL_MODE(n-8)); \
        } else if (n <= 15) {   \
            RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; \
            GPIOC->CRL &= ~(_GPIO_CRL_CNF(n-10)) & ~(_GPIO_CRL_MODE(n-10)); \
        }   \
    } while(0)

/* ADC Conversion Configuration */
//TODO: implement DMA related configurations
#define ADC_CONV_RIGHT_ALIGN()      ( ADC1->CR2 &= ~(ADC_CR2_ALIGN) )
#define ADC_CONV_LEFT_ALIGN()       ( ADC1->CR2 |= ADC_CR2_ALIGN )

#define ADC_CONV_CONT_MODE()        ( ADC1->CR2 |= ADC_CR2_CONT )
#define ADC_CONV_SINGLE_MODE()      ( ADC1->CR2 &= ~(ADC_CR2_CONT) )

/* ADC Group Configuration */
#define ADC_GRP_NUM_OF_CONV(n)      SET_REG_BITS_AT_MASK(ADC1->SQR1, ADC_SQR1_L, (n-1), ADC_SQR1_L_Pos)
#define ADC_GRP_1ST_CONV_CHAN(n)    SET_REG_BITS_AT_MASK(ADC1->SQR3, ADC_SQR3_SQ1, (n), ADC_SQR3_SQ1_Pos)
#define ADC_GRP_2ND_CONV_CHAN(n)    SET_REG_BITS_AT_MASK(ADC1->SQR3, ADC_SQR3_SQ2, (n), ADC_SQR3_SQ2_Pos)
#define ADC_GRP_3RD_CONV_CHAN(n)    SET_REG_BITS_AT_MASK(ADC1->SQR3, ADC_SQR3_SQ3, (n), ADC_SQR3_SQ3_Pos)
#define ADC_GRP_4TH_CONV_CHAN(n)    SET_REG_BITS_AT_MASK(ADC1->SQR3, ADC_SQR3_SQ4, (n), ADC_SQR3_SQ4_Pos)
#define ADC_GRP_5TH_CONV_CHAN(n)    SET_REG_BITS_AT_MASK(ADC1->SQR3, ADC_SQR3_SQ5, (n), ADC_SQR3_SQ5_Pos)
#define ADC_GRP_6TH_CONV_CHAN(n)    SET_REG_BITS_AT_MASK(ADC1->SQR3, ADC_SQR3_SQ6, (n), ADC_SQR3_SQ6_Pos)
#define ADC_GRP_7TH_CONV_CHAN(n)    SET_REG_BITS_AT_MASK(ADC1->SQR2, ADC_SQR2_SQ7, (n), ADC_SQR2_SQ7_Pos)
#define ADC_GRP_8TH_CONV_CHAN(n)    SET_REG_BITS_AT_MASK(ADC1->SQR2, ADC_SQR2_SQ8, (n), ADC_SQR2_SQ8_Pos)
#define ADC_GRP_9TH_CONV_CHAN(n)    SET_REG_BITS_AT_MASK(ADC1->SQR2, ADC_SQR2_SQ9, (n), ADC_SQR2_SQ9_Pos)
#define ADC_GRP_10TH_CONV_CHAN(n)   SET_REG_BITS_AT_MASK(ADC1->SQR2, ADC_SQR2_SQ10, (n), ADC_SQR2_SQ10_Pos)
#define ADC_GRP_11TH_CONV_CHAN(n)   SET_REG_BITS_AT_MASK(ADC1->SQR2, ADC_SQR2_SQ11, (n), ADC_SQR2_SQ11_Pos)
#define ADC_GRP_12TH_CONV_CHAN(n)   SET_REG_BITS_AT_MASK(ADC1->SQR2, ADC_SQR2_SQ12, (n), ADC_SQR2_SQ12_Pos)
#define ADC_GRP_13TH_CONV_CHAN(n)   SET_REG_BITS_AT_MASK(ADC1->SQR1, ADC_SQR1_SQ13, (n), ADC_SQR1_SQ13_Pos)
#define ADC_GRP_14TH_CONV_CHAN(n)   SET_REG_BITS_AT_MASK(ADC1->SQR1, ADC_SQR1_SQ14, (n), ADC_SQR1_SQ14_Pos)
#define ADC_GRP_15TH_CONV_CHAN(n)   SET_REG_BITS_AT_MASK(ADC1->SQR1, ADC_SQR1_SQ15, (n), ADC_SQR1_SQ15_Pos)
#define ADC_GRP_16TH_CONV_CHAN(n)   SET_REG_BITS_AT_MASK(ADC1->SQR1, ADC_SQR1_SQ16, (n), ADC_SQR1_SQ16_Pos)

/* Conversion macros */
#define adc_enable_EOC_irq()    \
    do {    \
        __NVIC_EnableIRQ(ADC1_2_IRQn);  \
        ADC1->CR1 |= ADC_CR1_EOCIE; \
    } while(0);

#define ADC_EVT_EOC                 ( ADC1->SR & ADC_SR_EOC )
#define adc_disable_irq()           ( __NVIC_DisableIRQ(ADC1_2_IRQn) )
#define adc_read()                  ( ADC1->DR )


#endif