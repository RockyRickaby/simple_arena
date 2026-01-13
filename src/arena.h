#ifndef ARENA_ALLOC_H
#define ARENA_ALLOC_H

#include <stddef.h> // size_t
#include <stdalign.h>

#define ARENA_PUSH_ARRAY(arena, _Type, count, effective_size_out) arena_push((arena), sizeof(_Type) * (count), alignof(_Type), effective_size_out)
#define ARENA_PUSH_ARRAY_ZERO(arena, _Type, count, effective_size_out) arena_push_zero((arena), sizeof(_Type) * (count), alignof(_Type), effective_size_out)
#define ARENA_PUSH(arena, _Type, effective_size_out) ARENA_PUSH_ARRAY(arena, _Type, 1, effective_size_out)
#define ARENA_PUSH_ZERO(arena, _Type, effective_size_out) ARENA_PUSH_ARRAY_ZERO(arena, _Type, 1, effective_size_out)

struct _arena_s;
typedef struct _arena_s Arena;

Arena *arena_create(size_t reserve_size);
void arena_destroy(Arena* arena);

void *arena_push(Arena *arena, size_t bytes, size_t align, size_t *effective_size_out);
void *arena_push_zero(Arena *arena, size_t bytes, size_t align, size_t *effective_size_out);
void arena_pop(Arena *arena, size_t bytes);
void arena_pop_to(Arena *arena, size_t pos);
void arena_clear(Arena *arena);
void arena_clear_zero(Arena *arena);

size_t arena_get_pos(Arena *arena);

#endif // ARENA_ALLOC_H