#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void insertion_sort(void *base, size_t len, size_t size,
                    int (*cmp)(const void *, const void *)) {
  char *array = base;
  char *key = malloc(size);
  if (!key)
    return;

  for (size_t i = 1; i < len; ++i) {
    char *curr = array + i * size;
    memcpy(key, curr, size);

    size_t j = i;
    while (j > 0 && cmp(array + (j - 1) * size, key) > 0) {
      j--;
    }

    // Desloca todo o bloco [j, i-1] uma posição para frente
    memmove(array + (j + 1) * size, array + j * size, (i - j) * size);

    // Insere a chave
    memcpy(array + j * size, key, size);
  }

  free(key);
}
