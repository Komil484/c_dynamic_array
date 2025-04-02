#include "dynamic_array.h"
#include <stdio.h>

int main(void)
{
    // initializing a new dynamic array 'da' of type 'int'
    int *da = da_new(int);

    for (int i = 0; i < 5; ++i) {
        int item = ((i + 1) * 17 + 7) % 37;
        // adding 'item' to the end of 'da'
        da_append(da, item);
    }

    // iterating by length
    for (int i = 0; i < da_len(da); ++i) {
        printf("%02d ", da[i]);
    }
    printf("\n");

    // releasing memory allocated by 'da'
    da_free(da);

    return 0;
}
