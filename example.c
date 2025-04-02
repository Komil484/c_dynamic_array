#include "dynamic_array.h"
#include <stdio.h>

int main(void)
{
    int **da = da_new(int *);

    for (int i = 0; i < 5; ++i) {
        da_append(da, da_new(int));
        for (int j = 0; j <= i; ++j) {
            int item = ((i + 1) * 31 + (j + 1) * 17) % 37;
            da_append(da[i], item);
        }
    }

    for (int i = 0; i < da_len(da); ++i) {
        for (int j = 0; j < da_len(da[i]); ++j) {
            printf("%02d ", da[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < da_len(da); ++i) {
        da_free(da[i]);
    }
    da_free(da);

    return 0;
}
