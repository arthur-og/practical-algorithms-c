#include "insertion_sort.h"
#include <stddef.h>
#include <stdio.h>

int cmp_int(const void *a, const void *b) {
  if (*(const int *)a < *(const int *)b)
    return -1;
  if (*(const int *)a > *(const int *)b)
    return 1;
  return 0;
}

void print_arr(int arr[], size_t len) {
  for (size_t i = 0; i < len; ++i) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

int main() {
  int arr[] = {1, 23, 901, 4, 2, 12, 99, 0};
  size_t len = sizeof(arr) / sizeof(arr[0]);

  printf("Original array: ");
  print_arr(arr, len);

  insertion_sort(arr, len, sizeof(arr[0]), cmp_int);

  printf("Sorted array: ");
  print_arr(arr, len);

  return 0;
}
