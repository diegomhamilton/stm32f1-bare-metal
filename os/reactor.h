#ifndef REACTOR_H
#define REACTOR_H

#include "hcos_types_impl.h"

typedef void (*reactor_cb_t)(hcos_word_t);

void reactor_init(void);
void reactor_start(void);
hcos_base_int_t add_reactor_handler(reactor_cb_t cb, hcos_word_t data);

#endif /* REACTOR_H */
