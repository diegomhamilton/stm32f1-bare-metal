#include "core_cm3.h"
#include "stm32f103xb.h"
#include "virtual_timer.h"


void vt_init(void) {
    NVIC_ClearPendingIRQ(SysTick_IRQn);
    NVIC_EnableIRQ(SysTick_IRQn);
}

void vt_deinit(void) {
    NVIC_DisableIRQ(SysTick_IRQn);
}
