// include/dynamic_array.h
#pragma once
#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Ajuste se quiser mais/menos bytes no SBO.
#ifndef DA_INLINE_BYTES
#define DA_INLINE_BYTES 256
#endif

typedef struct DynamicArray DynamicArray;

typedef enum {
  DYN_OK = 0,
  DYN_ERR_OOM = -1,
  DYN_ERR_OVERFLOW = -2,
  DYN_ERR_INVAL = -3,
  DYN_ERR_RANGE = -4
} da_status;

// Cria/destroi
DynamicArray *da_create(size_t elem_size);
void da_destroy(DynamicArray *da);

// Capacidade/estado
size_t da_size(const DynamicArray *da);
size_t da_capacity(const DynamicArray *da);
size_t da_elem_size(const DynamicArray *da);
void da_clear(DynamicArray *da);

// Crescimento e redução
int da_reserve(DynamicArray *da, size_t min_capacity);
int da_shrink_to_fit(DynamicArray *da);

// Inserção
int da_push_back(DynamicArray *da, const void *elem);

// Acesso bruto ao buffer (para I/O eficiente: fwrite/fread)
void *da_data(DynamicArray *da);
const void *da_cdata(const DynamicArray *da);

#ifdef __cplusplus
}
#endif
