#include <stdint.h>
#include <stdio.h>

#include "circular_buffer/circular_buffer.h"

int main() {
  const size_t BUFFER_SIZE = 10;
  uint8_t buffer[BUFFER_SIZE];

  circular_buf_handle_t cbuf = circular_buf_init(buffer, BUFFER_SIZE);

  printf("Buffer capacity:%zu Buffer size:%zu Is buffer empty:%s Is buffer "
         "full:%s\n",
         circular_buf_capacity(cbuf), circular_buf_size(cbuf),
         is_circular_buf_empty(cbuf) ? "true" : "false",
         is_circular_buf_full(cbuf) ? "true" : "false");

  // Fill the buffer using circular_buf_put
  for (uint8_t i = 0; i < BUFFER_SIZE; ++i) {
    circular_buf_put(cbuf, i);
    printf("Buffer put:%u -> size=%zu is buffer full=%s \n", i,
           circular_buf_size(cbuf),
           is_circular_buf_full(cbuf) ? "true" : "false");
  }

  // Attempt to add one more element using circular_buf_put2
  if (circular_buf_put2(cbuf, 99) == -1) {
    printf("Buffer is full, cannot add 99 using circular_buf_put2\n");
  }

  // Retrieve all elements from the buffer
  uint8_t out_data;
  while (circular_buf_get(cbuf, &out_data) == 0) {
    printf("Buffer get:%u -> size=%zu is buffer empty=%s \n", out_data,
           circular_buf_size(cbuf),
           is_circular_buf_empty(cbuf) ? "true" : "false");
  }

  /// Add some elements again to demonstrate wrap-around
  for (uint8_t i = 100; i < 105; ++i) {
    if (circular_buf_put2(cbuf, i) == 0) {
      printf("Put2 %u succeeded \n", i);
    } else {
      printf("Put2 %u failed, buffer full \n", i);
    }
  }
 
  while (circular_buf_get(cbuf, &out_data) == 0) {
    printf("Got %u\n", out_data);
  }

  circular_buf_free(cbuf);

  return 0;
}
