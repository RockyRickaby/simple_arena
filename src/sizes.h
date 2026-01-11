#ifndef SIZES_ARENA_H
#define SIZES_ARENA_H

#include <stddef.h>

#define KiB(n) ((size_t)n << 10)
#define MiB(n) ((size_t)n << 20)
#define GiB(n) ((size_t)n << 30)

#define KB(n) ((size_t)n * 1000)
#define MB(n) ((size_t)n * 1000000)
#define GB(n) ((size_t)n * 1000000000)

#endif // SIZES_ARENA_H