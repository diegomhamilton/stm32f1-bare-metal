#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "stm32f103xb.h"

/*
 * CONSTANTS DEFINING GPIO PIN MODE. Default speed of 10 MHz for the
 * output modes
 */
#define IN_ANALOG_MODE 0x00
#define IN_FLOATING 0x04
#define IN_PULLUP 0x08
#define IN_PULLDOWN 0x08
#define OUT_GP_PUSHPULL 0x01
#define OUT_GP_OPENDRAIN 0x05
#define OUT_AF_PUSHPULL 0x09
#define OUT_AF_OPENDRAIN 0x0D

/* Special values to be used with XOR and the constants above */
#define SPEED_10_MHZ 0b00
#define SPEED_2_MHZ 0b11
#define SPEED_50_MHZ 0b10


/*
 * Ports definitions
 */
#define IOPORT1 GPIOA
#define IOPORT2 GPIOB
#define IOPORT3 GPIOC
#define IOPORT4 GPIOD
#define IOPORT5 GPIOE


/*
 *
 */
#define generate_mode(inout, speed) (((speed) & ~0x3) | (inout))


/*
 * Functionality to extend a pin mode to a group of pins or the entire
 * port
 */
uint64_t gpio_replicate_mode_for_mask(uint8_t mode, uint16_t bitmask);

#define gpio_replicate_mode_for_port(mode) gpio_replicate_mode_for_mask(mode, 0xFFFF)
#define gpio_generate_mode_mask(bitmask) gpio_replicate_mode_for_mask(0x0F, bitmask)

#if !defined(GPIO_GROUP_MASK)
#define GPIO_GROUP_MASK(width) ((uint32_t)(1U << (width)) - 1U)
#endif


/*
 * Functions
 */
void gpio_init(void);


/*
 * Macros to operate on the port level
 */
#define gpio_read_port(port)			\
    ((uint32_t) ((port)->IDR))

#define gpio_read_port_latches(port)		\
    ((uint32_t) ((port)->ODR))

#define gpio_write_port(port, value)		\
    ((port)->ODR = (value))

#define gpio_set_port(port)			\
    ((port)->ODR = 0xFFFF)

#define gpio_clear_port(port)			\
    ((port)->ODR = 0)

#define gpio_toggle_port(port)			\
    ((port)->ODR = ~(port)->ODR)

#define gpio_set_port_mode(port, mode_64bits) do {	\
	(port)->CRL = (uint32_t) (mode_64bits);		\
	(port)->CRH = (uint32_t) ((mode_64bits) >> 32);	\
    } while(0)

#define gpio_get_port_mode(port) \
    ((((uint64_t)(port)->CRH) << 32) | (port)->CRL)


/*
 * Macros to operate om a level of group of bits
 */
#define gpio_read_group(port, mask, offset)		\
    ((gpio_read_port(port) >> (offset)) & (mask))

#define gpio_read_group_latches(port, mask, offset)		\
    ((gpio_read_port_latches(port) >> (offset)) & (mask))

#define gpio_write_group(port, mask, offset, value)			\
    ((port)->BSRR = (((uint32_t) ~((value) ^ (mask))) << ((offset) + 16)) | \
     ((value) << (offset)))
/* An alternate solution:
    (gpio_write_port((gpio_read_port(port) & ~((mask) << (offset))) |	\
    ((value) << (offset)))) */

#define gpio_set_group(port, mask, offset)	\
    ((port)->BSRR = ((mask) << (offset)))

#define gpio_clear_group(port, mask, offset)	\
    ((port)->BRR = ((mask) << (offset)))

#define gpio_toggle_group(port, mask, offset)			      \
    ((port)->BSRR = (((~(port)->ODR) & ((mask) << (offset))) << 16) | \
     ((port)->ODR & ((mask) << (offset))))

#define gpio_set_group_mode(port, bitmask, offset, mode)		\
    ((~gpio_generate_mode_mask((bitmask) << (offset)) & gpio_get_port_mode(port)) | \
     (((mode) & gpio_generate_mode_mask((bitmask)) << (offset))))

#define gpio_get_group_mode(port, bitmask, offset)			\
    (gpio_get_port_mode(port) & gpio_generate_mode_mask((bitmask) << (offset)))


/*
 * Macros to operate om the level of single bits
 */
/* TODO: Should we provide a version returning the value without
   shifting? */
#define gpio_read_pin(port, pin_nbr)		\
    ((gpio_read_port(port) >> (pin_nbr)) & 0x1)

#define gpio_read_pin_latch(port, pin_nbr)		\
    ((gpio_read_port_latches(port) >> (pin_nbr)) & 0x1)

#define gpio_write_pin(port, pin_nbr, value)				\
    ((port)->ODR = ((port)->ODR & ~(1 << (pin_nbr))) | ((value) << (pin_nbr)))

#define gpio_set_pin(port, pin_nbr)		\
    ((port)->BSRR = (1 << (pin_nbr)))

#define gpio_clear_pin(port, pin_nbr)		\
    (port)->BRR = (1 << (pin_nbr))

#define gpio_toggle_pin(port, pin_nbr)		\
    ((port)->BSRR = (((~(port)->ODR) & (1U << (pin_nbr))) << 16) |	\
     ((port)->ODR & (1U << (pin_nbr))))

static inline void gpio_set_pin_mode(GPIO_TypeDef *port, int pin_nbr, uint8_t mode) {
    if (pin_nbr < 8)
	port->CRL = (port->CRL & ~(0xFU << (pin_nbr*4))) |
	    ((mode & 0xF) << (pin_nbr*4));
    else
	port->CRH = (port->CRH & ~(0xFU << ((pin_nbr - 8)*4))) |
	    ((mode & 0xF) << ((pin_nbr - 8)*4));
    if (mode == IN_PULLUP)
	port->BSRR = 1 << pin_nbr;
    else if (mode == IN_PULLDOWN)
	port->BRR = 1 << pin_nbr;
}

#endif /* GPIO_H */
