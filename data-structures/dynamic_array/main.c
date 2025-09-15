// app.c
#include "dynamic_array.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  DynamicArray *vec = da_create(sizeof(char));
  assert(vec);

  const char *msg = "abcde";
  for (const char *p = msg; *p; ++p) {
    int st = da_push_back(vec, p);
    assert(st == DYN_OK);
  }

  fwrite(da_cdata(vec), da_elem_size(vec), da_size(vec),
         stdout); // imprime abcde
  da_destroy(vec);
  return 0;
}
