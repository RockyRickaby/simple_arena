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
    Arena *ar = arena_create(5);
    size_t pos = arena_get_pos(ar);
    printf("%lu\n", pos);
    int k;
    size_t out;
    ARENA_PUSH(ar, int, &out);
    pos = arena_get_pos(ar);
    // arena_push(ar, 13, 4, &out);
    // arena_pop_to(ar, pos + 1);

    // int c = getc(stdin);
    arena_clear_zero(ar);
    arena_destroy(ar);
}