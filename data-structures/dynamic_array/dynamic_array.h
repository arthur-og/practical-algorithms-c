// include/dynamic_array.h
#pragma once
#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file dynamic_array.h
 * @brief Minimal, C11-compatible dynamic array with small-buffer optimization
 * (SBO).
 *
 * Key features:
 *  - Opaque handle (`DynamicArray`) — implementation details hidden.
 *  - Small-buffer optimization (inline storage up to DA_INLINE_BYTES).
 *  - Safe overflow checks for size * elem_size.
 *  - Basic operations: create/destroy, push_back, pop_back, back, reserve,
 *    shrink_to_fit, clear.
 *
 * Thread-safety: not thread-safe. Concurrent access must be synchronized by
 * caller.
 */

/** Inline capacity in bytes for Small Buffer Optimization (SBO).
 *
 * If an element's size is <= DA_INLINE_BYTES then the initial capacity will be
 * (DA_INLINE_BYTES / elem_size), and storage will be placed inside the struct
 * (no initial heap allocation). Adjust at compile time before including the
 * header.
 */
#ifndef DA_INLINE_BYTES
#define DA_INLINE_BYTES 256
#endif

typedef struct DynamicArray DynamicArray;

/** Status codes returned by functions. Negative values indicate failure. */
typedef enum {
  DYN_OK = 0,            /**< success */
  DYN_ERR_OOM = -1,      /**< out of memory / allocation failed */
  DYN_ERR_OVERFLOW = -2, /**< arithmetic overflow (size * elem_size) */
  DYN_ERR_INVAL =
      -3, /**< invalid argument (NULL pointer, zero elem_size, etc.) */
  DYN_ERR_RANGE =
      -4 /**< index out of range (used by e.g. da_pop_back on empty) */
} da_status;

/* -------------------------
 * Construction / Destruction
 * ------------------------- */

/**
 * @brief Create a dynamic array for elements of size @p elem_size.
 *
 * @param elem_size Size in bytes of a single element (must be > 0).
 * @return Pointer to a newly allocated DynamicArray on success, or NULL on
 * error.
 *
 * Ownership: the caller owns the returned pointer and must call da_destroy().
 */
DynamicArray *da_create(size_t elem_size);

/**
 * @brief Destroy a dynamic array, freeing any heap memory it used.
 *
 * @param da Pointer previously returned by da_create. Passing NULL is safe.
 *
 * Note: this does not call any element destructors — if elements own resources,
 * the caller must release them before calling da_destroy().
 */
void da_destroy(DynamicArray *da);

/* -------------------------
 * Observers / Properties
 * ------------------------- */

/**
 * @brief Number of elements currently stored.
 * @return size in element units (0 if da == NULL).
 */
size_t da_size(const DynamicArray *da);

/**
 * @brief Current capacity in element units.
 * @return capacity (0 if da == NULL).
 */
size_t da_capacity(const DynamicArray *da);

/**
 * @brief Size of one element in bytes.
 * @return elem_size (0 if da == NULL).
 */
size_t da_elem_size(const DynamicArray *da);

/**
 * @brief Reset length to zero (keeps capacity and contents intact).
 *
 * This is O(1). It does not free memory or call destructors for elements.
 */
void da_clear(DynamicArray *da);

/* -------------------------
 * Capacity management
 * ------------------------- */

/**
 * @brief Ensure the array has capacity for at least min_capacity elements.
 *
 * @param da Dynamic array pointer.
 * @param min_capacity Requested minimum capacity (in elements).
 * @return DYN_OK on success, DYN_ERR_OOM if allocation failed,
 *         DYN_ERR_OVERFLOW if arithmetic overflow detected,
 *         DYN_ERR_INVAL if da == NULL.
 *
 * Complexity: potentially O(n) due to reallocation / memcpy.
 */
int da_reserve(DynamicArray *da, size_t min_capacity);

/**
 * @brief Reduce heap allocation to match current size, using SBO if possible.
 *
 * If current size in bytes fits in DA_INLINE_BYTES, the contents are moved to
 * inline storage and the heap buffer is freed. If size is zero the heap buffer
 * is freed and capacity is set to the SBO-derived capacity or 1.
 *
 * @return DYN_OK on success, DYN_ERR_OOM if realloc failed, DYN_ERR_OVERFLOW if
 *         arithmetic overflow detected, DYN_ERR_INVAL if da == NULL.
 */
int da_shrink_to_fit(DynamicArray *da);

/* -------------------------
 * Element insertion / access
 * ------------------------- */

/**
 * @brief Append an element to the end of the array.
 *
 * Copies elem_size bytes from @p elem into the array.
 *
 * @param da Dynamic array pointer (must be non-NULL).
 * @param elem Pointer to an element-sized buffer (must be non-NULL).
 * @return DYN_OK on success, DYN_ERR_OOM if allocation failed,
 *         DYN_ERR_OVERFLOW if arithmetic overflow detected,
 *         DYN_ERR_INVAL if invalid args.
 *
 * Complexity: amortized O(1). A single push may trigger O(n) reallocation.
 *
 * Notes:
 *  - If a reallocation occurs, any pointer previously obtained via da_data()
 *    becomes invalid.
 *  - This function does a byte-wise copy (memcpy). For elements that require
 *    construction/destruction semantics, the caller must manage resources.
 */
int da_push_back(DynamicArray *da, const void *elem);

/**
 * @brief Remove the last element.
 *
 * If @p out is non-NULL, the removed element's bytes are copied into @p out
 * (elem_size bytes). If the array is empty, returns DYN_ERR_RANGE.
 *
 * @return DYN_OK on success, DYN_ERR_RANGE if the array is empty,
 *         DYN_ERR_INVAL if da == NULL.
 */
int da_pop_back(DynamicArray *da, void *out);

/**
 * @brief Return a pointer to the last element (mutable) or NULL if empty.
 *
 * The pointer may point into SBO or heap memory and can be invalidated by
 * subsequent mutating operations (da_push_back, da_reserve, da_shrink_to_fit).
 */
void *da_back(DynamicArray *da);

/**
 * @brief Returns a pointer to the underlying contiguous buffer (mutable).
 *
 * The returned pointer points to valid storage for da_size() elements.
 * The pointer may reference the internal small buffer (SBO) or heap memory.
 *
 * @warning The pointer can become invalid after any mutating operation that
 * performs reallocation: da_push_back, da_reserve, da_shrink_to_fit, etc.
 */
void *da_data(DynamicArray *da);

/**
 * @brief Returns a const pointer to the underlying buffer.
 */
const void *da_cdata(const DynamicArray *da);

#ifdef __cplusplus
}
#endif
