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

int recognition(int inImg[], int r, int c) {

    // resize image
    int resizedInput[imgLen];
    resample(r, c, &(inImg[0]), resizedInput);

    int faceIndex = processImage(resizedInput);

    outputFaceIndex(faceIndex);

    return faceIndex;
}

int main() {
    recognition(inputImg, 20, 20);
}
