#include "dynamic_array.h"
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>

struct DynamicArray {
  size_t size;
  size_t capacity;
  size_t elem_size;
  void *data;
  alignas(max_align_t) unsigned char sbo[DA_INLINE_BYTES];
};

static inline bool mul_overflow_size_t(size_t a, size_t b) {
  if (a == 0 || b == 0)
    return false;
  return a > SIZE_MAX / b;
}

DynamicArray *da_create(size_t elem_size) {
  if (elem_size == 0)
    return NULL;

  DynamicArray *da = (DynamicArray *)malloc(sizeof(*da));

  if (!da)
    return NULL;

  da->size = 0;
  da->elem_size = elem_size;

  if (elem_size <= DA_INLINE_BYTES) {
    size_t cap = DA_INLINE_BYTES / elem_size;
    if (cap == 0)
      cap = 1;
    da->capacity = cap;
    da->data = da->sbo;
  } else {
    da->capacity = 1;
    if (mul_overflow_size_t(da->capacity, da->elem_size)) {
      free(da);
      return NULL;
    }
    da->data = malloc(elem_size * da->capacity);
    if (!da->data) {
      free(da);
      return NULL;
    }
  }
  return da;
}

void da_destroy(DynamicArray *da) {
  if (!da)
    return;
  if (da->data && da->data != da->sbo)
    free(da->data);
  free(da);
}

size_t da_size(const DynamicArray *da) { return da ? da->size : 0; }
size_t da_capacity(const DynamicArray *da) { return da ? da->capacity : 0; }
size_t da_elem_size(const DynamicArray *da) { return da ? da->elem_size : 0; }
void *da_data(DynamicArray *da) { return da ? da->data : NULL; }
const void *da_cdata(const DynamicArray *da) { return da ? da->data : NULL; }

void da_clear(DynamicArray *da) {
  if (!da)
    return;
  da->size = 0;
}

int da_reserve(DynamicArray *da, size_t min_capacity) {
  if (!da)
    return DYN_ERR_INVAL;

  if (min_capacity <= da->capacity)
    return DYN_OK;

  size_t new_cap = da->capacity ? da->capacity : 1;

  while (new_cap < min_capacity) {

    if (new_cap > SIZE_MAX / 2) {
      new_cap = min_capacity;
      break;
    }
    new_cap *= 2;
  }

  if (mul_overflow_size_t(new_cap, da->elem_size))
    return DYN_ERR_OVERFLOW;
  size_t new_bytes = new_cap * da->elem_size;

  if (da->data == da->sbo) {
    void *heap = malloc(new_bytes);
    if (!heap)
      return DYN_ERR_OOM;

    memcpy(heap, da->sbo, da->size * da->elem_size);
    da->data = heap;
    da->capacity = new_cap;
    return DYN_OK;
  } else {
    void *tmp = realloc(da->data, new_bytes);
    if (!tmp)
      return DYN_ERR_OOM;
    da->data = tmp;
    da->capacity = new_cap;
    return DYN_OK;
  }
}

int da_shrink_to_fit(DynamicArray *da) {
  if (!da)
    return DYN_ERR_INVAL;

  if (da->data == da->sbo)
    return DYN_OK;

  if (da->size == 0) {
    free(da->data);
    da->capacity = (da->elem_size <= DA_INLINE_BYTES)
                       ? (DA_INLINE_BYTES / da->elem_size)
                       : 1;
    if (da->capacity == 0)
      da->capacity = 1;
    return DYN_OK;
  }

  if (mul_overflow_size_t(da->size, da->elem_size))
    return DYN_ERR_OVERFLOW;

  size_t need = da->size * da->elem_size;

  if (need <= DA_INLINE_BYTES) {
    memcpy(da->sbo, da->data, need);
    free(da->data);
    da->data = da->sbo;
    da->capacity = (DA_INLINE_BYTES / da->elem_size);
    if (da->capacity == 0)
      da->capacity = 1;
    return DYN_OK;
  } else {
    void *tmp = realloc(da->data, need);
    if (!tmp)
      return DYN_ERR_OOM;

    da->data = tmp;
    da->capacity = da->size;
    return DYN_OK;
  }
}

int da_push_back(DynamicArray *da, const void *elem) {
  if (!da || !elem)
    return DYN_ERR_INVAL;
  if (da->capacity == 0) {
    int st = da_reserve(da, 1);
    if (st != DYN_OK)
      return st;
  }
  if (da->size >= da->capacity) {
    size_t want =
        (da->capacity > SIZE_MAX / 2) ? (da->capacity + 1) : (da->capacity * 2);
    if (want < da->capacity + 1)
      want = da->capacity + 1;
    int st = da_reserve(da, want);
    if (st != DYN_OK)
      return st;
  }
  unsigned char *base = (unsigned char *)da->data;
  size_t offset;
  if (mul_overflow_size_t(da->size, da->elem_size))
    return DYN_ERR_OVERFLOW;
  offset = da->size * da->elem_size;

  memcpy(base + offset, elem, da->elem_size);
  ++da->size;
  return DYN_OK;
}
