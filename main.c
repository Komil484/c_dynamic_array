#include "dynamic_array.h"
#include <stdio.h>

int main(void)
{
    int *da = da_new(int);
    /*
    int **da = da_new(int *);
        for (int i = 0; i < 5; ++i) {
        da_append(da, da_new(int));
    }

    for (;;) {
        int item = (a * 31 + b * 17) % 37;
    }
    */
    for (int i = 0; i < 1000; ++i) {
        printf("appended item\n");
        da_append(da, i);
    }
    for (int i = 0; i < da_len(da); ++i) {
        printf("%d ", da[i]);
    }
    printf("\n");
    da_free(da);

    return 0;
}
