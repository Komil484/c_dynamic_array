#include "dynamic_array.h"
#include <stdio.h>
#include <memory.h>

struct da_header *da_get_header(void *da)
{
    return &((struct da_header *)da)[-1];
}

void *_da_internals_new(size_t size)
{
    struct da_header *dah = malloc(DA_DEFAULT_CAPACITY * size + DA_HEADER_SIZE);
    dah->cap = DA_DEFAULT_CAPACITY;
    dah->len = 0;
    return &dah[1];
}

void da_free(void *da)
{
    free(da_get_header(da));
}

void *_da_internals_append(void *da, size_t size)
{
    struct da_header *dah = da_get_header(da);
    if (dah->len >= dah->cap) {
        dah->cap = dah->cap / 2 * 3 + 4;
        dah = realloc(dah, dah->cap * size + DA_HEADER_SIZE);
        if (dah == NULL) {
            fprintf(stderr, "Failed to allocate memory");
            exit(1);
        }
    }
    return &dah[1];
}

size_t da_len(void *da)
{
    return da_get_header(da)->len;
}

size_t da_cap(void *da)
{
    return da_get_header(da)->cap;
}

void *_da_internals_copy(void *da, size_t size)
{
    struct da_header *dah = da_get_header(da);
    struct da_header *dah_new = malloc(dah->cap * size + DA_HEADER_SIZE);
    if (dah_new == NULL) {
        fprintf(stderr, "Failed to allocate memory");
        exit(1);
    }
    void *da_new = &dah_new[1];
    memcpy(dah_new, dah, dah->cap * size + DA_HEADER_SIZE);
    return da_new;
}
