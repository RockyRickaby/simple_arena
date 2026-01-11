#ifndef ARENA_ALIGN_H
#define ARENA_ALIGN_H

#define ALIGN_POW2_UP(n, p) (((size_t)(n) + ((size_t)(p) - 1)) & (~((size_t)(p) - 1)))
#define ALIGN_POW2_DOWN(n, p) (((size_t)(n)) & (~((size_t)(p) - 1)))
#define IS_POW_2(n) ((size_t)(n) != 0 && !((size_t)(n) & ((size_t)(n) - 1)))

#endif // ARENA_ALIGN_H