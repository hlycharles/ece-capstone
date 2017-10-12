#include <stdlib.h>
#include <stdio.h>

#include "util/matrix.h"
#include "util/io.h"

int imgSize = 3;

int imgSetSize = 2;

int *calculateAvgImg() {
    int **imgs = readImgs(imgSize, imgSetSize);
    int *avgImg = malloc(imgSize * imgSize * sizeof(int));
    // init average image
    for (int i = 0; i < imgSize * imgSize; i++) {
        avgImg[i] = 0;
    }
    // calculate aggregate sum for each pixel
    for (int c = 0; c < imgSetSize; c++) {
        int *img = imgs[c];
        for (int i = 0; i < imgSize * imgSize; i++) {
            avgImg[i] += img[i];
        }
    }
    // calculate average value for each pixel
    for (int i = 0; i < imgSize * imgSize; i++) {
        avgImg[i] /= imgSetSize;
    }

    storeVec(avgImg, imgSize, "../data/avg.txt");
    return avgImg;
}

void calculateEigenElems(int *avgImg) {
    int **imgs = readImgs(imgSize, imgSetSize);
    int **normalized = malloc(imgSetSize * sizeof(int));
    for (int i = 0; i < imgSetSize; i++) {
        normalized[i] = malloc(imgSize * imgSize * sizeof(int));
        for (int j = 0; j < imgSize * imgSize; j++) {
            normalized[i][j] = imgs[i][j] - avgImg[j];
        }
    }
    // preprocess matrices
    int imgLen = imgSize * imgSetSize;
    int **normal_trans = matrix_transpose(normalized, imgSetSize, imgLen);
    int **normal_mult = matrix_mult(
        normalized, imgSetSize, imgLen, normal_trans, imgLen, imgSetSize
    );
    // calculate eigen elements
    double *eval = malloc(sizeof(double) * imgSetSize * 2);
    double **evec = malloc(sizeof(double) * imgSetSize);
    for (int i = 0; i < imgSetSize; i++) {
        evec[i] = malloc(sizeof(double) * imgSetSize * 2);
        for (int j = 0; j < imgSetSize * 2; j++) {
            evec[i][j] = 0;
        }
    }
    eigen_system((int **)normal_mult, imgSetSize, eval, evec);

    storeVec_double(eval, imgSetSize * 2, "../data/eval.txt");

    for (int i = 0; i < imgSetSize; i++) {
        double *final_vec = matrix_mult_cplx(normal_trans, imgLen, imgSetSize, evec[i], imgSetSize * 2);
        storeVec_double(final_vec, imgLen * 2, "../data/evec.txt");
    }
 }

int main() {
    int *avg = calculateAvgImg(); 
    
    calculateEigenElems(avg);

    free(avg);
    return 0;
}