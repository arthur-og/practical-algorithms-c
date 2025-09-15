#ifndef SELECTION_SORT_H
#define SELECTION_SORT_H

#include <stddef.h> // For size_t

/*
 * Swap two integers in memory
 *
 * Params:
 *     a - pointer to first integer
 *     b - pointer to second integer
 *
 * Description:
 *     Exchanges the values stored at the addresses provided.
 *     The function asserts that the pointers are valid.
 */
static inline void swap(int *a, int *b);

/*
 * In-place Selection Sort
 *
 * Params:
 *     arr - pointer to the first element of the array
 *     len - number of elements in the array
 *
 * Complexity:
 *   - Comparisons: ~ n*(n-1)/2  (O(n^2))
 *   - Swaps: at most n-1         (O(n))
 *
 * Description:
 *     Partition the array into a sorted prefix and an unsorted suffix.
 *     For each i from 0 to n-2, find the minimum element in arr[i..n-1]
 *     and place it at arr[i].
 */
void selection_sort(int arr[], size_t len);

#endif // SELECTION_SORT_H
