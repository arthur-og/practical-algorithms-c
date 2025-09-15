#include "quick_sort.h"
#include <stdio.h>

/*
 * Example program demonstrating Quick Sort.
 * The user inputs the array size and elements.
 * The program sorts the array in-place and prints before and after.
 */
int main() {
  size_t len;

  // Ask the user for the number of elements
  printf("Enter the number of elements: ");
  scanf("%zu", &len);

  // Declare the array dynamically
  int array[len];

  // Read array elements from user input
  printf("Enter %zu integers:\n", len);
  for (size_t i = 0; i < len; ++i) {
    scanf("%d", &array[i]);
  }

  // Print array before sorting
  printf("Before sorting:\n");
  for (size_t i = 0; i < len; ++i) {
    printf("%d ", array[i]);
  }
  printf("\n");

  // Sort the array using Quick Sort
  quick_sort(array, 0, len - 1);

  // Print array after sorting
  printf("After sorting:\n");
  for (size_t i = 0; i < len; ++i) {
    printf("%d ", array[i]);
  }
  printf("\n");

  return 0;
}
