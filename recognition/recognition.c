#include "util/matrix.h"
#include "util/io.h"
#include "data/avg.h"
#include "data/evec.h"
#include "data/eval.h"

const int imgSize = 20;
const int imgSetSize = 6;
// imgSize * imgSize
const int imgLen = 400;

void calcWeightVectorElem(double *evec, int *normalized, double *dist) {
    matrix_mult_double_rev(evec, imgLen, normalized, imgLen, dist);
}

int findFaceIndex(double *wvec, double wvecs[][imgSetSize], int dists[]) {
    int index = -1;
    int minDist = -1;
    for (int i = 0; i < imgSetSize; i++) {
        double *currVec = wvecs[i];
        int dist = vec_dist(wvec, currVec, imgSetSize);
        dists[i] = dist;
        if (minDist < 0 || dist < minDist) {
            minDist = dist;
            index = i;
        }
    }
    return index;
}

// return index of person recognized, -1 if not a person
int processImage(int inputImg[], int dists[]) {
    // calculate normalized image
    int normalized[imgLen];
    int i, j;
    for (i = 0; i < imgLen; i++) {
        normalized[i] = inputImg[i] - avgImg[i];
    }

    // calculate weight vector
    double wvec[imgSetSize];
    for (j = 0; j < imgSetSize; j++) {
        double *evec = evecs[j];
        double dist;
        calcWeightVectorElem(evec, normalized, &dist);
        wvec[j] = dist;
    }

    // find the face index
    int faceIndex = findFaceIndex(wvec, wvecs, dists);

    return faceIndex;
}
