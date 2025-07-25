#ifndef _RING_BUFFER_
#define _RING_BUFFER_

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

typedef struct
{
    uint32_t head;
    uint32_t tail;

}ringBuffer_t;

typedef struct
{
    ringBuffer_t ctrl;
    char buffer[UART_BUFFER_SIZE];
}ringBufferHandle_t;

void RingBuffer_init(ringBufferHandle_t *handle);
void RingBuffer_write(ringBufferHandle_t *handle, char data);
char RingBuffer_read(ringBufferHandle_t *handle);
bool RingBuffer_isFull(const ringBufferHandle_t *handle);
bool RingBuffer_isEmpty(const ringBufferHandle_t *handle);


#endif