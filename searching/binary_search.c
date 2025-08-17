#include <stddef.h>
#include <stdio.h>

ptrdiff_t finder(const int array[], size_t len, int num) {
  size_t begin = 0, end = len;
  while (begin < end) {
    int mid = begin + (end - begin) / 2;
    int attpt = array[mid];
    if (attpt == num)
      return (ptrdiff_t)mid;

    if (attpt < num)
      begin = mid + 1;
    else
      end = mid;
  }
  return -1;
}

int main() {
  const int array[] = {1, 2, 3, 4, 5, 6};

  size_t len = sizeof(array) / sizeof(int);

  printf("%td\n", finder(array, len, 12));

  return 0;
}
