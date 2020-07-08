#include "virtual_timer_priv.h"
#include "virtual_timer.h"
#include "reactor.h"


static volatile uint32_t no_ticks = 0;

void
SysTick_Handler(void)
{
    int i;

    no_ticks++;

    for(i=0; i<VT_RT_HANDLER_ARRAY_MAX; i++)
        if(vt_rt_handler_array[i])
            if(((timer_cb_t) vt_rt_handler_array[i])())
                vt_rt_handler_array[i] = 0;

    for(i=VT_NON_RT_HANDLER_ARRAY_MAX; i<2*VT_NON_RT_HANDLER_ARRAY_MAX; i++)
        if(vt_non_rt_handler_array[i]) {
            if(--vt_non_rt_handler_array[i - VT_NON_RT_HANDLER_ARRAY_MAX])
                continue;
            add_reactor_handler(vt_non_rt_handler_array[i], 0);
            if(!vt_non_rt_handler_array[i + VT_NON_RT_HANDLER_ARRAY_MAX]) {
                /* If the handler is a one-shot, kill it right
                   away */
                vt_non_rt_handler_array[i] = 0;
                vt_non_rt_handler_array[i - VT_NON_RT_HANDLER_ARRAY_MAX] = 0;
            } else
                /* Otherwise, reloads it with the appropriate
                   timeout value */
                vt_non_rt_handler_array[i - VT_NON_RT_HANDLER_ARRAY_MAX] =
                    vt_non_rt_handler_array[i + VT_NON_RT_HANDLER_ARRAY_MAX];
        }
}
