#ifndef IO_H
#define IO_H

unsigned **readImgs(int imgSize, int imgSetSize);

void storeVec(unsigned *v, int size, char path[]);
void storeVec_double(double *v, int size, char path[]);

#endif