#ifndef ARENA_ALLOC_H
#define ARENA_ALLOC_H

#include <stddef.h> // size_t
#include <stdalign.h>

#define ARENA_PUSH_ARRAY(arena, _Type, count, effective_size_out) arena_push((arena), sizeof(_Type) * (count), alignof(_Type), effective_size_out)
#define ARENA_PUSH_ARRAY_ZERO(arena, _Type, count, effective_size_out) arena_push_zero((arena), sizeof(_Type) * (count), alignof(_Type), effective_size_out)
#define ARENA_PUSH(arena, _Type, effective_size_out) ARENA_PUSH_ARRAY(arena, _Type, 1, effective_size_out)
#define ARENA_PUSH_ZERO(arena, _Type, effective_size_out) ARENA_PUSH_ARRAY_ZERO(arena, _Type, 1, effective_size_out)

// opaque. not to be messed with
struct _arena_s;
// use this as the type
typedef struct _arena_s Arena;

// Creates (allocates) an arena and returns a pointer to it.
// The size of the arena is equal to reserve_size plus the size of the Arena struct
Arena *arena_create(size_t reserve_size);
// Destroys (frees) the arena.
void arena_destroy(Arena* arena);

// Returns a pointer that points to the newly allocated space.
// Bytes may be of any size as long as it fits the arena. The arena does not resize.
// Align must be a power of two. The alignof macro can be used here.
//
// Holes might be left in the arena after popping previously allocated items by their actual size because of pointer alignment.
// The variable effective_size_out holds the total amount of bytes that have been effectively allocated for the object.
// This allows for popping pointers without potentially leaving unused space in the arena.
// This can also be dealt with by simply clearing the arena.
void *arena_push(Arena *arena, size_t bytes, size_t align, size_t *effective_size_out);
// Returns a pointer that points to the newly allocated space and initializes it with zeroes.
// Bytes may be of any size as long as it fits the arena. The arena does not resize.
// Align must be a power of two. The alignof macro can be used here.
//
// Holes might be left in the arena after popping previously allocated items by their actual size because of pointer alignment.
// The variable effective_size_out holds the total amount of bytes that have been effectively allocated for the object.
// This allows for popping pointers without potentially leaving unused space in the arena.
// This can also be dealt with by simply clearing the arena.
void *arena_push_zero(Arena *arena, size_t bytes, size_t align, size_t *effective_size_out);
// Pops an amount of bytes off of the arena. Bytes may have any value.
// It may partially deallocate objects if not used properly.
void arena_pop(Arena *arena, size_t bytes);
// Moves the internal pointer to position pos.
// Pos may have any value.
// The internal pointer can only be moved towards the opposite direction of
// whatever direction the internal stack may grow torwards (downwards or upwards).
//
// It can be used with the arena_get_pos function to create a sort of scope. The current position
// is saved, some bytes are allocated and then everything is freed at once without having to worry about
// the space that's allocated too much
void arena_pop_to(Arena *arena, size_t pos);
// Pops everything off the arena
void arena_clear(Arena *arena);
// Pops everything off the arena and initializes it to zero
void arena_clear_zero(Arena *arena);
// Returns the current position of the internal pointer
size_t arena_get_pos(Arena *arena);

// Prints to standard output
// Prints a newline if ARENA_DEBUG is not set
// Otherwise, prints the bytes stores in the arena
void arena_print_bytes(const Arena *arena);
// Prints to the output file
// Prints a newline if ARENA_DEBUG is not set
// Otherwise, prints the bytes stores in the arena
void arena_fprint_bytes(FILE *output, const Arena *arena);


#endif // ARENA_ALLOC_H