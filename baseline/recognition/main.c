#include <stdlib.h>

#include "util/matrix.h"

int imgSize = 20;

unsigned *readInputImage() {
    return NULL;
}

unsigned *readAvgImage() {
    return NULL;
}

// return index of person recognized, -1 if not a person
int processImage(unsigned *img) {
    unsigned *avgImg = readInputImage();
    // calculate normalized image
    unsigned *normalized = malloc(imgSize * imgSize * sizeof(unsigned));
    for (int i = 0; i < imgSize * imgSize; i++) {
        normalized[i] = img[i] - avgImg[i];
    }

    // free memory
    free(normalized);

    return -1;
}

int calcWeightVectorElem(unsigned *evec, unsigned *normalized) {
    return 0;
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