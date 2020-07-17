#include "reactor.h"
#include "hcos_asm_impl.h"


extern char _CHECK_SIZE_OF_[1/!(sizeof(hcos_word_t) != sizeof(hcos_ptr_t))];


#define QUEUE_MAX_SZ 8

static reactor_cb_t queue[QUEUE_MAX_SZ*2];
static volatile hcos_base_int_t head;
static volatile hcos_base_int_t tail;
static volatile hcos_base_int_t reactor_on;


void
reactor_init(void) {
	if(reactor_on)
		return;
	head = tail = 0;
}

void
reactor_start(void) {
	reactor_cb_t cb;
	hcos_word_t data;

	reactor_on = 1;
	while(reactor_on) {
		if (head == tail)
			continue;
		cb = queue[2*tail];
		if(!cb) {
			log_warning(1);
			continue;
		}
		data = (hcos_word_t) queue[2*tail+1];
		queue[2*tail] = queue[2*tail+1] = 0;
                hcos_base_int_t new_tail = tail + 1;
		if(new_tail >= QUEUE_MAX_SZ)
			tail = 0;
                else
                        tail = new_tail;
		cb(data);
	}
}


hcos_base_int_t
reactor_add_handler(reactor_cb_t cb, hcos_word_t data) {
	hcos_base_int_t rc = 0;
	hcos_base_int_t new_head = head + 1;
	if (new_head >= QUEUE_MAX_SZ)
	    new_head = 0;
	if(new_head == tail) {
		log_warning(2);
		rc = 1;
	} else {
		queue[2*head] = cb;
		queue[2*head+1] = (reactor_cb_t) data;
		head = new_head;
	}
	return rc;
}
