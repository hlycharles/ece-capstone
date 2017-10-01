
int imgSize = 20;

unsigned **readInputImage() {
    return null;
}

unsigned **readAvgImage() {
    return null;
}

unsigned **avgImg = readInputImage();

// return index of person recnogized, -1 if not a person
int processImage(unsigned **img) {
    // calculate normalized image
    unsigned **normalized = malloc(imgSize * sizeof(unsigned*));
    for (int i = 0; i < imgSize; i++) {
        unsigned *row = img[i];
        for (int j = 0; j < imgSize; j++) {
            // image subtraction
        }
    }
    return -1;
}

// output face index
void outputFaceIndex(int faceIndex) {}

int main() {

    // keep processing input images
    while true {
        unsigned **inputImg = readInputImage();
        int faceIndex = processImage(inputImg);
        outputFaceIndex(faceIndex);
    }

    return 0;
}