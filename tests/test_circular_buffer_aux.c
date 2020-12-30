#include "buffers.h"


/* Initialize your buffer with element type and size. */
typedef BYTE_BUFFER_DEF(10) buffer_int_t;
typedef BUFFER_STRUCT_DEF(float, uint8_t, 10) buffer_float_t;

void test_buffer_push_2_pop_1(buffer_int_t *buffer, int repeat, int i1, int i2) {
    int i;
    uint8_t j = i1 + i2;
    uint16_t storage = 0;

//    printf("Testing buffer with 2 pushes / 1 pop %d times...\r\n", repeat);
//    print_buffer(buffer);
    for (i = 1; i <= repeat; i++) {
        /* Also testing expression parameter in push */
        buffer_push(*buffer, j++);
        buffer_push(*buffer, j++);
        buffer_pop(*buffer, storage);
        
        /* This should print the numbers from 0 to 8 sequentially if repeat > 10.
         * After this (buffer full), only even numbers will be printed, as the
         * odd numbers are being overwritten. At this moment, the occupation should
         * be always 9 (we are popping a full buffer with 10 elements, so we get 9)
         * */
//        printf("Popping %d, occupation %d\r\n", storage, (int) buffer_occupancy(buffer));
    }
//    printf("------------------------------\r\n");
}
