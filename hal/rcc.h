#ifndef RCC_H
#define RCC_H

#define RCC_AFIO_CLK_ENABLE()   do {				\
	__IO uint32_t tmpreg;					\
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;			\
	/* Delay after an RCC peripheral clock enabling */	\
	tmpreg = RCC->APB2ENR & RCC_APB2ENR_AFIOEN;		\
	(void)tmpreg;						\
    } while(0U)

#define RCC_GPIOA_CLK_ENABLE()   do {				\
	__IO uint32_t tmpreg;					\
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;			\
	/* Delay after an RCC peripheral clock enabling */	\
	tmpreg = RCC->APB2ENR & RCC_APB2ENR_IOPAEN;		\
	(void)tmpreg;						\
    } while(0U)

#define RCC_GPIOB_CLK_ENABLE()   do {				\
	__IO uint32_t tmpreg;					\
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;			\
	/* Delay after an RCC peripheral clock enabling */	\
	tmpreg = RCC->APB2ENR & RCC_APB2ENR_IOPBEN;		\
	(void)tmpreg;						\
    } while(0U)

#define RCC_GPIOC_CLK_ENABLE()   do {				\
	__IO uint32_t tmpreg;					\
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;			\
	/* Delay after an RCC peripheral clock enabling */	\
	tmpreg = RCC->APB2ENR & RCC_APB2ENR_IOPCEN;		\
	(void)tmpreg;						\
    } while(0U)

#define RCC_GPIOD_CLK_ENABLE()   do {				\
	__IO uint32_t tmpreg;					\
	RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;			\
	/* Delay after an RCC peripheral clock enabling */	\
	tmpreg = RCC->APB2ENR & RCC_APB2ENR_IOPDEN;		\
	(void)tmpreg;						\
    } while(0U)

#define RCC_ADC1_CLK_ENABLE()   do {				\
	__IO uint32_t tmpreg;					\
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;			\
	/* Delay after an RCC peripheral clock enabling */	\
	tmpreg = RCC->APB2ENR & RCC_APB2ENR_ADC1EN;		\
	(void)tmpreg;						\
    } while(0U)

#define RCC_TIM1_CLK_ENABLE()   do {				\
	__IO uint32_t tmpreg;					\
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;			\
	/* Delay after an RCC peripheral clock enabling */	\
	tmpreg = RCC->APB2ENR & RCC_APB2ENR_TIM1EN;		\
	(void)tmpreg;						\
    } while(0U)

#define RCC_SPI1_CLK_ENABLE()   do {				\
	__IO uint32_t tmpreg;					\
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;			\
	/* Delay after an RCC peripheral clock enabling */	\
	tmpreg = RCC->APB2ENR & RCC_APB2ENR_SPI1EN;		\
	(void)tmpreg;						\
    } while(0U)

#define RCC_USART1_CLK_ENABLE()   do {				\
	__IO uint32_t tmpreg;					\
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;			\
	/* Delay after an RCC peripheral clock enabling */	\
	tmpreg = RCC->APB2ENR & RCC_APB2ENR_USART1EN;		\
	(void)tmpreg;						\
    } while(0U)

#define RCC_TIM2_CLK_ENABLE()   do {				\
	__IO uint32_t tmpreg;					\
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;			\
	/* Delay after an RCC peripheral clock enabling */	\
	tmpreg = RCC->APB1ENR & RCC_APB1ENR_TIM2EN;		\
	(void)tmpreg;						\
    } while(0U)

#define RCC_TIM3_CLK_ENABLE()   do {				\
	__IO uint32_t tmpreg;					\
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;			\
	/* Delay after an RCC peripheral clock enabling */	\
	tmpreg = RCC->APB1ENR & RCC_APB1ENR_TIM3EN;		\
	(void)tmpreg;						\
    } while(0U)

#define RCC_WWDG_CLK_ENABLE()   do {				\
	__IO uint32_t tmpreg;					\
	RCC->APB1ENR |= RCC_APB1ENR_WWDGEN;			\
	/* Delay after an RCC peripheral clock enabling */	\
	tmpreg = RCC->APB1ENR & RCC_APB1ENR_WWDGEN;		\
	(void)tmpreg;						\
    } while(0U)

#define RCC_USART2_CLK_ENABLE()   do {				\
	__IO uint32_t tmpreg;					\
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;			\
	/* Delay after an RCC peripheral clock enabling */	\
	tmpreg = RCC->APB1ENR & RCC_APB1ENR_USART2EN;		\
	(void)tmpreg;						\
    } while(0U)

#define RCC_I2C1_CLK_ENABLE()   do {				\
	__IO uint32_t tmpreg;					\
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;			\
	/* Delay after an RCC peripheral clock enabling */	\
	tmpreg = RCC->APB1ENR & RCC_APB1ENR_I2C1EN;		\
	(void)tmpreg;						\
    } while(0U)

#define RCC_BKP_CLK_ENABLE()   do {				\
	__IO uint32_t tmpreg;					\
	RCC->APB1ENR |= RCC_APB1ENR_BKPEN;			\
	/* Delay after an RCC peripheral clock enabling */	\
	tmpreg = RCC->APB1ENR & RCC_APB1ENR_BKPEN;		\
	(void)tmpreg;						\
    } while(0U)

#define RCC_PWR_CLK_ENABLE()   do {				\
	__IO uint32_t tmpreg;					\
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;			\
	/* Delay after an RCC peripheral clock enabling */	\
	tmpreg = RCC->APB1ENR & RCC_APB1ENR_PWREN;		\
	(void)tmpreg;						\
    } while(0U)

#define RCC_TIM4_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = RCC->APB1ENR & RCC_APB1ENR_TIM4EN;\
                                        (void)tmpreg; \
                                      } while(0U)

#define RCC_SPI2_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = RCC->APB1ENR & RCC_APB1ENR_SPI2EN;\
                                        (void)tmpreg; \
                                      } while(0U)

#define RCC_USART3_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        RCC->APB1ENR |= RCC_APB1ENR_USART3EN;\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = RCC->APB1ENR & RCC_APB1ENR_USART3EN;\
                                        (void)tmpreg; \
                                      } while(0U)

#define RCC_I2C2_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = RCC->APB1ENR & RCC_APB1ENR_I2C2EN;\
                                        (void)tmpreg; \
                                      } while(0U)

#define RCC_TIM1_CLK_DISABLE()      (RCC->APB2ENR &= ~(RCC_APB2ENR_TIM1EN))
#define RCC_SPI1_CLK_DISABLE()      (RCC->APB2ENR &= ~(RCC_APB2ENR_SPI1EN))
#define RCC_USART1_CLK_DISABLE()    (RCC->APB2ENR &= ~(RCC_APB2ENR_USART1EN))

#define RCC_TIM2_CLK_DISABLE()      (RCC->APB1ENR &= ~(RCC_APB1ENR_TIM2EN))
#define RCC_TIM3_CLK_DISABLE()      (RCC->APB1ENR &= ~(RCC_APB1ENR_TIM3EN))
#define RCC_WWDG_CLK_DISABLE()      (RCC->APB1ENR &= ~(RCC_APB1ENR_WWDGEN))
#define RCC_USART2_CLK_DISABLE()    (RCC->APB1ENR &= ~(RCC_APB1ENR_USART2EN))
#define RCC_I2C1_CLK_DISABLE()      (RCC->APB1ENR &= ~(RCC_APB1ENR_I2C1EN))
#define RCC_AFIO_CLK_DISABLE()      (RCC->APB2ENR &= ~(RCC_APB2ENR_AFIOEN))
#define RCC_GPIOA_CLK_DISABLE()     (RCC->APB2ENR &= ~(RCC_APB2ENR_IOPAEN))
#define RCC_GPIOB_CLK_DISABLE()     (RCC->APB2ENR &= ~(RCC_APB2ENR_IOPBEN))
#define RCC_GPIOC_CLK_DISABLE()     (RCC->APB2ENR &= ~(RCC_APB2ENR_IOPCEN))
#define RCC_GPIOD_CLK_DISABLE()     (RCC->APB2ENR &= ~(RCC_APB2ENR_IOPDEN))
#define RCC_ADC1_CLK_DISABLE()      (RCC->APB2ENR &= ~(RCC_APB2ENR_ADC1EN))

#define RCC_TIM4_CLK_DISABLE()        (RCC->APB1ENR &= ~(RCC_APB1ENR_TIM4EN))
#define RCC_SPI2_CLK_DISABLE()        (RCC->APB1ENR &= ~(RCC_APB1ENR_SPI2EN))
#define RCC_USART3_CLK_DISABLE()      (RCC->APB1ENR &= ~(RCC_APB1ENR_USART3EN))
#define RCC_I2C2_CLK_DISABLE()        (RCC->APB1ENR &= ~(RCC_APB1ENR_I2C2EN))

#endif /* RCC_H */
