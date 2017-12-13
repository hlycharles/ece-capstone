#include <stdio.h>
#include <time.h>

#include "util/matrix.h"
#include "util/io.h"

const int imgSize = 20;
const int imgSetSize = 15;
const int imgLen = imgSize * imgSize;

// input image
int inputImg[imgLen];
// average image
int avgImg[imgLen];

// pretrained eigenvectors
double evecs[imgSetSize][imgLen];
// pretrained weight vectors
double wvecs[imgSetSize][imgSetSize];

void readInputImage() {
    readImg(imgSize, "../data/input.txt", inputImg);
}

void readAvgImage() {
    readImg(imgSize, "../data/avg.txt", avgImg);
}

void readEVecs() {
    readCplxMatrix(imgSetSize, imgLen, "../data/evec.txt", evecs);
}

void readWVecs() {
    readCplxMatrix(imgSetSize, imgSetSize, "../data/eval.txt", wvecs);
}

void calcWeightVectorElem(double *evec, int *normalized, double *dist) {
    matrix_mult_double_rev(evec, imgLen, normalized, imgLen, dist);
}

int findFaceIndex(double *wvec) {
    int index = -1;
    int minDist = -1;
    for (int i = 0; i < imgSetSize; i++) {
        double *currVec = wvecs[i];
        int dist = vec_dist(wvec, currVec, imgSetSize);
        printf("dist: %d\n", dist);
        if (minDist < 0 || dist < minDist) {
            minDist = dist;
            index = i;
        }
    }
    return index;
}

// return index of person recognized, -1 if not a person
int processImage() {
    // calculate normalized image
    int normalized[imgLen];
    for (int i = 0; i < imgLen; i++) {
        normalized[i] = inputImg[i] - avgImg[i];
    }

    // calculate weight vector
    double wvec[imgSetSize];
    for (int i = 0; i < imgSetSize; i++) {
        double *evec = evecs[i];
        double dist;
        calcWeightVectorElem(evec, normalized, &dist);
        wvec[i] = dist;
    }

    // find the face index
    int faceIndex = findFaceIndex(wvec);

    return faceIndex;
}

// output face index
void outputFaceIndex(int faceIndex) {
    printf("face: %d\n", faceIndex);
}

int main() {

    readInputImage();
    readAvgImage();

    readEVecs();
    readWVecs();

    int faceIndex;
    clock_t start = clock();
    for (int i = 0; i < 100; i++) {
        faceIndex = processImage();
        // outputFaceIndex(faceIndex);
    }
    clock_t diff = clock() - start;
    double msec = diff * 1000.0 / CLOCKS_PER_SEC;
    printf("Time: %fms\n", msec);

    return 0;
}
