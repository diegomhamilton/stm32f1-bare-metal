#include "virtual_timer.h"
#include "virtual_timer_priv.h"
#include "hcos_asm_impl.h"


extern char _CHECK_SIZE_OF_[1/!(sizeof(hcos_word_t) != sizeof(hcos_ptr_t))];


hcos_ptr_t vt_rt_handler_array[VT_RT_HANDLER_ARRAY_MAX];
hcos_ptr_t vt_non_rt_handler_array[VT_NON_RT_HANDLER_ARRAY_MAX*3];


void vt_init(void) __attribute ((weak));
void vt_init(void) {
    /* Some mumbo-jumbo to set timer */
}


void vt_deinit(void) __attribute ((weak));
void vt_deinit(void) {
    /* Some mumbo-jumbo to set timer */
}


hcos_base_int_t vt_add_rt_handler(timer_cb_t cb) {
    hcos_base_int_t rc = 1;
    int i;

    if(!cb)
        return rc;

    hcos_cli();
    for(i=0; i<VT_RT_HANDLER_ARRAY_MAX; i++)
        if(!vt_rt_handler_array[i]) {
            vt_rt_handler_array[i] = (hcos_ptr_t) cb;
            rc = 0;
            break;
        }
    hcos_sei();
    return rc;
}


hcos_base_int_t vt_add_non_rt_handler(timer_cb_t cb, hcos_word_t timeout, hcos_base_int_t is_periodic) {
    hcos_base_int_t rc = 1;
    int i;

    if(!(timeout && cb))
        return rc;

    hcos_cli();
    for(i=0; i<VT_NON_RT_HANDLER_ARRAY_MAX; i++)
        if(!vt_non_rt_handler_array[i]) {
            vt_non_rt_handler_array[i] = (hcos_ptr_t) timeout;
            vt_non_rt_handler_array[i + VT_NON_RT_HANDLER_ARRAY_MAX] = (hcos_ptr_t) cb;
            if(is_periodic)
                vt_non_rt_handler_array[i + 2*VT_NON_RT_HANDLER_ARRAY_MAX] = (hcos_ptr_t) timeout;
            else
                vt_non_rt_handler_array[i + 2*VT_NON_RT_HANDLER_ARRAY_MAX] = 0;
            rc = 0;
            break;
        }
    hcos_sei();
    return rc;
}


hcos_base_int_t vt_delete_rt_handler(timer_cb_t cb) {
    hcos_base_int_t rc = 1;
    int i;

    if(!cb)
        return rc;

    hcos_cli();
    for(i=0; i<VT_RT_HANDLER_ARRAY_MAX; i++)
        if(vt_rt_handler_array[i] == (hcos_ptr_t) cb) {
            vt_rt_handler_array[i] = 0;
            rc = 0;
            break;
        }
    hcos_sei();
    return rc;
}


hcos_base_int_t vt_delete_non_rt_handler(timer_cb_t cb) {
    hcos_base_int_t rc = 1;
    int i;

    if(!cb)
        return rc;

    hcos_cli();
    for(i=VT_NON_RT_HANDLER_ARRAY_MAX; i<2*VT_NON_RT_HANDLER_ARRAY_MAX; i++)
        if(vt_non_rt_handler_array[i] == (hcos_ptr_t) cb) {
              vt_non_rt_handler_array[i - VT_NON_RT_HANDLER_ARRAY_MAX] = 0;
            vt_non_rt_handler_array[i] = 0;
            vt_non_rt_handler_array[i + VT_NON_RT_HANDLER_ARRAY_MAX] = 0;
            rc = 0;
            break;
        }
    hcos_sei();
    return rc;
}


hcos_base_int_t vt_update_non_rt_handler(timer_cb_t cb, hcos_word_t timeout, hcos_base_int_t is_periodic) {
    hcos_base_int_t rc = 1;
    int i;

    if(!(timeout && cb))
        return rc;

    hcos_cli();
    for(i=VT_NON_RT_HANDLER_ARRAY_MAX; i<2*VT_NON_RT_HANDLER_ARRAY_MAX; i++)
        if(vt_non_rt_handler_array[i] == (hcos_ptr_t) cb) {
            vt_non_rt_handler_array[i - VT_NON_RT_HANDLER_ARRAY_MAX] = (hcos_ptr_t) timeout;
            vt_non_rt_handler_array[i] = (hcos_ptr_t) cb;
            if(is_periodic)
                vt_non_rt_handler_array[i + VT_NON_RT_HANDLER_ARRAY_MAX] = (hcos_ptr_t) timeout;
            else
                vt_non_rt_handler_array[i + VT_NON_RT_HANDLER_ARRAY_MAX] = 0;
            rc = 0;
            break;
        }
    hcos_sei();
    return rc;
}
