/*
*   This is a simple abstraction layer for ADC1 in STM32F103C8T6.
*/
#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include "stm32f103xb.h"

/* Enable ADC1 clock and performs calibration */
#define adc_init() \
    do {    \
        RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; \
        ADC1->CR2 |= ADC_CR2_ADON | ADC_CR2_CAL;    \
        while(ADC1->CR2 & ADC_CR2_CAL); \
    } while(0)

/* Macro to Configure MODE and CNF */
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

typedef struct {

} adc_group_t;

#define adc_read()              ADC1->DR

#define _GPIO_CRL_CNF(n)     (unsigned long) (0x3UL << (2 + (n) * 4))
#define _GPIO_CRL_MODE(n)    (unsigned long) (0x3UL << ((n) * 4))

#endif