#include "bits/types/siginfo_t.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static inline void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

size_t Lomuto(int arr[], size_t begin, size_t end) {
  int pivo = arr[end];
  int pivo_idx = begin;

  for (size_t i = begin; i < end; ++i) {
    if (arr[i] <= pivo) {
      swap(&arr[i], &arr[pivo_idx]);
      ++pivo_idx;
    }
  }
  swap(&arr[pivo_idx], &arr[end]);
  return pivo_idx;
}

size_t random_Lomuto(int arr[], size_t begin, size_t end) {
  size_t pivo_idx = (rand() % (end - begin + 1)) + begin;
  // printf("%zu -- ", pivo_idx);
  swap(&arr[pivo_idx], &arr[end]);
  return Lomuto(arr, begin, end);
}

void quick_sort(int arr[], size_t begin, size_t end) {
  if (begin < end) {
    size_t pivo_idx = random_Lomuto(arr, begin, end);
    quick_sort(arr, begin, (pivo_idx > 1) ? pivo_idx - 1 : 0);
    quick_sort(arr, pivo_idx + 1, end);
  }
}

int main() {
  int arr[] = {12, 43, 1, 2, 4, 56, 0, 0, 1, 3, -8};
  size_t len = sizeof(arr) / sizeof(arr[0]);

  for (size_t i = 0; i < len; ++i) {
    printf("%d ", arr[i]);
  }
  printf("\n");

  quick_sort(arr, 0, len - 1);

  for (size_t i = 0; i < len; ++i) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}
