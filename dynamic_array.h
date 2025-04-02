#pragma once
#include <stdlib.h>

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
