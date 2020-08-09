#ifndef SYSTIME_H
#define SYSTIME_H

#include "stdint.h"
#include "core_cm3.h"

extern volatile uint32_t systick_no_ticks;

/* This assumes a 72MHz clock */
#define systime_get_time() ((((uint64_t) systick_no_ticks + 1)*72000) - SysTick->VAL)

#endif /* SYSTIME_H */
