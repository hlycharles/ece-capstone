#include <stdio.h>

#include "data/input.h"
#include "./recognition.h"
#include "../resample/resample.h"

void readInputImage(int inputImg[]) {
    for (int i = 0; i < imgLen; i++) {
        inputImg[i] = inputImgVal[i];
    }
}

// output face index
void outputFaceIndex(int faceIndex) {
    printf("face: %d\n", faceIndex);
}

int main() {
    initData();

    // get input data
    int inputImg[imgLen];
    readInputImage(inputImg);

    // resize image
    int resizedInput[imgLen];
    resample(imgSize, imgSize, &(inputImg[0]), resizedInput);

    int faceIndex = processImage(resizedInput);

    outputFaceIndex(faceIndex);

    return faceIndex;
}
