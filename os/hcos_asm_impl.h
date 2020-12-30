#ifndef HCOS_ASM_STM32_IMPL
#define HCOS_ASM_STM32_IMPL

#include <cmsis_gcc.h>


#define hcos_nop() do { __NOP();} while(0)
#define hcos_cli() do { __disable_irq();} while(0)
#define hcos_sei() do { __enable_irq();} while(0)
#define log_warning(c) do { __NOP();} while(0)

#endif /* HCOS_ASM_STM32_IMPL */
