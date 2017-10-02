#include <stdlib.h>

#include "util/matrix.h"

int imgSize = 20;

int imgSetSize = 60;

unsigned **readImgs() {
    return NULL;
}

unsigned *readInputImage() {
    return NULL;
}

unsigned *readAvgImage() {
    return NULL;
}

unsigned **readEVecs() {
    return NULL;
}

unsigned **readWVecs() {
    return NULL;
}

int calcWeightVectorElem(unsigned *evec, unsigned *normalized) {
    int imgLen = imgSize * imgSize;
    // convert to multiplicable forms
    unsigned **evec_m = malloc(sizeof(unsigned *));
    evec_m[0] = malloc(sizeof(unsigned) * imgLen);
    for (int i = 0; i < imgLen; i++) {
        evec_m[0][i] = evec[i];
    }
    unsigned **normalized_m = malloc(sizeof(unsigned *) * imgLen);
    for (int i = 0; i < imgLen; i++) {
        normalized_m[i] = malloc(sizeof(unsigned));
        normalized_m[i][0] = normalized[i];
    }
    unsigned **w_m = matrix_mult(evec_m, 1, imgLen, normalized_m, imgLen, 1);

    int result = w_m[0][0];
    free(w_m);
    free(evec_m);
    free(normalized_m);
    return result;
}

int findFaceIndex(unsigned *wvec) {
    int index = -1;
    int minDist = -1;
    unsigned **wvecs = readWVecs();
    for (int i = 0; i < imgSetSize; i++) {
        unsigned *currVec = malloc(sizeof(unsigned) * (imgSetSize - 1));
        for (int j = 0; j < imgSetSize - 1; j++) {
            currVec[j] = wvecs[i][j];
        }
        int dist = vec_dist(wvec, currVec, imgSetSize - 1);
        if (minDist < 0 || dist < minDist) {
            minDist = dist;
            index = i;
        }
    }
    return index;
}

// return index of person recognized, -1 if not a person
int processImage(unsigned *img) {
    unsigned *avgImg = readInputImage();
    // calculate normalized image
    unsigned *normalized = malloc(imgSize * imgSize * sizeof(unsigned));
    for (int i = 0; i < imgSize * imgSize; i++) {
        normalized[i] = img[i] - avgImg[i];
    }

    // calculate weight vector
    unsigned *wvec = malloc(sizeof(unsigned) * (imgSetSize - 1));
    unsigned **evecs = readEVecs();
    for (int i = 0; i < imgSetSize - 1; i++) {
        unsigned *evec = malloc(sizeof(unsigned) * imgSize * imgSize);
        for (int e = 0; e < imgSize * imgSize; e++) {
            evec[e] = evecs[i][e];
        }
        int weight = calcWeightVectorElem(evec, normalized);
        free(evec);
        wvec[i] = weight;
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
        unsigned *inputImg = readInputImage();
        int faceIndex = processImage(inputImg);
        outputFaceIndex(faceIndex);
    }

    return 0;
}