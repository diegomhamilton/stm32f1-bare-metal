#include "stm32f103xb.h"
#include "dma.h"
#include "reactor.h"


/*
 * Global variables definitions
 */
dma_driver_t DMAD1;


/*
 * Functions definitions
 */
void dma_init(void) {
    int i;

    DMAD1.dev = DMA1;
    for (i=0; i<DMA_NBR_CHANNELS; i++) {
	DMAD1.half_transfer_cb[i] = DMAD1.full_transfer_cb[i] =
	    DMAD1.error_cb[i] = 0;
	DMAD1.half_transfer_rt_cb[i] = DMAD1.full_transfer_rt_cb[i] =
	    DMAD1.error_rt_cb[i] = 0;
	DMAD1.half_transfer_args[i] = DMAD1.full_transfer_args[i] =
	    DMAD1.error_args[i] = i;
	DMAD1.half_transfer_rt_args[i] = DMAD1.full_transfer_rt_args[i] =
	    DMAD1.error_rt_args[i] = i;
    }
}

int dma_start(dma_driver_t* drv) {
    if (drv == &DMAD1) {
	RCC_DMA1_CLK_ENABLE();

	NVIC_SetPriority(DMA1_Channel1_IRQn, 1);
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	NVIC_SetPriority(DMA1_Channel2_IRQn, 1);
	NVIC_EnableIRQ(DMA1_Channel2_IRQn);
	NVIC_SetPriority(DMA1_Channel3_IRQn, 1);
	NVIC_EnableIRQ(DMA1_Channel3_IRQn);
	NVIC_SetPriority(DMA1_Channel4_IRQn, 1);
	NVIC_EnableIRQ(DMA1_Channel4_IRQn);
	NVIC_SetPriority(DMA1_Channel5_IRQn, 1);
	NVIC_EnableIRQ(DMA1_Channel5_IRQn);
	NVIC_SetPriority(DMA1_Channel6_IRQn, 1);
	NVIC_EnableIRQ(DMA1_Channel6_IRQn);
	NVIC_SetPriority(DMA1_Channel7_IRQn, 1);
	NVIC_EnableIRQ(DMA1_Channel7_IRQn);
    }

    return 0;
}

int dma_unbind(dma_driver_t* drv, int channel) {
    drv->dev->channels[channel].CCR = 0;

    drv->half_transfer_cb[channel] = 0;
    drv->full_transfer_cb[channel] = 0;
    drv->error_cb[channel] = 0;

    return 0;
}

int dma_stop(dma_driver_t* drv) {
    /* TODO: Disable interrupts (all of them?) */
    RCC_DMA1_CLK_DISABLE();
    return 0;
}

void default_error_cb(hcos_word_t arg) {
}

void dma_memcpy_basic(dma_driver_t* drv, const uint8_t* src, uint8_t* dst,
		      uint16_t n, reactor_cb_t cb, uint8_t pinc) {
    int n_orig = n;
    int ctr = n >> 2;  /* Dividing by 4 */
    uint8_t channel;
    dma_bind_config_t cfg = {.peripheral_address = (uint32_t) src,
			     .memory_address  = (uint32_t) dst,
			     .nbr_bytes       = ctr,
			     .mem2mem_flag    = DMA_MEM2MEM_MODE,
			     .priority        = DMA_PRIO_LOW,
			     .peripheral_size = DMA_PER_SIZE_32BITS,
			     .memory_size     = DMA_MEM_SIZE_32BITS,
			     .peripheral_increment = (pinc ? DMA_PER_INCREMENT_MODE : 0),
			     .memory_increment = DMA_MEM_INCREMENT_MODE,
			     .direction = DMA_FROM_PER_MODE,
			     .circular_mode = DMA_NO_CIRCULAR_MODE,
			     .half_transfer_cb = 0,
			     .full_transfer_cb = cb,
			     .error_cb = default_error_cb,
			     .half_transfer_args = 0,
			     .full_transfer_args = 0,
			     .error_args = 0,
			     .half_transfer_rt_cb = 0,
			     .full_transfer_rt_cb = 0,
			     .error_rt_cb = 0,
			     .half_transfer_rt_args = 0,
			     .full_transfer_rt_args = 0,
			     .error_rt_args = 0,
    };

    if (ctr) {
	for (channel=0; channel<7; channel++)
	    if (!dma_is_busy(drv, channel))
		break;
	if (channel < 7) {
	    dma_bind(drv, channel, cfg);
	    dma_enable(drv, channel);
	    n &= 0x3;  /* Remainder of n divided by 4 */
	}
    }

    /* Write the remaining few bytes */
    src += (pinc ? n_orig - n : 0);
    dst += (n_orig - n);
    if (pinc) {
	while(n-- > 0)
	    *dst++ = *src++;
    } else {
	while(n-- > 0)
	    *dst++ = *src;
    }
}

void DMA1_Channel1_IRQHandler(void) {
    uint32_t isr = DMA1->ISR;

    if (isr & DMA_ISR_TCIF1) { /*  */
	DMA1->IFCR = DMA_ISR_TCIF1;
	if (DMAD1.full_transfer_rt_cb[0])
	    DMAD1.full_transfer_rt_cb[0](DMAD1.full_transfer_rt_args[0]);
	if (DMAD1.full_transfer_cb[0])
	    reactor_add_handler(DMAD1.full_transfer_cb[0],
				DMAD1.full_transfer_args[0]);
    } else if (isr & DMA_ISR_HTIF1) { /*  */
	DMA1->IFCR = DMA_ISR_HTIF1;
	if (DMAD1.half_transfer_rt_cb[0])
	    DMAD1.half_transfer_rt_cb[0](DMAD1.half_transfer_rt_args[0]);
	if (DMAD1.half_transfer_cb[0])
	    reactor_add_handler(DMAD1.half_transfer_cb[0],
				DMAD1.half_transfer_args[0]);
    } else if (isr & DMA_ISR_TEIF1) { /*  */
	DMA1->IFCR = DMA_ISR_TEIF1;
	if (DMAD1.error_rt_cb[0])
	    DMAD1.error_rt_cb[0](DMAD1.error_rt_args[0]);
	if (DMAD1.error_cb[0])
	    reactor_add_handler(DMAD1.error_cb[0], DMAD1.error_args[0]);
    }
}

void DMA1_Channel2_IRQHandler(void) {
    uint32_t isr = DMA1->ISR;

    if (isr & DMA_ISR_TCIF2) { /*  */
	DMA1->IFCR = DMA_ISR_TCIF2;
	if (DMAD1.full_transfer_rt_cb[1])
	    DMAD1.full_transfer_rt_cb[1](DMAD1.full_transfer_rt_args[1]);
	if (DMAD1.full_transfer_cb[1])
	    reactor_add_handler(DMAD1.full_transfer_cb[1],
				DMAD1.full_transfer_args[1]);
    } else if (isr & DMA_ISR_HTIF2) { /*  */
	DMA1->IFCR = DMA_ISR_HTIF2;
	if (DMAD1.half_transfer_rt_cb[1])
	    DMAD1.half_transfer_rt_cb[1](DMAD1.half_transfer_rt_args[1]);
	if (DMAD1.half_transfer_cb[1])
	    reactor_add_handler(DMAD1.half_transfer_cb[1],
				DMAD1.half_transfer_args[1]);
    } else if (isr & DMA_ISR_TEIF2) { /*  */
	DMA1->IFCR = DMA_ISR_TEIF2;
	if (DMAD1.error_rt_cb[1])
	    DMAD1.error_rt_cb[1](DMAD1.error_rt_args[1]);
	if (DMAD1.error_cb[1])
	    reactor_add_handler(DMAD1.error_cb[1], DMAD1.error_args[1]);
    }
}

void DMA1_Channel3_IRQHandler(void) {
    uint32_t isr = DMA1->ISR;

    if (isr & DMA_ISR_TCIF3) { /*  */
	DMA1->IFCR = DMA_ISR_TCIF3;
	if (DMAD1.full_transfer_rt_cb[2])
	    DMAD1.full_transfer_rt_cb[2](DMAD1.full_transfer_rt_args[2]);
	if (DMAD1.full_transfer_cb[2])
	    reactor_add_handler(DMAD1.full_transfer_cb[2],
				DMAD1.full_transfer_args[2]);
    } else if (isr & DMA_ISR_HTIF3) { /*  */
	DMA1->IFCR = DMA_ISR_HTIF3;
	if (DMAD1.half_transfer_rt_cb[2])
	    DMAD1.half_transfer_rt_cb[2](DMAD1.half_transfer_rt_args[2]);
	if (DMAD1.half_transfer_cb[2])
	    reactor_add_handler(DMAD1.half_transfer_cb[2],
				DMAD1.half_transfer_args[2]);
    } else if (isr & DMA_ISR_TEIF3) { /*  */
	DMA1->IFCR = DMA_ISR_TEIF3;
	if (DMAD1.error_rt_cb[2])
	    DMAD1.error_rt_cb[2](DMAD1.error_rt_args[2]);
	if (DMAD1.error_cb[2])
	    reactor_add_handler(DMAD1.error_cb[2], DMAD1.error_args[2]);
    }
}

void DMA1_Channel4_IRQHandler(void) {
    uint32_t isr = DMA1->ISR;

    if (isr & DMA_ISR_TCIF4) { /*  */
	DMA1->IFCR = DMA_ISR_TCIF4;
	if (DMAD1.full_transfer_rt_cb[3])
	    DMAD1.full_transfer_rt_cb[3](DMAD1.full_transfer_rt_args[3]);
	if (DMAD1.full_transfer_cb[3])
	    reactor_add_handler(DMAD1.full_transfer_cb[3],
				DMAD1.full_transfer_args[3]);
    } else if (isr & DMA_ISR_HTIF4) { /*  */
	DMA1->IFCR = DMA_ISR_HTIF4;
	if (DMAD1.half_transfer_rt_cb[3])
	    DMAD1.half_transfer_rt_cb[3](DMAD1.half_transfer_rt_args[3]);
	if (DMAD1.half_transfer_cb[3])
	    reactor_add_handler(DMAD1.half_transfer_cb[3],
				DMAD1.half_transfer_args[3]);
    } else if (isr & DMA_ISR_TEIF4) { /*  */
	DMA1->IFCR = DMA_ISR_TEIF4;
	if (DMAD1.error_rt_cb[3])
	    DMAD1.error_rt_cb[3](DMAD1.error_rt_args[3]);
	if (DMAD1.error_cb[3])
	    reactor_add_handler(DMAD1.error_cb[3], DMAD1.error_args[3]);
    }
}

void DMA1_Channel5_IRQHandler(void) {
    uint32_t isr = DMA1->ISR;

    if (isr & DMA_ISR_TCIF5) { /*  */
	DMA1->IFCR = DMA_ISR_TCIF5;
	if (DMAD1.full_transfer_rt_cb[4])
	    DMAD1.full_transfer_rt_cb[4](DMAD1.full_transfer_rt_args[4]);
	if (DMAD1.full_transfer_cb[4])
	    reactor_add_handler(DMAD1.full_transfer_cb[4],
				DMAD1.full_transfer_args[4]);
    } else if (isr & DMA_ISR_HTIF5) { /*  */
	DMA1->IFCR = DMA_ISR_HTIF5;
	if (DMAD1.half_transfer_rt_cb[4])
	    DMAD1.half_transfer_rt_cb[4](DMAD1.half_transfer_rt_args[4]);
	if (DMAD1.half_transfer_cb[4])
	    reactor_add_handler(DMAD1.half_transfer_cb[4],
				DMAD1.half_transfer_args[4]);
    } else if (isr & DMA_ISR_TEIF5) { /*  */
	DMA1->IFCR = DMA_ISR_TEIF5;
	if (DMAD1.error_rt_cb[4])
	    DMAD1.error_rt_cb[4](DMAD1.error_rt_args[4]);
	if (DMAD1.error_cb[4])
	    reactor_add_handler(DMAD1.error_cb[4], DMAD1.error_args[4]);
    }
}

void DMA1_Channel6_IRQHandler(void) {
    uint32_t isr = DMA1->ISR;

    if (isr & DMA_ISR_TCIF6) { /*  */
	DMA1->IFCR = DMA_ISR_TCIF6;
	if (DMAD1.full_transfer_rt_cb[5])
	    DMAD1.full_transfer_rt_cb[5](DMAD1.full_transfer_rt_args[5]);
	if (DMAD1.full_transfer_cb[5])
	    reactor_add_handler(DMAD1.full_transfer_cb[5],
				DMAD1.full_transfer_args[5]);
    } else if (isr & DMA_ISR_HTIF6) { /*  */
	DMA1->IFCR = DMA_ISR_HTIF6;
	if (DMAD1.half_transfer_rt_cb[5])
	    DMAD1.half_transfer_rt_cb[5](DMAD1.half_transfer_rt_args[5]);
	if (DMAD1.half_transfer_cb[5])
	    reactor_add_handler(DMAD1.half_transfer_cb[5],
				DMAD1.half_transfer_args[5]);
    } else if (isr & DMA_ISR_TEIF6) { /*  */
	DMA1->IFCR = DMA_ISR_TEIF6;
	if (DMAD1.error_rt_cb[5])
	    DMAD1.error_rt_cb[5](DMAD1.error_rt_args[5]);
	if (DMAD1.error_cb[5])
	    reactor_add_handler(DMAD1.error_cb[5], DMAD1.error_args[5]);
    }
}

void DMA1_Channel7_IRQHandler(void) {
    uint32_t isr = DMA1->ISR;

    if (isr & DMA_ISR_TCIF7) { /*  */
	DMA1->IFCR = DMA_ISR_TCIF7;
	if (DMAD1.full_transfer_rt_cb[6])
	    DMAD1.full_transfer_rt_cb[6](DMAD1.full_transfer_rt_args[6]);
	if (DMAD1.full_transfer_cb[6])
	    reactor_add_handler(DMAD1.full_transfer_cb[6],
				DMAD1.full_transfer_args[6]);
    } else if (isr & DMA_ISR_HTIF7) { /*  */
	DMA1->IFCR = DMA_ISR_HTIF7;
	if (DMAD1.half_transfer_rt_cb[6])
	    DMAD1.half_transfer_rt_cb[6](DMAD1.half_transfer_rt_args[6]);
	if (DMAD1.half_transfer_cb[6])
	    reactor_add_handler(DMAD1.half_transfer_cb[6],
				DMAD1.half_transfer_args[6]);
    } else if (isr & DMA_ISR_TEIF7) { /*  */
	DMA1->IFCR = DMA_ISR_TEIF7;
	if (DMAD1.error_rt_cb[6])
	    DMAD1.error_rt_cb[6](DMAD1.error_rt_args[6]);
	if (DMAD1.error_cb[6])
	    reactor_add_handler(DMAD1.error_cb[6], DMAD1.error_args[6]);
    }
}
