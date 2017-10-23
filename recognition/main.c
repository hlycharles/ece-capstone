#include <stdio.h>

#include "util/matrix.h"
#include "util/io.h"
#include "data/input.h"
#include "data/avg.h"
#include "data/evec.h"
#include "data/eval.h"

const int imgSize = 20;
const int imgSetSize = 6;
const int imgLen = imgSize * imgSize;

// input image
int inputImg[imgLen];
// average image
int avgImg[imgLen];

// pretrained eigenvectors
double evecs[imgSetSize][imgLen * 2];
// pretrained weight vectors
double wvecs[imgSetSize][imgSetSize * 2];

void readInputImage() {
    for (int i = 0; i < imgLen; i++) {
        inputImg[i] = inputImgVal[i];
    }
}

void readAvgImage() {
    for (int i = 0; i < imgLen; i++) {
        avgImg[i] = avgImgVal[i];
    }
}

void readEVecs() {
    for (int i = 0; i < imgSetSize; i++) {
        for (int j = 0; j < imgLen * 2; j++) {
            evecs[i][j] = evecsVal[i * imgLen * 2 + j];
        }
    }
}

void readWVecs() {
    for (int i = 0; i < imgSetSize; i++) {
        for (int j = 0; j < imgSetSize * 2; j++) {
            wvecs[i][j] = wvecsVal[i * imgSetSize * 2 + j];
        }
    }
}

void calcWeightVectorElem(double *evec, int *normalized, double *real, double *img) {
    matrix_mult_cplx_rev(evec, imgLen, normalized, imgLen, real, img);
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
    double wvec[imgSetSize * 2];
    for (int i = 0; i < imgSetSize; i++) {
        double *evec = evecs[i];
        double real, img;
        calcWeightVectorElem(evec, normalized, &real, &img);
        wvec[2 * i] = real;
        wvec[2 * i + 1] = img;
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

    int faceIndex = processImage();
    outputFaceIndex(faceIndex);

    return 0;
}
