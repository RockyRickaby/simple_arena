#include <stdio.h> // perror, maybe printf
#include <stdlib.h> // malloc, free
#include <stddef.h> // max_align_t, size_t
#include <string.h> // memset
#include <assert.h> // assert
#include <inttypes.h> // PRIu64

#include "arena.h"
#include "align.h"

#define ARENA_SIZE(ar) ((ar)->reserve_size + sizeof(*(ar)))
#define ARENA_ALIGN sizeof(Arena)

#ifdef ARENA_DEBUG
#   define ARENA_DEBUG_ENABLE 1
#else
#   define ARENA_DEBUG_ENABLE 0
#endif // ARENA_DEBUG

#define ARENA_ASSERT(x) do { if (ARENA_DEBUG_ENABLE) assert(x); } while(0)
#define ARENA_LOG(fd, level, fmt, ...)\
    do {\
        if (ARENA_DEBUG_ENABLE) {\
            fprintf(fd, "%s %s(): " fmt "\n", level, __func__, __VA_ARGS__);\
        }\
    } while(0)

#define ARENA_LOG_INFO  "[\x1b[1;34mINFO\x1b[0m]"
#define ARENA_LOG_WARN  "[\x1b[1;33mWARN\x1b[0m]"
#define ARENA_LOG_ERR   "[\x1b[1;31mERROR\x1b[0m]"

struct _arena_s {
    size_t start;
    size_t end;
    size_t reserve_size;
};

// reserve some space for the arena itself
Arena *arena_create(size_t reserve_size) {
    Arena *arena = calloc(ARENA_ALIGN + reserve_size, sizeof(char));
    if (!arena) {
        ARENA_LOG(stderr, ARENA_LOG_ERR, "Could not allocate arena of size %" PRIu64, reserve_size);
        perror("arena_create");
        abort();
    }
    arena->start = ARENA_ALIGN;
    arena->end = arena->start + reserve_size;
    arena->reserve_size = reserve_size;
    ARENA_LOG(stdout, ARENA_LOG_INFO, "%"PRIu64 " total bytes have been allocated", (ARENA_ALIGN + reserve_size));

    return arena;
}

void arena_destroy(Arena* arena) {
    if (!arena) return;
    arena->start = 0;
    arena->end = 0;
    arena->reserve_size = 0;
    free(arena);
}

void *arena_push(Arena *arena, size_t bytes, size_t align, size_t *effective_size_out) {
    if (!(arena && bytes != 0 && IS_POW_2(align))) {
        if (ARENA_DEBUG_ENABLE) {
            if (!arena) {
                ARENA_LOG(stdout, ARENA_LOG_WARN, "parameter 'arena' is null. No allocations were made", NULL);
            }
            if (!bytes) {
                ARENA_LOG(stdout, ARENA_LOG_WARN, "parameter 'bytes' is zero. No allocations were made", NULL);   
            }
            if (!IS_POW_2(align)) {
                ARENA_LOG(stdout, ARENA_LOG_WARN, "parameter 'align' is not a power of two. Parameter value: %"PRIu64". No allocations were made", align);
            }
        }
        return NULL;
    }
    ARENA_ASSERT(arena->start <= arena->end && "The arena has been overflowed");

    size_t prev_pos = arena->end;
    size_t ptr = arena->end - bytes;
    ptr &= ~(align - 1); // align down according to the user's alignment requirements 
    if (ptr < arena->start) {
        effective_size_out ? *effective_size_out = 0 : 0;
        ARENA_LOG(stdout, ARENA_LOG_WARN, "Not enough space for the allocation of %"PRIu64 " bytes with alignment %"PRIu64, bytes, align);
        return NULL;
    }
    // char *out = (char*)arena + arena->start;
    arena->end = ptr;
    effective_size_out ? *effective_size_out = (prev_pos - arena->end) : 0; 
    char *out = (char*)arena + arena->end;
    ARENA_LOG(stdout, ARENA_LOG_INFO, "Bytes asked: " "%"PRIu64 ". Alignment: " "%"PRIu64 ". Inserted " "%"PRIu64, bytes, align, *effective_size_out);
    return out;
}

void *arena_push_zero(Arena *arena, size_t bytes, size_t align, size_t *effective_size_out) {
    void *out = arena_push(arena, bytes, align, effective_size_out);
    return out ? memset(out, 0, bytes) : NULL;
}

size_t arena_get_pos(Arena *arena) {
    return arena ? arena->end : 0;
}

// TODO - check underflow? improve this
void arena_pop(Arena *arena, size_t bytes) {
    if (!arena) return;
    // size_t min = arena->start - sizeof(Arena) < bytes ? arena->start - sizeof(Arena) : bytes;
    // arena->start -= min;
    size_t min = arena->end + bytes > ARENA_SIZE(arena) ? ARENA_SIZE(arena) - arena->end : bytes;
    ARENA_LOG(stdout, ARENA_LOG_INFO, "Popping %"PRIu64" bytes off the arena. Asked for %"PRIu64, min, bytes);
    arena->end += min;
}

void arena_pop_to(Arena *arena, size_t start) {
    if (!arena) return;
    // size_t min = start < arena->start? arena->start - start : 0;
    size_t min = start > arena->end ? start - arena->end : 0;
    ARENA_LOG(stdout, ARENA_LOG_INFO, "Popping %"PRIu64" bytes off the arena. Requested position: %"PRIu64, min, start);
    arena_pop(arena, min);
}

void arena_clear(Arena *arena) {
    if (!arena) return;
    arena_pop_to(arena, ARENA_SIZE(arena));
}

void arena_clear_zero(Arena *arena) {
    if (!arena) return;
    arena_clear(arena);
    memset(arena + 1, 0, arena->reserve_size);
}

void arena_print_bytes(const Arena *arena) {
    arena_fprint_bytes(stdout, arena);
}

#ifdef ARENA_DEBUG
void arena_fprint_bytes(FILE *output, const Arena *arena) {
    size_t end = arena->start + arena->reserve_size;
    const char *ptr = (char*)arena;
    int count = 0;
    fprintf(output, "   ");
    for (size_t i = 0; i < end; i++) {
        if (count >= 16) {
            fprintf(output, "\n   ");
            count = 0;
        }
        fprintf(output, "%02hhX ", ptr[i]);
        count++;
    }
    fprintf(output, "\n"); // ????
}
#else
void arena_fprint_bytes(FILE *output, Arena *arena) {
    fprintf(output, "\n");
    return;
}
#endif // ARENA_DEBUG