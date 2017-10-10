#ifndef IO_H
#define IO_H

int *readImg(int imgSize, char path[]);
int **readImgs(int imgSize, int imgSetSize);

double **readCplxMatrix(int r, int c, char path[]);

void storeVec(int *v, int size, char path[]);
void storeVec_double(double *v, int size, char path[]);

#endif