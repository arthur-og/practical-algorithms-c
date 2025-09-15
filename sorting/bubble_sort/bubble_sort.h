#ifndef BUBBLE_SORT_H
#define BUBBLE_SORT_H

#include <stddef.h> // for size_t

/**
 * @brief Generic bubble sort function.
 *
 * @param arr Pointer to the array to sort
 * @param len Number of elements in the array
 * @param size Size of each element in bytes
 * @param cmp Pointer to a comparison function
 *
 * The comparison function should return:
 *   >0 if first element is greater than second
 *    0 if they are equal
 *   <0 if first element is less than second
 *
 * This allows sorting arrays of any type.
 */
void bubble_sort(void *arr, size_t len, size_t size,
                 int (*cmp)(const void *, const void *));

#endif // BUBBLE_SORT_H
