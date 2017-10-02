#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_eigen.h>

unsigned **matrix_transpose(unsigned **matrix, int r, int c) {
    unsigned **result = malloc(sizeof(unsigned *) * c);
    for (int j = 0; j < c; j++) {
        result[j] = malloc(r * sizeof(unsigned));
        for (int i = 0; i < r; i++) {
            result[j][r - i - 1] = matrix[i][j];
        }
    }
    return result;
}

unsigned **matrix_mult(unsigned **matrix1, int r1, int c1, unsigned **matrix2, int r2, int c2) {
    if (c1 != r2) {
        return NULL;
    }
    unsigned **result = malloc(sizeof(unsigned *) * r1);
    for (int i = 0; i < r1; i++) {
        result[i] = malloc(c2 * sizeof(unsigned));
        for (int j = 0; j < c2; j++) {
            int sum = 0;
            for (int k = 0; k < c1; k++) {
                sum += matrix1[i][k] * matrix2[k][j];
            }
            result[i][j] = sum;
        }
    }
    return result;
}

void eigen_system(unsigned **matrix, int size) {
    gsl_eigen_nonsymmv_workspace *w = gsl_eigen_nonsymmv_alloc(size);
    gsl_matrix *m = gsl_matrix_alloc(size, size);
    // populate gsl matrix
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            gsl_matrix_set(m, i, j, matrix[i][j]);
        }
    }
    gsl_vector_complex *eval = gsl_vector_complex_calloc(size);
    gsl_matrix_complex *evec = gsl_matrix_complex_calloc(size, size);
    gsl_eigen_nonsymmv(m, eval, evec, w);
}

// calculate Euclidean distance between two vectors
int vec_dist(unsigned *v1, unsigned *v2, int len) {
    int result = 0;
    for (int i = 0; i < len; i++) {
        int diff = v1[i] - v2[i];
        len += diff * diff;
    }
    return (int)(sqrt(result));
}
