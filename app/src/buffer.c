
#include "../inc/buffer.h"
#include <string.h>

bool RingBuffer_isFull(const ringBufferHandle_t *handle)
{
    return handle->ctrl.head == (handle->ctrl.tail - 1);
}

bool RingBuffer_isEmpty(const ringBufferHandle_t *handle)
{
    return (handle->ctrl.head == handle->ctrl.tail);
}

void RingBuffer_init(ringBufferHandle_t *handle)
{
    memset(handle->buffer, '\0', sizeof(handle->buffer));
    handle->ctrl.head = 0;
    handle->ctrl.tail = 0;
}

void RingBuffer_write(ringBufferHandle_t *handle, char data)
{
    // clear buffer if it's full before writing
    if(RingBuffer_isFull(handle))
    {
        RingBuffer_init(handle);
    }

    handle->buffer[handle->ctrl.head] = data;
    handle->ctrl.head  = (handle->ctrl.head + 1) % UART_BUFFER_SIZE;

}

char RingBuffer_read(ringBufferHandle_t *handle)
{
    char data = '\0';
    if (!RingBuffer_isEmpty(handle))
    {
        data = handle->buffer[handle->ctrl.tail];
        handle->ctrl.tail = (handle->ctrl.tail + 1) % UART_BUFFER_SIZE;
    }
    return data;
}