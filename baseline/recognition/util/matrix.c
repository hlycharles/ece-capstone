#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_eigen.h>

int **matrix_transpose(int **matrix, int r, int c) {
    int **result = malloc(sizeof(int *) * c);
    for (int j = 0; j < c; j++) {
        result[j] = malloc(r * sizeof(int));
        for (int i = 0; i < r; i++) {
            result[j][i] = matrix[i][j];
        }
    }
    return result;
}

int **matrix_mult(int **matrix1, int r1, int c1, int **matrix2, int r2, int c2) {
    if (c1 != r2) {
        return NULL;
    }
    int **result = malloc(sizeof(int *) * r1);
    for (int i = 0; i < r1; i++) {
        result[i] = malloc(c2 * sizeof(int));
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

double *matrix_mult_cplx(int **m, int r1, int c1, double *v, int r2) {
    if (c1 * 2 != r2) {
        return NULL;
    }
    double *result = malloc(sizeof(double) * r1 * 2);
    for (int i = 0; i < r1; i++) {
        double real = 0;
        double img = 0;
        for (int j = 0; j < c1; j++) {
            real += m[i][j] * v[2 * j];
            img += m[i][j] * v[2 * j + 1];
        }
        result[2 * i] = real;
        result[2 * i + 1] = img;
    }
    return result;
}

double *matrix_mult_cplx_rev(double *v1, int c, int *v2, int r) {
    if (c != r) {
        return NULL;
    }
    double *result = malloc(sizeof(double) * 2);
    double real = 0;
    double img = 0;
    for (int i = 0; i < c; i++) {
        real += v1[2 * i] * v2[i];
        // printf("real: %.3f\n", real);
        img += v1[2 * i + 1] * v2[i];
    }
    result[0] = real;
    result[1] = img;
    return result;
}

void eigen_system(int **matrix, int size, double *eva, double **eve) {
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

    if (eva != NULL) {
        for (int i = 0; i < size; i++) {
            eva[2 * i] = GSL_VECTOR_REAL(eval, i);
            eva[2 * i + 1] = GSL_VECTOR_IMAG(eval, i);
        }
    }

    if (eve != NULL) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                eve[i][2 * j] = GSL_REAL(gsl_matrix_complex_get(evec, j, i));
                eve[i][2 * j + 1] = GSL_IMAG(gsl_matrix_complex_get(evec, j, i));
            }
        }
    }
}

// calculate Euclidean distance between two vectors
int vec_dist(double *v1, double *v2, int len) {
    double result = 0;
    for (int i = 0; i < len; i++) {
        double diffReal = v1[2 * i] - v2[2 * i];
        // printf("diffReal: %.3f\n", diffReal);
        double diffImg = v1[2 * i + 1] - v2[2 * i + 1];
        // printf("diffImg: %.3f\n", diffImg);
        result += diffReal * diffReal + diffImg * diffImg;
    }
    return (int)(sqrt(result));
}

// free matrix
void free_matrix(int **m, int r) {
    for (int i = 0; i < r; i++) {
        free(m[i]);
    }
    free(m);
}
