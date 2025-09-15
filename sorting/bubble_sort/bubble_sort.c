#include "bubble_sort.h"
#include <string.h> // for memcpy

// Generic swap implementation
void swap(void *a, void *b, size_t size) {
  unsigned char tmp[size]; // temporary buffer
  memcpy(tmp, a, size);    // copy first element to temp
  memcpy(a, b, size);      // copy second element to first
  memcpy(b, tmp, size);    // copy temp to second element
}

// Generic bubble sort implementation
void bubble_sort(void *arr, size_t len, size_t size,
                 int (*cmp)(const void *, const void *)) {
  char *base = (char *)arr; // convert to char* for pointer arithmetic

  for (size_t i = 0; i < len; ++i) {
    for (size_t j = 0; j < len - i - 1; ++j) {
      void *elem1 = base + j * size;
      void *elem2 = base + (j + 1) * size;

      // Use comparison function to decide if swap is needed
      if (cmp(elem1, elem2) > 0) {
        swap(elem1, elem2, size);
      }
    }
  }
}
