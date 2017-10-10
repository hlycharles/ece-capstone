#include <stdlib.h>
#include <stdio.h>

#include "../matrix.h"

int testTranspose() {
    int r = 5, c = 2;
    int **m = malloc(sizeof(int *) * r);
    for (int i = 0; i < r; i++) {
        m[i] = malloc(sizeof(int) * c);
    }
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            m[i][j] = i * 100 + j;
        }
    }

    int **tm = matrix_transpose(m, r, c);
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

int testEntry(int **m, int i, int j, int target) {
    if (m[i][j] != target) {
        printf("ERROR: row %d col %d : expect %d instead receive %d\n", i, j, target, m[i][j]);
        return -1;
    }
    return 0;
}

int testMult() {
    int r1 = 4, c1 = 2;
    int **m = malloc(sizeof(int *) * r1);
    for (int i = 0; i < r1; i++) {
        m[i] = malloc(sizeof(int) * c1);
    }
    m[0][0] = 2;
    m[0][1] = 5;
    m[1][0] = 8;
    m[1][1] = 10;
    m[2][0] = 11;
    m[2][1] = 3;
    m[3][0] = 9;
    m[3][1] = 30;

    int r2 = 2, c2 = 3;
    int **n = malloc(sizeof(int *) * r2);
    for (int i = 0; i < r2; i++) {
        n[i] = malloc(sizeof(int) * c2);
    }
    n[0][0] = 12;
    n[0][1] = 80;
    n[0][2] = 13;
    n[1][0] = 1;
    n[1][1] = 6;
    n[1][2] = 15;

    int success = 0;
    int **t = matrix_mult(m, r1, c1, n, r2, c2);
    success += testEntry(t, 0, 0, 29);
    success += testEntry(t, 0, 1, 190);
    success += testEntry(t, 0, 2, 101);
    success += testEntry(t, 1, 0, 106);
    success += testEntry(t, 1, 1, 700);
    success += testEntry(t, 1, 2, 254);
    success += testEntry(t, 2, 0, 135);
    success += testEntry(t, 2, 1, 898);
    success += testEntry(t, 2, 2, 188);
    success += testEntry(t, 3, 0, 138);
    success += testEntry(t, 3, 1, 900);
    success += testEntry(t, 3, 2, 567);

    free_matrix(m, r1);
    free_matrix(n, r2);
    free_matrix(t, r1);

    return 0;
}

int testEigenSys() {
    int size = 3;
    int **m = malloc(sizeof(int *) * size);
    for (int i = 0; i < size; i++) {
        m[i] = malloc(sizeof(int) * size);
    }

    m[0][0] = 1;
    m[0][1] = 0;
    m[0][2] = 0;
    m[1][0] = 0;
    m[1][1] = 1;
    m[1][2] = 0;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 0;

    eigen_system(m, size, NULL, NULL);

    return 0;
}

int main() {
    int transpose = testTranspose();
    if (transpose >= 0) {
        printf("Ok: transpose\n");
    }

    int mult = testMult();
    if (mult >= 0) {
        printf("Ok: mult\n");
    }

    int eigen = testEigenSys();
    if (eigen >= 0) {
        printf("Ok: eigen\n");
    }

    return 0;
}