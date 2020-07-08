#include "reactor.h"
#include "hcos_asm_impl.h"


extern char _CHECK_SIZE_OF_[1/!(sizeof(hcos_word_t) != sizeof(hcos_ptr_t))];


#define QUEUE_MAX_SZ 8

volatile hcos_base_int_t queue_sz;
reactor_cb_t queue[QUEUE_MAX_SZ*2];
hcos_base_int_t head;
hcos_base_int_t tail;
volatile hcos_base_int_t reactor_on;


void
reactor_init(void)
{
	if(reactor_on)
		return;
	reactor_on = 1;
	head = tail = queue_sz = 0;
}

void
reactor_start(void)
{
	reactor_cb_t cb;
	hcos_word_t data;

	while(reactor_on) {
		if (!queue_sz)
			continue;
		hcos_cli();
		queue_sz--;
		cb = queue[2*tail];
		if(!cb) {
			hcos_sei();
			log_warning(1);
			continue;
		}
		data = (hcos_word_t) queue[2*tail+1];
		queue[2*tail] = queue[2*tail+1] = 0;
		if(++tail >= QUEUE_MAX_SZ)
			tail = 0;
		hcos_sei();
		cb(data);
	}
}


hcos_base_int_t
add_reactor_handler(reactor_cb_t cb, hcos_word_t data)
{
	hcos_base_int_t rc = 0;
	hcos_cli();
	if(queue_sz >= QUEUE_MAX_SZ) {
		log_warning(2);
		rc = 1;
	}
	else {
		queue_sz++;
		queue[2*head] = cb;
		queue[2*head+1] = (reactor_cb_t) data;
		if(++head >= QUEUE_MAX_SZ)
			head = 0;
	}
	hcos_sei();
	return rc;
}
