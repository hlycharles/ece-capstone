#ifndef MATRIX_H
#define MATRIX_H

unsigned **matrix_mult(unsigned **matrix1, int r1, int c1, unsigned **matrix2, int r2, int c2);
unsigned **matrix_transpose(unsigned **matrix, int r, int c);
void eigen_system(unsigned **matrix, int size);
int vec_dist(unsigned *v1, unsigned *v2, int len);

#endif