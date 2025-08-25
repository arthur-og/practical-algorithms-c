# DynamicArray (C, SBO)

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](#)
[![License](https://img.shields.io/badge/license-MIT-blue)](LICENSE)

A minimal, dependency-free **dynamic array** implementation in C with **Small-Buffer Optimization (SBO)**.
Designed for predictable, fast, cache-friendly contiguous storage with safe integer-overflow checks and a tiny, practical API.

---

## Table of contents

* [Highlights](#highlights)
* [Features](#features)
* [When to use this library](#when-to-use-this-library)
* [Repository layout](#repository-layout)
* [Quick start (build & example)](#quick-start-build--example)
* [API overview](#api-overview)
* [Usage examples](#usage-examples)
* [Design & semantics](#design--semantics)
* [Error codes](#error-codes)
* [Testing & benchmarks](#testing--benchmarks)
* [Contributing](#contributing)
* [License](#license)
* [FAQ](#faq)
* [CHANGELOG (starter)](#changelog-starter)

---

## Highlights

* Opaque `DynamicArray` handle (implementation hidden in `.c`).
* Small-Buffer Optimization (SBO) — avoids heap allocation for small arrays.
* Safe multiplication overflow checks (`size * elem_size`).
* Compact, practical API: `create`, `destroy`, `push_back`, `pop_back`, `back`, `reserve`, `shrink_to_fit`, `clear`, accessors.
* No element constructors/destructors — raw `memcpy` semantics; caller manages complex element lifetimes.

---

## Features

* Contiguous memory (excellent cache locality).
* Configurable SBO bytes via `DA_INLINE_BYTES` (default: `256`).
* Doubling growth strategy (amortized O(1) push).
* Optional `pop` out-copy and `back` pointer helpers.
* Minimal C11-only dependencies — portable and embeddable.

---

## When to use this library

Choose `DynamicArray` when you need:

* Fast iteration over contiguous memory.
* A compact growable buffer with low allocation overhead on small sizes.
* A tiny, dependency-free container for serialization, I/O buffers, stacks, or temporary buffers.

Avoid when you need automatic construction/destruction of elements (this library performs raw copies and *caller* must manage resource lifetimes).

---

## Repository layout (recommended)

```
.
├── include/
│   └── dynamic_array.h
├── src/
│   └── dynamic_array.c
├── examples/
│   └── example_int.c
├── tests/
│   └── test_dynamic_array.c
├── Makefile
└── README.md
```

---

## Quick start (build & example)

### Build (static library + example)

```sh
mkdir -p build
# compile library
gcc -std=c11 -O2 -Wall -Wextra -Iinclude -c src/dynamic_array.c -o build/dynamic_array.o
ar rcs build/libdynamic_array.a build/dynamic_array.o

# compile example
gcc -std=c11 -O2 -Iinclude examples/example_int.c build/libdynamic_array.a -o build/example_int

# run example
./build/example_int
```

### Simple Makefile (snippet)

```makefile
CC = gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra -Iinclude
LIB = build/libdynamic_array.a
SRC = src/dynamic_array.c

all: $(LIB) build/example_int

build:
	mkdir -p build

$(LIB): build $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o build/dynamic_array.o
	ar rcs $(LIB) build/dynamic_array.o

build/example_int: build examples/example_int.c $(LIB)
	$(CC) $(CFLAGS) examples/example_int.c $(LIB) -o build/example_int

.PHONY: clean
clean:
	rm -rf build
```

---

## API overview

Header: `include/dynamic_array.h`

Important functions:

```c
DynamicArray *da_create(size_t elem_size);
void da_destroy(DynamicArray *da);

size_t da_size(const DynamicArray *da);
size_t da_capacity(const DynamicArray *da);
size_t da_elem_size(const DynamicArray *da);
void da_clear(DynamicArray *da);

int da_reserve(DynamicArray *da, size_t min_capacity);
int da_shrink_to_fit(DynamicArray *da);

int da_push_back(DynamicArray *da, const void *elem);
int da_pop_back(DynamicArray *da, void *out);       // out may be NULL
void *da_back(DynamicArray *da);                    // returns NULL if empty

void *da_data(DynamicArray *da);                    // mutable pointer
const void *da_cdata(const DynamicArray *da);       // const pointer
```

All functions that can fail return a `da_status` or `NULL` for `da_create` on error. Check return values.

---

## Usage examples

### Example: integers

`examples/example_int.c`:

```c
#include "dynamic_array.h"
#include <stdio.h>

int main(void) {
  DynamicArray *a = da_create(sizeof(int));
  if (!a) return 1;

  for (int i = 0; i < 10; ++i) {
    if (da_push_back(a, &i) != DYN_OK) { da_destroy(a); return 1; }
  }

  int *data = (int *)da_data(a);
  for (size_t i = 0; i < da_size(a); ++i) {
    printf("%d\n", data[i]);
  }

  da_destroy(a);
  return 0;
}
```

### Example: elements owning resources

If your element contains heap-allocated members, free them before `da_clear` / `da_destroy`:

```c
typedef struct { char *name; } Person;

void free_persons(DynamicArray *arr) {
  Person *p = (Person*)da_data(arr);
  for (size_t i = 0; i < da_size(arr); ++i)
    free(p[i].name);
}

int main(void) {
  DynamicArray *arr = da_create(sizeof(Person));
  Person p = { strdup("Alice") };
  da_push_back(arr, &p);

  free_persons(arr);  // free nested resources
  da_destroy(arr);
}
```

### Using `da_pop_back`

```c
int x;
if (da_pop_back(a, &x) == DYN_OK) {
  printf("popped %d\n", x);
}
```

---

## Design & semantics

* **Opaque type**: The `DynamicArray` structure is defined in `src/dynamic_array.c`; header exposes an opaque pointer type.
* **SBO**: `DA_INLINE_BYTES` bytes are embedded in the struct. If `elem_size <= DA_INLINE_BYTES` initial storage is inline (no heap).
* **Growth strategy**: doubling capacity until requested minimum is reached. Overflow checks are performed.
* **Pointer invalidation**: any mutating operation that reallocates memory (push, reserve, shrink\_to\_fit) may invalidate pointers returned by `da_data()` or `da_back()`.
* **No constructors/destructors**: library uses raw `memcpy`. If elements manage resources, caller must handle lifetime.
* **Thread-safety**: not thread-safe. Synchronize externally if needed.

---

## Error codes

The API returns the following `da_status` values:

| Code               | Value | Meaning                                  |
| ------------------ | :---: | ---------------------------------------- |
| `DYN_OK`           |   0   | Success                                  |
| `DYN_ERR_OOM`      |   -1  | Allocation failed                        |
| `DYN_ERR_OVERFLOW` |   -2  | Arithmetic overflow (`size * elem_size`) |
| `DYN_ERR_INVAL`    |   -3  | Invalid parameter (e.g., `NULL`)         |
| `DYN_ERR_RANGE`    |   -4  | Range error (e.g., `pop` on empty)       |

Always check return values for functions that can fail.

---

## Testing & benchmarks

### Suggested tests (add to `tests/`)

* Creation/destruction for element sizes: 1, 4, 16, > `DA_INLINE_BYTES`.
* Push/pop transition (SBO -> heap -> SBO via `shrink_to_fit`).
* `da_reserve` behavior and failure modes (simulate OOM via environment where possible).
* Overflow protection: attempt to reserve extremely large capacities and assert `DYN_ERR_OVERFLOW`.

### Simple test runner

You can create a small test runner in `tests/test_dynamic_array.c` that uses `assert()` to validate behavior; build with the Makefile.

### Benchmarks ideas

* Micro-benchmark `push_back` throughput for element sizes 1..64 and compare to a singly-linked-list stack.
* Measure number of allocations for typical workloads and average wall-clock time per push.

> For reproducible results, compile with `-O2` and use `clock_gettime(CLOCK_MONOTONIC, ...)` to measure.

---

## Contributing

Contributions are welcome.

Guidelines:

1. Fork the repository.
2. Create a feature branch: `git checkout -b feat/your-feature`.
3. Add tests that demonstrate the new behavior or reproduce a bug.
4. Open a PR with description, rationale, and performance notes (if relevant).

Please keep the public API stable; document breaking changes in `CHANGELOG.md`.

---

## License

This project is provided under the **MIT License**. Add a `LICENSE` file with the MIT text:

```
MIT License

Copyright (c) 2025 <Your Name>

Permission is hereby granted, free of charge, to any person obtaining a copy
...
```

(Replace `<Your Name>` and fill the full MIT text in `LICENSE`)

---

## FAQ

**Q: Why SBO?**
A: SBO avoids heap allocations for small arrays — fewer allocations and better runtime behavior for typical small buffers.

**Q: Why not call constructors/destructors?**
A: Simplicity and C compatibility. The library stores raw bytes; callers manage complex element lifetimes.

**Q: Is it thread-safe?**
A: No. Wrap operations with your own synchronization for concurrent use.

**Q: How to change SBO size?**
A: Define `DA_INLINE_BYTES` before including the header or compile with `-DDA_INLINE_BYTES=512`.

**Q: Can I keep pointers returned by `da_data()` across pushes?**
A: No — mutating operations that reallocate will invalidate pointers. Use indices or copy required data out.

---

## CHANGELOG (starter)

```
# Changelog

## [Unreleased]
- Add `da_pop_back` and `da_back`.
- Improve SBO transitions and overflow handling.
- Add more documentation and examples.

## [0.1.0] - 2025-08-25
- Initial release: create, destroy, push_back, reserve, shrink_to_fit, clear.
```

---
