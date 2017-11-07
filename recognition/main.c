// #include "data/input.h"
#include "./recognition.h"
#include "../resample/resample.h"

/*
void readInputImage(int inputImg[]) {
    for (int i = 0; i < imgLen; i++) {
        inputImg[i] = inputImgVal[i];
    }
} */

void recognition(int a, int *result) {
#pragma HLS INTERFACE s_axilite port=return bundle=HLS_MACC_PERIPH_BUS
#pragma HLS INTERFACE s_axilite port=a bundle=HLS_MACC_PERIPH_BUS
#pragma HLS INTERFACE s_axilite port=result bundle=HLS_MACC_PERIPH_BUS

	static int dists[6];
	static int currentDist = 0;

	static int inputImg[400];
	static int resizedInput[400];

	static int pixelIndex = 0;

	inputImg[pixelIndex] = a;
	pixelIndex++;

	if (pixelIndex == 400) {
		pixelIndex = 0;

		initData();

		// get input data
		// readInputImage(inputImg);

		// resize image
		// TODO: specify latency
		// resample(imgSize, imgSize, &(inputImg[0]), resizedInput);

		int faceIndex = processImage(resizedInput, dists);

		if (a == 1) {
			int dist = dists[currentDist];
			currentDist += 1;
			if (currentDist >= 6) {
				currentDist = 0;
			}
			*result = dist;
		} else {
			*result = faceIndex;
		}
	} else {
		*result = -1;
	}
}
