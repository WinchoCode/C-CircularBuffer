#ifndef CIRCULAR_BUFFER_H // It seems that this is preferred style over #pragma once
#define CIRCULAR_BUFFER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

//Opaque circular buffer structure to hide implementation details
typedef struct circular_buf_t circular_buf_t;

//Handle type 
typedef circular_buf_t* circular_buf_handle_t;

///Pass in a storage buffer and size
///Returns handle
circular_buf_handle_t circular_buf_init(uint8_t* buffer, size_t size);

///Free the circular buffer structure
///Does not free the data buffer; you must free that yourself
void circular_buf_free(circular_buf_handle_t cbuf);

///Reset the circular buffer to empty, head == tail
void circular_buf_reset(circular_buf_handle_t cbuf);

///Put data into the circular buffer
///Continually overwrites old data when buffer is full
void circular_buf_put(circular_buf_handle_t cbuf, uint8_t data);

///Put version that doesn't overwrite
///Returns 0 on success, -1 if buffer full
int circular_buf_put2(circular_buf_handle_t cbuf, uint8_t data);

///Get data from the circular buffer
///Returns 0 on success, -1 if buffer empty
int circular_buf_get(circular_buf_handle_t cbuf, uint8_t* data);

///Returns true if the buffer is empty
bool is_circular_buf_empty(circular_buf_handle_t cbuf);

///Returns true if the buffer is full
bool is_circular_buf_full(circular_buf_handle_t cbuf);

///Returns the maximum capacity of the buffer
size_t circular_buf_capacity(circular_buf_handle_t cbuf);

///Returns the current number of elements in the buffer
size_t circular_buf_size(circular_buf_handle_t cbuf);

#endif // CIRCULAR_BUFFER_H