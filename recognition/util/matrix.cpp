#include <stdlib.h>
#include <math.h>

void matrix_mult_double_rev(double *v1, int c, int *v2, int r, double *result) {
    if (c != r) {
        return;
    }
    double total = 0;
    for (int i = 0; i < c; i++) {
        total += v1[i] * v2[i];
    }
    *result = total;
}

// calculate Euclidean distance between two vectors
int vec_dist(double *v1, double *v2, int len) {
    double result = 0;
    for (int i = 0; i < len; i++) {
        double diff = v1[i] - v2[i];
        result += diff * diff;
    }
    return (int)(sqrt(result));
}
