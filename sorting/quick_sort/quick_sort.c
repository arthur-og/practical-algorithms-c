#include "quick_sort.h"
#include <assert.h>
#include <stdlib.h> // for rand()

/*
 * Swap two integers in memory
 *
 * Params:
 *     a - pointer to first integer
 *     b - pointer to second integer
 *
 * Description:
 *     Exchanges the values stored at the addresses provided.
 */
static inline void swap(int *a, int *b) {
  assert(a != NULL && b != NULL);
  int temp = *a;
  *a = *b;
  *b = temp;
}

/*
 * Lomuto partition scheme
 *
 * Partitions a subarray around a pivot (last element).
 * Elements <= pivot go left, > pivot go right.
 *
 * Returns:
 *     Final index of the pivot
 */
size_t Lomuto(int arr[], size_t begin, size_t end) {
  int pivot = arr[end];
  size_t pivot_idx = begin;

  for (size_t i = begin; i < end; ++i) {
    if (arr[i] <= pivot) {
      swap(&arr[i], &arr[pivot_idx]);
      ++pivot_idx;
    }
  }

  swap(&arr[pivot_idx], &arr[end]);
  return pivot_idx;
}

/*
 * Randomized Lomuto partition
 *
 * Chooses a random pivot to reduce worst-case performance on sorted arrays.
 */
size_t random_Lomuto(int arr[], size_t begin, size_t end) {
  size_t pivot_idx = (rand() % (end - begin + 1)) + begin;
  swap(&arr[pivot_idx], &arr[end]);
  return Lomuto(arr, begin, end);
}

/*
 * Quick Sort implementation
 *
 * Recursively sorts subarrays before and after a randomly chosen pivot.
 */
void quick_sort(int arr[], size_t begin, size_t end) {
  if (begin < end) {
    size_t pivot_idx = random_Lomuto(arr, begin, end);

    // Sort left subarray
    quick_sort(arr, begin, (pivot_idx > 0) ? pivot_idx - 1 : 0);

    // Sort right subarray
    quick_sort(arr, pivot_idx + 1, end);
  }
}
