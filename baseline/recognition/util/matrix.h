#ifndef MATRIX_H
#define MATRIX_H

int **matrix_mult(int **matrix1, int r1, int c1, int **matrix2, int r2, int c2);
int **matrix_transpose(int **matrix, int r, int c);
void eigen_system(int **matrix, int size, double *eva, double **eve);
int vec_dist(int *v1, int *v2, int len);

void free_matrix(int **m, int r);

#endif