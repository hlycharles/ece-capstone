#include <stdlib.h>

#include "util/matrix.h"

int imgSize = 20;

int imgSetSize = 60;

unsigned **readImgs() {
    return NULL;
}

void storeAvgImg(unsigned *img) {
    return;
}

unsigned *calculateAvgImg() {
    unsigned **imgs = readImgs();
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

    storeAvgImg(avgImg);
    return avgImg;
}

void calculateEigenElems(unsigned *avgImg) {
    unsigned **imgs = readImgs();
    unsigned **normalized = malloc(imgSetSize * sizeof(unsigned ()));
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
    eigen_system(normal_mult, imgSetSize);
 }

int main() {
    unsigned *avgImg = calculateAvgImg();

    return 0;
}