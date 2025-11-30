#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "circular_buffer.h"

// The hidden definition of the circular buffer structure
struct circular_buf_t {
  uint8_t *buffer_data;
  size_t head_index;
  size_t tail_index;
  size_t max_buffer_capacity;
  bool is_buffer_full;
};

static void advance_pointer(circular_buf_handle_t cbuf);
static void retreat_pointer(circular_buf_handle_t cbuf);

circular_buf_handle_t circular_buf_init(uint8_t *buffer, size_t size) {
  assert(buffer && size);

  circular_buf_handle_t cbuf = malloc(sizeof(circular_buf_t));
  assert(cbuf);

  cbuf->buffer_data = buffer;
  cbuf->max_buffer_capacity = size;
  circular_buf_reset(cbuf);

  assert(is_circular_buf_empty(cbuf));

  return cbuf;
}

void circular_buf_reset(circular_buf_handle_t cbuf) {
  assert(cbuf);

  cbuf->head_index = 0;
  cbuf->tail_index = 0;
  cbuf->is_buffer_full = false;
}

void circular_buf_free(circular_buf_handle_t cbuf) {
  assert(cbuf);
  free(cbuf);
}

bool is_circular_buf_full(circular_buf_handle_t cbuf) {
  assert(cbuf);

  return cbuf->is_buffer_full;
}

bool is_circular_buf_empty(circular_buf_handle_t cbuf) {
  assert(cbuf);

  return (!cbuf->is_buffer_full && (cbuf->head_index == cbuf->tail_index));
}

size_t circular_buf_capacity(circular_buf_handle_t cbuf) {
  assert(cbuf);

  return cbuf->max_buffer_capacity;
}

size_t circular_buf_size(circular_buf_handle_t cbuf) {
  assert(cbuf);

  size_t size = cbuf->max_buffer_capacity;

  if (!cbuf->is_buffer_full) {
    if (cbuf->head_index >= cbuf->tail_index) {
      size = cbuf->head_index - cbuf->tail_index;
    } else {
      size = cbuf->max_buffer_capacity + cbuf->head_index - cbuf->tail_index;
    }
  }

  return size;
}

static void advance_pointer(circular_buf_handle_t cbuf) {
  assert(cbuf);

  if (cbuf->is_buffer_full) {
    if (++(cbuf->tail_index) == cbuf->max_buffer_capacity) {
      cbuf->tail_index = 0;
    }
  }

  if (++(cbuf->head_index) == cbuf->max_buffer_capacity) {
    cbuf->head_index = 0;
  }

  cbuf->is_buffer_full = (cbuf->head_index == cbuf->tail_index);
}

static void retreat_pointer(circular_buf_handle_t cbuf) {
  assert(cbuf);

  cbuf->is_buffer_full = false;
  if (++(cbuf->tail_index) == cbuf->max_buffer_capacity) {
    cbuf->tail_index = 0;
  }
}

void circular_buf_put(circular_buf_handle_t cbuf, uint8_t data) {
  assert(cbuf);

  cbuf->buffer_data[cbuf->head_index] = data;

  advance_pointer(cbuf);
}

/// Returns 0 on success, -1 if buffer full for demonstration purposes
int circular_buf_put2(circular_buf_handle_t cbuf, uint8_t data) {
  int return_value = -1;

  assert(cbuf && cbuf->buffer_data);

  if (!is_circular_buf_full(cbuf)) {
    cbuf->buffer_data[cbuf->head_index] = data;
    advance_pointer(cbuf);
    return_value = 0;
  }

  return return_value;
}

int circular_buf_get(circular_buf_handle_t cbuf, uint8_t *data) {
  assert(cbuf && data && cbuf->buffer_data);

  int return_value = -1;

  if (!is_circular_buf_empty(cbuf)) {
    *data = cbuf->buffer_data[cbuf->tail_index];
    retreat_pointer(cbuf);
    
    return_value = 0;
  }

  return return_value;
}