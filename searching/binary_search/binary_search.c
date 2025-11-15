#include "binary_search.h"
#include <stddef.h>
#include <string.h>
/*
 * Iterative binary search implementation
 */
ptrdiff_t binary_search(void *begin, size_t len, size_t elem_size,
                        const void *element, compfunc comp) {
  void *end = begin + elem_size * len;

  while (begin < end) {
    void *mid = begin + (end - begin) / 2;
    void *attpt;
    memcpy(attpt, mid, len);

    if (comp(attpt, element) == 0)
      return (ptrdiff_t)mid; // Element found, return index

    if (comp(attpt, element) == 1)
      begin = mid + 1; // Search right half
    else
      end = mid; // Search left half
  }

  return -1; // Element not found
}
