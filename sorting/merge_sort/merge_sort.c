#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void merge(int *arr, size_t begin, size_t mid, size_t end) {
  int *temp_buffer = malloc((end - begin + 1) * sizeof(int));
  printf("%zu %zu %zu\n", begin, mid, end);
  size_t first_arr = begin, second_arr = mid + 1;
  for (size_t i = 0; i < end - begin + 1; ++i) {
    if ((first_arr <= mid && arr[first_arr] <= arr[second_arr]) ||
        second_arr > end) {
      temp_buffer[i] = arr[i];
      arr[i] = arr[first_arr];
      ++first_arr;
    } else {
      temp_buffer[i] = arr[i];
      arr[i] = arr[second_arr];
      ++second_arr;
    }
  }
}

void merge_sort(int *arr, size_t begin, size_t end) {
  if (begin < end) {
    size_t mid = begin + (end - begin) / 2;

    merge_sort(arr, begin, mid);
    merge_sort(arr, mid + 1, end);
    merge(arr, begin, mid, end);
  }
}

int main() {
  int arr[] = {1, 2, 5, 10, 2, 33, 0, 4};
  size_t len = sizeof(arr) / sizeof(int);

  printf("Array original: ");
  for (size_t i = 0; i < len; ++i)
    printf("%d ", arr[i]);
  printf("\n");

  merge_sort(arr, 0, len - 1);

  printf("Array ordenado: ");
  for (size_t i = 0; i < len; ++i)
    printf("%d ", arr[i]);
  printf("\n");

  return 0;
}
