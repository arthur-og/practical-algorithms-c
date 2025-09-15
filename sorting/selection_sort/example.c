#include "selection_sort.h" // Include the header for the Selection Sort library
#include <stdio.h>          // Include standard I/O functions (printf, scanf)

/*
 * Example program demonstrating the use of Selection Sort.
 * The user inputs the size of the array and its elements.
 * The program sorts the array in-place and prints before and after.
 */
int main() {
  size_t len; // Variable to store the number of elements in the array

  // Prompt the user to enter the array size
  printf("Enter the number of elements: ");
  scanf("%zu", &len); // Read the size from standard input

  // Declare the array dynamically with the specified length
  int array[len];

  // Prompt the user to enter the array elements
  printf("Enter %zu integers:\n", len);
  for (size_t i = 0; i < len; ++i) {
    scanf("%d", &array[i]); // Read each integer from input
  }

  // Print the array before sorting
  printf("Before sorting:\n");
  for (size_t i = 0; i < len; ++i) {
    printf("%d ", array[i]);
  }
  printf("\n");

  // Sort the array using Selection Sort (library function)
  selection_sort(array, len);

  // Print the array after sorting
  printf("After sorting:\n");
  for (size_t i = 0; i < len; ++i) {
    printf("%d ", array[i]);
  }
  printf("\n");

  return 0; // Exit the program successfully
}
