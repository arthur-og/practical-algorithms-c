#include "selection_sort.h"
#include <assert.h>
#include <stdbool.h>

/* Swap two integers in memory */
static inline void swap(int *a, int *b) {
  assert(a != NULL && b != NULL);
  int temp = *a;
  *a = *b;
  *b = temp;
}

/* Selection Sort implementation */
void selection_sort(int arr[], size_t len) {
  for (size_t i = 0; i + 1 < len; ++i) {
    size_t min_idx = i;

    // Find the index of the minimum element in arr[i .. len-1]
    for (size_t j = i + 1; j < len; ++j) {
      if (arr[j] < arr[min_idx]) {
        min_idx = j;
      }
    }

    // Swap the current element with the smallest found element (if necessary)
    if (min_idx != i) {
      swap(&arr[i], &arr[min_idx]);
    }
  }
}
