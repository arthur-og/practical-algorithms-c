#include <stddef.h>
#include <stdio.h>

#define VECTOR_INLINE_CAPACITY 256

typedef struct {
  size_t size;
  size_t capacity;
  int *data;
  int buffer[VECTOR_INLINE_CAPACITY];
} Vector;

void vector_init(Vector *v) {
  v->size = 0;
  v->capacity = VECTOR_INLINE_CAPACITY;
  v->data = v->buffer;
}

int main() {}
