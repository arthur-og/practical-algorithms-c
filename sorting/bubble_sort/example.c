#include "bubble_sort.h"
#include <stdio.h>

// Example integer comparison function
int cmp_int(const void *a, const void *b) {
  int x = *(const int *)a;
  int y = *(const int *)b;
  return (x > y) - (x < y);
}

// Print array
void print_arr(int arr[], size_t len) {
  for (size_t i = 0; i < len; ++i) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

int main() {
  // Example array of integers
  int arr[] = {5, 432, 1, 3, 12, 34, 1, 2, 0, 8};
  size_t len = sizeof(arr) / sizeof(arr[0]);

  // Print array before sorting
  printf("Before sorting:\n");
  print_arr(arr, len);

  // Sort the array using generic bubble sort
  bubble_sort(arr, len, sizeof(arr[0]), cmp_int);

  // Print array after sorting
  printf("After sorting:\n");
  print_arr(arr, len);

  return 0;
}
