#include <stdlib.h>
#include <stdio.h>

#include "util/matrix.h"
#include "util/io.h"

int imgSize = 3;

int imgSetSize = 2;

unsigned *calculateAvgImg() {
    unsigned **imgs = readImgs(imgSize, imgSetSize);
    unsigned *avgImg = malloc(imgSize * imgSize * sizeof(unsigned));
    // init average image
    for (int i = 0; i < imgSize * imgSize; i++) {
        avgImg[i] = 0;
    }
    // calculate aggregate sum for each pixel
    for (int c = 0; c < imgSetSize; c++) {
        unsigned *img = imgs[c];
        for (int i = 0; i < imgSize * imgSize; i++) {
            avgImg[i] += img[i];
        }
    }
    // calculate average value for each pixel
    for (int i = 0; i < imgSize * imgSize; i++) {
        avgImg[i] /= imgSetSize;
    }

    storeVec(avgImg, imgSize, "./avg.txt");
    return avgImg;
}

void calculateEigenElems(unsigned *avgImg) {
    unsigned **imgs = readImgs(imgSize, imgSetSize);
    unsigned **normalized = malloc(imgSetSize * sizeof(unsigned));
    for (int i = 0; i < imgSetSize; i++) {
        normalized[i] = malloc(imgSize * imgSize * sizeof(unsigned));
        for (int j = 0; j < imgSize * imgSize; j++) {
            normalized[i][j] = imgs[i][j] - avgImg[j];
        }
    }
    // preprocess matrices
    int imgLen = imgSize * imgSetSize;
    unsigned **normal_trans = matrix_transpose(normalized, imgSetSize, imgLen);
    unsigned **normal_mult = matrix_mult(
        normalized, imgSetSize, imgLen, normal_trans, imgLen, imgSetSize
    );
    // calculate eigen elements
    double *eval = malloc(sizeof(double) * imgSetSize * 2);
    double **evec = malloc(sizeof(double) * imgSetSize);
    for (int i = 0; i < imgSetSize; i++) {
        evec[i] = malloc(sizeof(double) * imgSetSize * 2);
    }
    eigen_system((int **)normal_mult, imgSetSize, eval, evec);

    storeVec_double(eval, imgSetSize * 2, "./eval.txt");
 }

int main() {
    unsigned *avg = calculateAvgImg(); 
    
    calculateEigenElems(avg);

    free(avg);
    return 0;
}