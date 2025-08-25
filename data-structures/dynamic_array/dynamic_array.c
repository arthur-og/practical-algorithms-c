// src/dynamic_array.c
#include "dynamic_array.h"

#include <assert.h>
#include <limits.h>
#include <stdalign.h> /* for alignas and max_align_t */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/*
 * Implementation notes / invariants:
 *  - DynamicArray stores raw bytes, no element constructors/destructors.
 *  - If elem_size <= DA_INLINE_BYTES, the buffer initially points to sbo (no
 * heap alloc).
 *  - da_data() may point to sbo or to heap; any mutating operation that grows
 * or shrinks the storage can invalidate pointers previously obtained from
 * da_data().
 *  - The API is NOT thread-safe. Caller must synchronize concurrent access.
 */

struct DynamicArray {
  size_t size;      /* number of elements currently stored */
  size_t capacity;  /* capacity in elements */
  size_t elem_size; /* element size in bytes */
  void *data;       /* pointer to storage (either sbo or a heap block) */
  alignas(max_align_t) unsigned char sbo[DA_INLINE_BYTES];
};

/* Helper: check multiplication overflow for a * b */
static inline bool mul_overflow_size_t(size_t a, size_t b) {
  if (a == 0 || b == 0)
    return false;
  return a > SIZE_MAX / b;
}

/* Helper: compute bytes needed for `count` elements, returning DYN_ERR_OVERFLOW
 * on overflow. */
static inline int bytes_for_count(size_t count, size_t elem_size,
                                  size_t *out_bytes) {
  if (mul_overflow_size_t(count, elem_size))
    return DYN_ERR_OVERFLOW;
  *out_bytes = count * elem_size;
  return DYN_OK;
}

/* Create a new dynamic array for elements of size `elem_size`. Returns NULL on
 * error. */
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
    /* start with capacity = 1 for large elements */
    da->capacity = 1;
    if (mul_overflow_size_t(da->capacity, da->elem_size)) {
      free(da);
      return NULL;
    }
    size_t bytes = da->capacity * da->elem_size;
    da->data = malloc(bytes);
    if (!da->data) {
      free(da);
      return NULL;
    }
  }
  return da;
}

/* Destroy the array and free any heap memory. Passing NULL is safe. */
void da_destroy(DynamicArray *da) {
  if (!da)
    return;
  if (da->data && da->data != da->sbo)
    free(da->data);
  free(da);
}

/* Observers */
size_t da_size(const DynamicArray *da) { return da ? da->size : 0; }
size_t da_capacity(const DynamicArray *da) { return da ? da->capacity : 0; }
size_t da_elem_size(const DynamicArray *da) { return da ? da->elem_size : 0; }
void *da_data(DynamicArray *da) { return da ? da->data : NULL; }
const void *da_cdata(const DynamicArray *da) { return da ? da->data : NULL; }

/* Reset logical size to zero. Does not free memory or call destructors. */
void da_clear(DynamicArray *da) {
  if (!da)
    return;
  da->size = 0;
}

/*
 * Ensure capacity >= min_capacity (in elements).
 * May allocate/move storage. Returns DYN_OK, or an error code.
 */
int da_reserve(DynamicArray *da, size_t min_capacity) {
  if (!da)
    return DYN_ERR_INVAL;
  if (min_capacity <= da->capacity)
    return DYN_OK;

  /* compute new capacity by doubling until it meets min_capacity */
  size_t new_cap = da->capacity ? da->capacity : 1;
  while (new_cap < min_capacity) {
    if (new_cap > SIZE_MAX / 2) { /* avoid overflow when doubling */
      new_cap = min_capacity;
      break;
    }
    new_cap *= 2;
  }

  /* check multiplication for bytes */
  if (mul_overflow_size_t(new_cap, da->elem_size))
    return DYN_ERR_OVERFLOW;
  size_t new_bytes = new_cap * da->elem_size;

  if (da->data == da->sbo) {
    /* move from SBO to heap */
    void *heap = malloc(new_bytes);
    if (!heap)
      return DYN_ERR_OOM;
    /* copy the currently used bytes (size * elem_size) */
    size_t used_bytes;
    int st = bytes_for_count(da->size, da->elem_size, &used_bytes);
    if (st != DYN_OK) {
      free(heap);
      return st;
    }
    if (used_bytes)
      memcpy(heap, da->sbo, used_bytes);
    da->data = heap;
    da->capacity = new_cap;
    return DYN_OK;
  } else {
    /* heap -> realloc (preserve old block if realloc fails) */
    void *tmp = realloc(da->data, new_bytes);
    if (!tmp)
      return DYN_ERR_OOM;
    da->data = tmp;
    da->capacity = new_cap;
    return DYN_OK;
  }
}

/*
 * Shrink storage to fit `size`. If size fits the SBO, move data to sbo and free
 * heap. If size==0 free heap and set capacity to SBO-based capacity or 1.
 */
int da_shrink_to_fit(DynamicArray *da) {
  if (!da)
    return DYN_ERR_INVAL;
  if (da->data == da->sbo)
    return DYN_OK; /* already SBO */

  if (da->size == 0) {
    free(da->data);
    da->data = da->sbo;
    da->capacity = (da->elem_size <= DA_INLINE_BYTES)
                       ? (DA_INLINE_BYTES / da->elem_size)
                       : 1;
    if (da->capacity == 0)
      da->capacity = 1;
    return DYN_OK;
  }

  if (mul_overflow_size_t(da->size, da->elem_size))
    return DYN_ERR_OVERFLOW;
  size_t need_bytes = da->size * da->elem_size;

  if (need_bytes <= DA_INLINE_BYTES) {
    /* move into SBO and free heap */
    memcpy(da->sbo, da->data, need_bytes);
    free(da->data);
    da->data = da->sbo;
    da->capacity = (DA_INLINE_BYTES / da->elem_size);
    if (da->capacity == 0)
      da->capacity = 1;
    return DYN_OK;
  } else {
    /* shrink heap block to exactly the needed bytes */
    void *tmp = realloc(da->data, need_bytes);
    if (!tmp)
      return DYN_ERR_OOM;
    da->data = tmp;
    da->capacity = da->size;
    return DYN_OK;
  }
}

/*
 * Append an element to the end (copy elem_size bytes). Amortized O(1).
 */
int da_push_back(DynamicArray *da, const void *elem) {
  if (!da || !elem)
    return DYN_ERR_INVAL;

  if (da->capacity == 0) {
    int st = da_reserve(da, 1);
    if (st != DYN_OK)
      return st;
  }

  if (da->size >= da->capacity) {
    /* compute desired capacity (double or +1 to avoid zero) */
    size_t want =
        (da->capacity > SIZE_MAX / 2) ? (da->capacity + 1) : (da->capacity * 2);
    if (want < da->capacity + 1)
      want = da->capacity + 1;
    int st = da_reserve(da, want);
    if (st != DYN_OK)
      return st;
  }

  /* compute offset bytes and copy */
  if (mul_overflow_size_t(da->size, da->elem_size))
    return DYN_ERR_OVERFLOW;
  size_t offset = da->size * da->elem_size;
  unsigned char *base = (unsigned char *)da->data;
  memcpy(base + offset, elem, da->elem_size);
  ++da->size;
  return DYN_OK;
}

/*
 * Remove last element. If `out` is non-NULL, copy the removed element into it.
 * Returns DYN_OK on success or DYN_ERR_RANGE if array is empty.
 */
int da_pop_back(DynamicArray *da, void *out) {
  if (!da)
    return DYN_ERR_INVAL;
  if (da->size == 0)
    return DYN_ERR_RANGE;

  size_t last_index = da->size - 1;
  unsigned char *base = (unsigned char *)da->data;
  void *src = base + (last_index * da->elem_size);
  if (out)
    memcpy(out, src, da->elem_size);
  da->size = last_index;
  return DYN_OK;
}

/*
 * Return pointer to last element (mutable) or NULL if empty.
 * Note: pointer may be invalidated by subsequent mutating operations.
 */
void *da_back(DynamicArray *da) {
  if (!da || da->size == 0)
    return NULL;
  unsigned char *base = (unsigned char *)da->data;
  return base + ((da->size - 1) * da->elem_size);
}
