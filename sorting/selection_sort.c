#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

static inline void swap(int *a, int *b) {
  assert(a != NULL && b != NULL);
  /* Dereference the pointers: the function receives &x and
   * accesses the pointer via *a*/
  int temp = *a;
  *a = *b;
  *b = temp;
} // Swap the integers stored at the two provided addresses
/*
 * In-place selection sort
 *
 * Params:
 *     arr - pointer to the first element
 *     len - number of elements in array
 *
 *Complexity:
 *   - Comparisons: ~ n*(n-1)/2  (O(n^2))
 *   - Swaps: at most n-1         (O(n))
 *
 * Partition the array into a sorted prefix and an unsorted suffix.
 * For each i from 0..n-2, find the minimum element in arr[i..n-1]
 * and place it at arr[i].
 */
void selection_sort(int arr[], size_t len) {
  for (size_t i = 0; i + 1 < len; ++i) {
    size_t min_idx = i;

    // Find the index of the minimum element in arr[i .. len-1]
    for (size_t j = i + 1; j < len; ++j) {
      if (arr[j] < arr[min_idx]) {
        min_idx = j;
      }
    }
    // Place the smallest found element at position i(if necessary)
    if (min_idx != i) {
      swap(&arr[i], &arr[min_idx]);
    }
  }
}

#include <stdio.h>

int main() {
  int array[] = {0, 5, 4, 3, 12, 0, 0, 1, 2};
  size_t len = sizeof(array) / sizeof(int);
  selection_sort(array, len);
  for (int i = 0; i < len; ++i) {
    printf("%d ", array[i]);
  }
  printf("\n");
}
