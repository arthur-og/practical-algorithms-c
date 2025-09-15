#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H

#include <stddef.h>

/**
 * @brief Sorts an array using the optimized Insertion Sort algorithm.
 *
 * @param base Pointer to the first element of the array.
 * @param len Number of elements in the array.
 * @param size Size of each element in bytes.
 * @param cmp Comparison function to determine the order.
 */
void insertion_sort(void *base, size_t len, size_t size,
                    int (*cmp)(const void *, const void *));

#endif // INSERTION_SORT_H
