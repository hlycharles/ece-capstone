#ifndef IO_H
#define IO_H

int **readImgs(int imgSize, int imgSetSize);

void storeVec(int *v, int size, char path[]);
void storeVec_double(double *v, int size, char path[]);

#endif