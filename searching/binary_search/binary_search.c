#include "binary_search.h"

/*
 * Iterative binary search implementation
 */
ptrdiff_t binary_search(const int array[], size_t len, int num) {
  size_t begin = 0;
  size_t end = len;

  while (begin < end) {
    size_t mid = begin + (end - begin) / 2;
    int attpt = array[mid];

    if (attpt == num)
      return (ptrdiff_t)mid; // Element found, return index

    if (attpt < num)
      begin = mid + 1; // Search right half
    else
      end = mid; // Search left half
  }

  return -1; // Element not found
}
