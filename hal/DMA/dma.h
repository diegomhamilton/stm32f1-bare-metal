#ifndef DMA_H
#define DMA_H

#include <stdint.h>
#include "stm32f103xb.h"
#include "rcc.h"
#include "reactor.h"
#include "hcos_types_impl.h"


/*
 * Constants for all DMA1 channels.  This is one less than given in
 * the RM008 document since we assume a zero based channel id
 */
#define DMA_ADC1_CHANNEL 0

#define DMA_SPI1_RX_CHANNEL 1
#define DMA_SPI1_TX_CHANNEL 2
#define DMA_SPI2_RX_CHANNEL 3
#define DMA_SPI2_TX_CHANNEL 4

#define DMA_UART3_TX_CHANNEL 1
#define DMA_UART3_RX_CHANNEL 2
#define DMA_UART1_TX_CHANNEL 3
#define DMA_UART1_RX_CHANNEL 4
#define DMA_UART2_RX_CHANNEL 5
#define DMA_UART2_TX_CHANNEL 6

#define DMA_I2C2_TX_CHANNEL 3
#define DMA_I2C2_RX_CHANNEL 4
#define DMA_I2C1_TX_CHANNEL 5
#define DMA_I2C1_RX_CHANNEL 6

#define DMA_TIM1_CH1_CHANNEL 1
#define DMA_TIM1_CH4_CHANNEL 3
#define DMA_TIM1_TRG_CHANNEL 3
#define DMA_TIM1_COM_CHANNEL 3
#define DMA_TIM1_UP_CHANNEL  4
#define DMA_TIM1_CH3_CHANNEL 5

#define DMA_TIM2_CH3_CHANNEL 0
#define DMA_TIM2_UP_CHANNEL  1
#define DMA_TIM2_CH1_CHANNEL 4
#define DMA_TIM2_CH2_CHANNEL 6
#define DMA_TIM2_CH4_CHANNEL 6

#define DMA_TIM3_CH3_CHANNEL 1
#define DMA_TIM3_CH4_CHANNEL 2
#define DMA_TIM3_UP_CHANNEL  2
#define DMA_TIM3_CH1_CHANNEL 5
#define DMA_TIM3_TRG_CHANNEL 5

#define DMA_TIM4_CH1_CHANNEL 0
#define DMA_TIM4_CH2_CHANNEL 3
#define DMA_TIM4_CH3_CHANNEL 4
#define DMA_TIM4_UP_CHANNEL  6


/*
 * Configuration constants
 */
#define DMA_CIRCULAR_MODE DMA_CCR_CIRC
#define DMA_NO_CIRCULAR_MODE 0
#define DMA_MEM_INCREMENT_MODE DMA_CCR_MINC
#define DMA_NO_MEM_INCREMENT_MODE 0
#define DMA_PER_INCREMENT_MODE DMA_CCR_PINC
#define DMA_NO_PER_INCREMENT_MODE 0
#define DMA_FROM_PER_MODE 0
#define DMA_FROM_MEM_MODE DMA_CCR_DIR

#define DMA_MEM2MEM_MODE DMA_CCR_MEM2MEM
#define DMA_PER2MEM_MODE 0

#define DMA_MEM_SIZE_8BITS 0
#define DMA_PER_SIZE_8BITS 0
#define DMA_MEM_SIZE_16BITS DMA_CCR_MSIZE_0
#define DMA_PER_SIZE_16BITS DMA_CCR_PSIZE_0
#define DMA_MEM_SIZE_32BITS DMA_CCR_MSIZE_1
#define DMA_PER_SIZE_32BITS DMA_CCR_PSIZE_1

#define DMA_PRIO_LOW 0
#define DMA_PRIO_MEDIUM DMA_CCR_PL_0
#define DMA_PRIO_HIGH DMA_CCR_PL_1
#define DMA_PRIO_VERY_HIGH (DMA_CCR_PL_0 | DMA_CCR_PL_1)


/*
 * Structure definitions
 */
typedef struct dma_bind_config_struct {
    uint32_t peripheral_address;
    uint32_t memory_address;
    uint32_t nbr_transfers;
    uint32_t mem2mem_flag;
    uint32_t priority;
    uint32_t peripheral_size;
    uint32_t memory_size;
    uint32_t peripheral_increment;
    uint32_t memory_increment;
    uint32_t direction;
    uint32_t circular_mode;
    reactor_cb_t half_transfer_cb;
    reactor_cb_t full_transfer_cb;
    reactor_cb_t error_cb;
    hcos_word_t half_transfer_args;
    hcos_word_t full_transfer_args;
    hcos_word_t error_args;
    reactor_cb_t half_transfer_rt_cb;
    reactor_cb_t full_transfer_rt_cb;
    reactor_cb_t error_rt_cb;
    hcos_word_t half_transfer_rt_args;
    hcos_word_t full_transfer_rt_args;
    hcos_word_t error_rt_args;
} dma_bind_config_t;

typedef struct dma_driver_struct {
    DMA_TypeDef* dev;
    reactor_cb_t half_transfer_cb[DMA_NBR_CHANNELS];
    reactor_cb_t full_transfer_cb[DMA_NBR_CHANNELS];
    reactor_cb_t error_cb[DMA_NBR_CHANNELS];
    hcos_word_t half_transfer_args[DMA_NBR_CHANNELS];
    hcos_word_t full_transfer_args[DMA_NBR_CHANNELS];
    hcos_word_t error_args[DMA_NBR_CHANNELS];
    reactor_cb_t half_transfer_rt_cb[DMA_NBR_CHANNELS];
    reactor_cb_t full_transfer_rt_cb[DMA_NBR_CHANNELS];
    reactor_cb_t error_rt_cb[DMA_NBR_CHANNELS];
    hcos_word_t half_transfer_rt_args[DMA_NBR_CHANNELS];
    hcos_word_t full_transfer_rt_args[DMA_NBR_CHANNELS];
    hcos_word_t error_rt_args[DMA_NBR_CHANNELS];
} dma_driver_t;


/*
 * Global variables definitions
 */
extern dma_driver_t DMAD1;

/*
 * Macros definitions
 */
#define dma_bind(drv, channel, cfg) do {				\
	(drv)->dev->channels[channel].CCR = 0;				\
	(drv)->half_transfer_cb[channel] = (cfg).half_transfer_cb;	\
	(drv)->full_transfer_cb[channel] = (cfg).full_transfer_cb;	\
	(drv)->error_cb[channel] = (cfg).error_cb;			\
	(drv)->half_transfer_args[channel] = (cfg).half_transfer_args;	\
	(drv)->full_transfer_args[channel] = (cfg).full_transfer_args;	\
	(drv)->error_args[channel] = (cfg).error_args;			\
	(drv)->half_transfer_rt_cb[channel] = (cfg).half_transfer_rt_cb; \
	(drv)->full_transfer_rt_cb[channel] = (cfg).full_transfer_rt_cb; \
	(drv)->error_rt_cb[channel] = (cfg).error_rt_cb;		\
	(drv)->half_transfer_rt_args[channel] = (cfg).half_transfer_rt_args; \
	(drv)->full_transfer_rt_args[channel] = (cfg).full_transfer_rt_args; \
	(drv)->error_rt_args[channel] = (cfg).error_rt_args;		\
									\
	(drv)->dev->IFCR = ((channel) == 0 ? DMA_IFCR_CGIF1 :		\
			    ((channel) == 1 ? DMA_IFCR_CGIF2 :		\
			     ((channel) == 2 ? DMA_IFCR_CGIF3 :		\
			      ((channel) == 3 ? DMA_IFCR_CGIF4 :	\
			       ((channel) == 4 ? DMA_IFCR_CGIF5 :	\
				((channel) == 5 ? DMA_IFCR_CGIF6 :	\
				 ((channel) == 6 ? DMA_IFCR_CGIF7 : 0))))))); \
									\
	(drv)->dev->channels[channel].CCR =				\
	    ((cfg).mem2mem_flag ? DMA_CCR_MEM2MEM : 0) |		\
	    (cfg).peripheral_size | (cfg).memory_size |			\
	    (cfg).peripheral_increment | (cfg).memory_increment |	\
	    (cfg).direction | (cfg).circular_mode | (cfg).priority |	\
	    ((cfg).half_transfer_cb || (cfg).half_transfer_rt_cb ? DMA_CCR_HTIE : 0) | \
	    ((cfg).full_transfer_cb || (cfg).full_transfer_rt_cb ? DMA_CCR_TCIE : 0) | \
	    ((cfg).error_cb || (cfg).error_rt_cb ? DMA_CCR_TEIE : 0);	\
	(drv)->dev->channels[channel].CPAR = (cfg).peripheral_address;	\
	(drv)->dev->channels[channel].CMAR = (cfg).memory_address;	\
	(drv)->dev->channels[channel].CNDTR = (cfg).nbr_transfers;		\
    } while(0)

#define dma_is_busy(drv, channel) ((drv)->dev->channels[channel].CNDTR != 0)
#define dma_enable(drv, channel) ((drv)->dev->channels[channel].CCR |= DMA_CCR_EN)
#define dma_disable(drv, channel) ((drv)->dev->channels[channel].CCR &= ~DMA_CCR_EN)

/*
 * Functions definitions
 */
void dma_init(void);
int dma_start(dma_driver_t* drv);
int dma_unbind(dma_driver_t* drv, int channel);
int dma_stop(dma_driver_t* drv);

extern uint32_t src;
void dma_memcpy_basic(dma_driver_t* drv, const uint8_t* src, uint8_t* dst,
		      uint16_t n, reactor_cb_t cb, uint8_t pinc);
#define dma_memcpy(drv, src, dst, n, cb) dma_memcpy_basic(drv, src, dst, n, cb, 1)
inline void dma_memset(dma_driver_t* drv, uint8_t *dst, uint8_t *b, uint16_t n,
		       reactor_cb_t cb) {
    uint32_t src = (((uint32_t) *b) << 24) | (((uint32_t) *b) << 16) |
	(((uint32_t) *b) << 8) | *b;
    dma_memcpy_basic(drv, (uint8_t *) &src, dst, n, cb, 0);
}

#endif /* DMA_H */
