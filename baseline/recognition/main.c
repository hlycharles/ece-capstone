#include <stdlib.h>

#include "util/matrix.h"

int imgSize = 20;

int imgSetSize = 60;

int *readInputImage() {
    return NULL;
}

int *readAvgImage() {
    return NULL;
}

int **readEVecs() {
    return NULL;
}

int **readWVecs() {
    return NULL;
}

int calcWeightVectorElem(int *evec, int *normalized) {
    int imgLen = imgSize * imgSize;
    // convert to multiplicable forms
    int **evec_m = malloc(sizeof(int *));
    evec_m[0] = malloc(sizeof(int) * imgLen);
    for (int i = 0; i < imgLen; i++) {
        evec_m[0][i] = evec[i];
    }
    int **normalized_m = malloc(sizeof(int *) * imgLen);
    for (int i = 0; i < imgLen; i++) {
        normalized_m[i] = malloc(sizeof(int));
        normalized_m[i][0] = normalized[i];
    }
    int **w_m = matrix_mult(evec_m, 1, imgLen, normalized_m, imgLen, 1);

    int result = w_m[0][0];
    free(w_m);
    free(evec_m);
    free(normalized_m);
    return result;
}

int findFaceIndex(int *wvec) {
    int index = -1;
    int minDist = -1;
    int **wvecs = readWVecs();
    for (int i = 0; i < imgSetSize; i++) {
        int *currVec = malloc(sizeof(int) * (imgSetSize - 1));
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
int processImage(int *img) {
    int *avgImg = readInputImage();
    // calculate normalized image
    int *normalized = malloc(imgSize * imgSize * sizeof(int));
    for (int i = 0; i < imgSize * imgSize; i++) {
        normalized[i] = img[i] - avgImg[i];
    }

    // calculate weight vector
    int *wvec = malloc(sizeof(int) * (imgSetSize - 1));
    int **evecs = readEVecs();
    for (int i = 0; i < imgSetSize - 1; i++) {
        int *evec = malloc(sizeof(int) * imgSize * imgSize);
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
        int *inputImg = readInputImage();
        int faceIndex = processImage(inputImg);
        outputFaceIndex(faceIndex);
    }

    return 0;
}