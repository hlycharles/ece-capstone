#include <stdio.h>

#include "data/input.h"
#include "./recognition.h"
#include "../resample/resample.h"

const int imgSize = 20;
const int imgLen = 400;

// output face index
void outputFaceIndex(int faceIndex) {
    printf("face: %d\n", faceIndex);
}

int main() {

    // resize image
    int resizedInput[imgLen];
    resample(imgSize, imgSize, &(inputImg[0]), resizedInput);

    int faceIndex = processImage(resizedInput);

    outputFaceIndex(faceIndex);

    return faceIndex;
}
