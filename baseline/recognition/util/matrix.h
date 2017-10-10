#ifndef MATRIX_H
#define MATRIX_H

int **matrix_mult(int **matrix1, int r1, int c1, int **matrix2, int r2, int c2);
double *matrix_mult_cplx(int **m, int r1, int c1, double *v, int r2);
double *matrix_mult_cplx_rev(double *m, int c, int *v, int r);
int **matrix_transpose(int **matrix, int r, int c);
void eigen_system(int **matrix, int size, double *eva, double **eve);
int vec_dist(double *v1, double *v2, int len);

void free_matrix(int **m, int r);

#endif