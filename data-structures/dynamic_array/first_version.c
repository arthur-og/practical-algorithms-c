#include "string.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define VDynamicArray_INLINE_CAPACITY 4

typedef struct {
  size_t size;
  size_t capacity;
  size_t elem_size;
  void *data;
  unsigned char buffer[VDynamicArray_INLINE_CAPACITY * sizeof(long long)];
} DynamicArray;

void DynamicArray_init(DynamicArray *v, size_t elem_size) {
  v->size = 0;
  v->capacity = VDynamicArray_INLINE_CAPACITY;
  v->elem_size = elem_size;
  v->data = v->buffer;
}

void push_back(DynamicArray *v, void *value) {
  if (v->size == v->capacity) {
    size_t new_capacity = v->capacity * 2;
    void *new_data;
    if (v->data == v->buffer) {
      new_data = malloc(new_capacity * v->elem_size);
      memcpy(new_data, v->data, v->size * v->elem_size);
    } else {
      new_data = realloc(v->data, new_capacity * v->elem_size);
    }
    if (!new_data)
      exit(1);
    v->data = new_data;
    v->capacity = new_capacity;
  }
  memcpy((char *)v->data + v->size * v->elem_size, value, v->elem_size);
  ++v->size;
}

void DynamicArray_free(DynamicArray *v) {
  if (v->data != v->buffer) {
    free(v->data);
  }
  v->data = NULL;
  v->size = v->capacity = 0;
}
#define DynamicArray_AT(v, type, i) (((type *)(v).data)[(i)])
int main() {

  DynamicArray v;
  DynamicArray_init(&v, sizeof(char));
  char num = 0;
entry:
  scanf("%c", &num);
  if (num != 'p') {
    push_back(&v, &num);
    goto entry;
  }

  for (size_t i = 0; i < v.size; ++i) {
    printf("%c", DynamicArray_AT(v, char, i));
  }
}
