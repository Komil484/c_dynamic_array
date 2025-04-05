#ifndef DYNAMIC_ARRAY_H_
#define DYNAMIC_ARRAY_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Usage:
 * define DYNAMIC_ARRAY_IMPLEMENTATION in exactly one .c file before including the header
 *
 * The interface provided by the library can be found in the "User Interface" section
 *
 * When a malloc or realloc fails, the library prints an error and quits the program
 * immediately this can be changed in 2 ways, either modifying the header file itself
 * specifically the body of the da_callback_on_alloc_failure() function, or by defining
 * DYNAMIC_ARRAY_CALLBACK in the same place where DYNAMIC_ARRAY_IMPLEMENTATION is defined
 * In this case, you must define the da_callback_on_alloc_failure() function in the same
 * file as the DYNAMIC_ARRAY_IMPLEMENTATION definition
 *
 */

/*
 * Constants
 */

#define DA_HEADER_SIZE sizeof(struct da_header)
#define DA_DEFAULT_CAPACITY 16

/*
 * User Interface
 */

/*
 * this function is called when malloc/realloc fails
 * if realloc fails, the ptr parameter is the pointer
 * to the first element of the array that was attempted
 * to be resized. in case of malloc, ptr is NULL
 */
void da_callback_on_alloc_failure(void *ptr);

/*
 * creates a new dynamic array of type type
 * {
 *     int *da = da_new(int); // da(int) => {}
 * }
 */
#define da_new(type) ((type *)(_da_internals_new(sizeof(type))))

/*
 * adds `item` to the end of the dynamic array
 * {
 *     // da(int) => {}
 *     da_append(da, 1); // da(int) => {1}
 *     da_append(da, 2); // da(int) => {1, 2}
 * }
 */
#define da_append(da, item)                                                                        \
    ((da) = _da_internals_append((da), sizeof(*(da))), (da)[da_get_header(da)->len++] = (item),    \
     (da))

/*
 * removes the last item from the header, and returns it
 * {
 *     // da(int) => {1, 2}
 *     da_pop(da); // da(int) => {1}
 *     int last = da_pop(da); // da(int) => {}, last => 1
 * }
 */
#define da_pop(da) ((da)[da_get_header(da)->len--])

/*
 * inserts `item` at `index`, everything starting from index will be moved over to the right
 * {
 *     // da(int) => {1, 2, 3}
 *     da_insert(da, 4, 1); // da(int) => {1, 4, 2, 3}
 *
 * }
 */
#define da_insert(da, item, index)                                                                 \
    (da_append((da), 0), memmove((char *)(da) + sizeof(*(da)), (da), (da_len(da) - index) * size), \
     (da)[index] = (item), (da))

/*
 * removes element at index
 * {
 *     // da(int) => {1, 2, 3, 4}
 *     da_remove(da, 1); // da(int) => {1, 3, 4}
 * }
 */
#define da_remove(da, index)                                                                       \
    (memmove((da), (char *)(da) + sizeof(*(da)), (da_len(da) - index - 1) * size), (da))

/*
 * makes a copy of da and returns it
 * {
 *     // da(int) => {1, 2, 3}
 *     int *da2 = da_copy(da); // da(int) => {1, 2, 3}, da2(int) => {1, 2, 3}
 *     da_append(da, 4); // da(int) => {1, 2, 3, 4}, da2(int) => {1, 2, 3}
 *     da_remove(da2, 1); // da(int) => {1, 2, 3, 4}, da2(int) => {1, 3}
 * }
 *
 * BEWARE, there is no safeguard against pointer conversion when assigning the value
 * returned by da_copy, make sure you are assigning to the correct pointer type
 */
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
        struct dada_header *dah_copy = dah;
        dah = realloc(dah, dah->cap * size + DA_HEADER_SIZE);
        if (dah == NULL) {
            da_callback_on_alloc_failure(*dah_copy[1]);
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
        da_callback_on_alloc_failure(dah_new);
    }
    void *da_new = &dah_new[1];
    memcpy(dah_new, dah, dah->cap * size + DA_HEADER_SIZE);
    return da_new;
}

#ifndef DYNAMIC_ARRAY_CALLBACK
void da_callback_on_alloc_failure(void *da)
{
    fprintf(stderr, "Failed to allocate memory");
    exit(1);
}
#endif

#endif /* DYNAMIC_ARRAY_IMPLEMENTATION */
