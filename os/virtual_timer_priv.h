#ifndef VIRTUAL_TIMER_PRIV
#define VIRTUAL_TIMER_PRIV

#include "hcos_types_impl.h"

#define VT_RT_HANDLER_ARRAY_MAX 4
#define VT_NON_RT_HANDLER_ARRAY_MAX 8

extern hcos_ptr_t vt_rt_handler_array[VT_RT_HANDLER_ARRAY_MAX];
extern hcos_ptr_t vt_non_rt_handler_array[VT_NON_RT_HANDLER_ARRAY_MAX*3];

#endif /* VIRTUAL_TIMER_PRIV */
