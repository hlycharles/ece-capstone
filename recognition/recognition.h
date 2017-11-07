#ifndef RECOGNITION_H
#define RECOGNITION_H

extern const int imgLen;
extern const int imgSize;

void initData();
int processImage(int inputImg[], int dists[]);

#endif
