/*
 * ------------------------------------------------------------
 * "THE BEERWARE LICENSE" (Revision 42):
 * diegohamilton26@gmail.com wrote this code. As long as you retain this 
 * notice, you can do whatever you want with this stuff. If we
 * meet someday, and you think this stuff is worth it, you can
 * buy me a beer in return,
 * Diego Hamilton.
 * ------------------------------------------------------------
 */

#include <stdio.h>
#include "buffers.h"

/* Initialize your buffer with element type and size. */
typedef BYTE_BUFFER_DEF(10) buffer_int_t;
typedef BUFFER_STRUCT_DEF(float, uint8_t, 10) buffer_float_t;

/* Test buffer functions */
void print_buffer(buffer_int_t *buffer);
void test_buffer_initialization(buffer_int_t *buffer);
void test_buffer_push_overwrite(buffer_int_t *buffer);
void test_buffer_pop_all(buffer_int_t *buffer);
void test_buffer_pop_empty(buffer_int_t *buffer);
void test_buffer_push_2_pop_1(buffer_int_t *buffer, int repeat, int, int);


int main(void) {
    int i, j;
    buffer_int_t buffer;
    buffer_reset(buffer);

    test_buffer_initialization(&buffer);
    test_buffer_push_overwrite(&buffer);
    test_buffer_pop_all(&buffer);
    test_buffer_pop_empty(&buffer);
    for (i=0; i<10000000; i++)
        for (j=0; j<5; j++)
            test_buffer_push_2_pop_1(&buffer, 15, i, j);

    buffer_float_t buffer2;
    buffer_reset(buffer2);

    return 0;
}

void print_buffer(buffer_int_t *buffer) {
    printf("Buffer info:\r\n\thead: %d, tail: %d\r\n", buffer->head, buffer->tail);
}

void test_buffer_initialization(buffer_int_t *buffer) {
    printf("Testing buffer initialization...\r\n");
    print_buffer(buffer);
    printf("Buffer empty, should be true: %d\r\n", is_buffer_empty(*buffer));
    printf("Buffer full, should be false: %d\r\n", is_buffer_full(*buffer));

    printf("------------------------------\r\n");
}

void test_buffer_push_overwrite(buffer_int_t *buffer) {
    int i;

    printf("Testing buffer w/ overwrite operation...\r\n");
    print_buffer(buffer);
    for(i = buffer_size(*buffer) + 1; i > 0; i--) {
        buffer_push(*buffer, i);
        printf("Pushed %d, occupation: %d\r\n", i, (int) buffer_occupancy(*buffer));
    }
    printf("------------------------------\r\n");
}

void test_buffer_pop_all(buffer_int_t *buffer) {
    int i;
    int buffer_occupation = buffer_occupancy(*buffer);
    uint16_t storage;

    printf("Testing popping all buffer elements...\r\n");
    print_buffer(buffer);
    for(i = 0; i < buffer_occupation; i++) {
        buffer_pop(*buffer, storage);
        printf("Element[%d] = %d, occupation: %d\r\n", i, storage, (int) buffer_occupancy(*buffer));
    }
    printf("------------------------------\r\n");
}

void test_buffer_pop_empty(buffer_int_t *buffer) {
    uint16_t storage = 0;

    printf("Testing popping empty buffer...\r\n");
    print_buffer(buffer);
    buffer_pop(*buffer, storage); //check that nothing unusual happens after popping empty buffer;
    printf("Testing whether popping empty buffer behaved correctly, expects -1: %d\r\n", storage);
    printf("------------------------------\r\n");
}
