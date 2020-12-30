#ifndef VIRTUAL_TIMER_H
#define VIRTUAL_TIMER_H

#include "hcos_types_impl.h"

typedef hcos_base_int_t (*timer_cb_t)(void);

void vt_init(void);
void vt_deinit(void);
hcos_base_int_t vt_add_rt_handler(timer_cb_t);
hcos_base_int_t vt_add_non_rt_handler(timer_cb_t, hcos_word_t, hcos_base_int_t);
hcos_base_int_t vt_delete_rt_handler(timer_cb_t);
hcos_base_int_t vt_delete_non_rt_handler(timer_cb_t);
hcos_base_int_t vt_update_non_rt_handler(timer_cb_t, hcos_word_t, hcos_base_int_t);

#endif /* VIRTUAL_TIMER_H */
