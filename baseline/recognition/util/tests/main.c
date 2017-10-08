#include <stdlib.h>
#include <stdio.h>

#include "../matrix.h"

int testTranspose() {
    int r = 5, c = 2;
    unsigned **m = malloc(sizeof(unsigned *) * r);
    for (int i = 0; i < r; i++) {
        m[i] = malloc(sizeof(unsigned) * c);
    }
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            m[i][j] = i * 100 + j;
        }
    }

    unsigned **tm = matrix_transpose(m, r, c);
    int success = 0;
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if (tm[j][i] != i * 100 + j) {
                printf(
                    "ERROR: row %d col %d : expect %d instead receive %d\n", 
                    i, j, i * 100 + j, tm[j][i]
                );
                success = -1;
            }
        }
    }

    free_matrix(m, r);
    free_matrix(tm, c);

    return success;
}

int main() {
    int transpose = testTranspose();
    if (transpose >= 0) {
        printf("Ok: transpose\n");
    }
    return 0;
}