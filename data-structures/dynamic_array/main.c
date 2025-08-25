// app.c
#include "dynamic_array.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  DynamicArray *a = da_create(sizeof(char));
  assert(a);

  const char *msg = "abcde";
  for (const char *p = msg; *p; ++p) {
    int st = da_push_back(a, p);
    assert(st == DYN_OK);
  }

  fwrite(da_cdata(a), da_elem_size(a), da_size(a), stdout); // imprime abcde
  da_destroy(a);
  return 0;
}
