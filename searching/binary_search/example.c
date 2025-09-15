#include "binary_search.h"
#include <stdio.h>

/*
 * Example program demonstrating Binary Search.
 * User inputs a number to search for in a sorted array.
 * Program prints the index of the element if found, or -1 if not.
 */
int main() {
  // Example sorted array
  const int array[] = {1, 2, 3, 4, 5, 6};
  size_t len = sizeof(array) / sizeof(array[0]);

  int num;

  // Ask user for the number to search
  printf("Enter the number to search for: ");
  scanf("%d", &num);

  // Perform binary search
  ptrdiff_t index = binary_search(array, len, num);

  // Print the result
  if (index >= 0)
    printf("Number %d found at index: %td\n", num, index);
  else
    printf("Number %d not found in the array.\n", num);

  return 0;
}
