#ifndef DYNAMIC_ARRAY_H_
#define DYNAMIC_ARRAY_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Constants
 */

#define DA_HEADER_SIZE sizeof(struct da_header)
#define DA_DEFAULT_CAPACITY 16

/*
 * User Interface
 */

#define da_new(type) ((type *)(_da_internals_new(sizeof(type))))

#define da_append(da, item)                                                                        \
    ((da) = _da_internals_append((da), sizeof(*(da))), (da)[da_get_header(da)->len++] = (item),    \
     (da))

#define da_pop(da) ((da)[da_get_header(da)->len--])

#define da_insert(da, item, index)                                                                 \
    (da_append((da), 0), memmove((char *)(da) + sizeof(*(da)), (da), (da_len(da) - index) * size), \
     (da)[index] = (item), (da))

#define da_remove(da, index)                                                                       \
    (memmove((da), (char *)(da) + sizeof(*(da)), (da_len(da) - index - 1) * size), (da))

#define da_copy(da) (_da_internals_copy((da), sizeof(*(da))))

struct da_header {
    size_t len, cap;
};

struct da_header *da_get_header(void *da);
void da_free(void *da);
size_t da_len(void *da);
size_t da_cap(void *da);

/*
 * Internals
 */
void *_da_internals_new(size_t size);
void *_da_internals_append(void *da, size_t size);
void *_da_internals_copy(void *da, size_t size);
#endif /* DYNAMIC_ARRAY_H_ */

#ifdef DYNAMIC_ARRAY_IMPLEMENTATION

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

#endif /* DYNAMIC_ARRAY_IMPLEMENTATION */
