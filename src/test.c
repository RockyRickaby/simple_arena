#ifdef __cplusplus
extern "C" {}
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdalign.h>
#include <string.h>

#include "arena.h"
#include "sizes.h"
#include "align.h"

int main(void) {
    Arena *ar = arena_create(4);
    arena_print_bytes(ar);

    size_t pos = arena_get_pos(ar);
    printf("%lu\n", pos);
    size_t out;
    int *ptr = ARENA_PUSH(ar, int, &out);
    *ptr = ~(1 << 31);
    pos = arena_get_pos(ar);
    arena_print_bytes(ar);
    // arena_push(ar, 13, 4, &out);
    // arena_pop_to(ar, pos + 1);

    // int c = getc(stdin);
    arena_clear_zero(ar);
    arena_print_bytes(ar);
    arena_destroy(ar);
}