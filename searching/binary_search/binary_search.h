#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include <stddef.h> // For size_t, ptrdiff_t

/*
 * Binary Search
 *
 * Params:
 *     array - pointer to the first element of a sorted array
 *     len   - number of elements in the array
 *     num   - value to search for
 *
 * Returns:
 *     Index of the element if found (as ptrdiff_t), or -1 if not found
 *
 * Description:
 *     Implements an iterative binary search on a sorted array of integers.
 *     The array must be sorted in ascending order.
 */
ptrdiff_t binary_search(const int array[], size_t len, int num);

#endif // BINARY_SEARCH_H
