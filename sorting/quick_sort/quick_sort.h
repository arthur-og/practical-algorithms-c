#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include <stddef.h> // For size_t

/*
 * Quick Sort algorithm (in-place)
 *
 * Params:
 *     arr   - pointer to the array
 *     begin - start index of the subarray
 *     end   - end index of the subarray
 *
 * Description:
 *     Sorts an array of integers using the Quick Sort algorithm
 *     with randomized pivot selection. Recursively sorts elements
 *     before and after the pivot in-place.
 */
void quick_sort(int arr[], size_t begin, size_t end);

#endif // QUICK_SORT_H
