#include <stdlib.h>

#include "util/matrix.h"
#include "util/io.h"

int imgSize = 3;

int imgSetSize = 2;

int *readInputImage() {
    return readImg(imgSize, "./input.txt");
}

int *readAvgImage() {
    return readImg(imgSize, "./avg.txt");
}

double **readEVecs() {
    return readCplxMatrix(imgSetSize, imgSize * imgSize, "./evec.txt");
}

double **readWVecs() {
    return readCplxMatrix(imgSetSize, imgSetSize, "./eval.txt");
}

void calcWeightVectorElem(double *evec, int *normalized, double *real, double *img) {
    int imgLen = imgSize * imgSize;

    double *w_m = matrix_mult_cplx_rev(evec, imgLen, normalized, imgLen);

    *real = w_m[0];
    *img = w_m[1];

    free(w_m);
}

int findFaceIndex(double *wvec) {
    int index = -1;
    int minDist = -1;
    double **wvecs = readWVecs();
    for (int i = 0; i < imgSetSize; i++) {
        double *currVec = wvecs[i];
        int dist = vec_dist(wvec, currVec, imgSetSize);
        if (minDist < 0 || dist < minDist) {
            minDist = dist;
            index = i;
        }
    }
    return index;
}

// return index of person recognized, -1 if not a person
int processImage(int *img) {
    int *avgImg = readInputImage();
    // calculate normalized image
    int *normalized = malloc(imgSize * imgSize * sizeof(int));
    for (int i = 0; i < imgSize * imgSize; i++) {
        normalized[i] = img[i] - avgImg[i];
    }

    // calculate weight vector
    double *wvec = malloc(sizeof(int) * imgSetSize * 2);
    double **evecs = readEVecs();
    for (int i = 0; i < imgSetSize; i++) {
        double *evec = malloc(sizeof(int) * imgSize * imgSize * 2);
        for (int e = 0; e < imgSize * imgSize * 2; e++) {
            evec[e] = evecs[i][e];
        }
        double real, img;
        calcWeightVectorElem(evec, normalized, &real, &img);
        free(evec);
        wvec[2 * i] = real;
        wvec[2 * i + 1] = img;
    }

    // find the face index
    int faceIndex = findFaceIndex(wvec);

    // free memory
    free(wvec);
    free(normalized);

    return faceIndex;
}

// output face index
void outputFaceIndex(int faceIndex) {}

int main() {

    // keep processing input images
    while (1) {
        int *inputImg = readInputImage();
        int faceIndex = processImage(inputImg);
        outputFaceIndex(faceIndex);
    }

    return 0;
}