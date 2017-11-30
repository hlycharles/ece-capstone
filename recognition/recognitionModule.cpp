#include "./recognition.h"
#include "../resample/resample.h"

const int imgSize = 20;

void recognition(int inputImg[400], int result[30]) {
#pragma HLS INTERFACE axis port=inputImg
#pragma HLS INTERFACE axis port=result

	static int dists[6];
	static int resizedImg[400];

	int i;

	resample(imgSize, imgSize, inputImg, resizedImg);

	int faceIndex = processImage(resizedImg, dists);

	// leave blank space for fault tolerance
	for (i = 0; i < 30; i++) {
		if (i < 18) {
			result[i] = faceIndex;
		} else {
			result[i] = dists[i % 6];
		}
	}
}

/*
void recognition(int a, int *result) {
#pragma HLS INTERFACE s_axilite port=return bundle=HLS_MACC_PERIPH_BUS
#pragma HLS INTERFACE s_axilite port=a bundle=HLS_MACC_PERIPH_BUS
#pragma HLS INTERFACE s_axilite port=result bundle=HLS_MACC_PERIPH_BUS

	static int customImg[400];
	static int pixelIndex = 0;

	static int dists[6];
	static int currentDist = 0;

	if (a == 300) {
		int dist = dists[currentDist];
		currentDist += 1;
		if (currentDist >= 6) {
			currentDist = 0;
		}
		*result = dist;
		return;
	}

	if (a == 400) {
		pixelIndex = 0;

		int resizedImg[400];

		resample(imgSize, imgSize, customImg, resizedImg);

		int faceIndex = processImage(resizedImg, dists);
		*result = faceIndex;
		return;
	}

	if (a == 500) {
		pixelIndex = 0;

		int resizedImg[400];

		resample(imgSize, imgSize, inputImg, resizedImg);

		int faceIndex = processImage(resizedImg, dists);
		*result = faceIndex;
		return;
	}

	customImg[pixelIndex] = a;
	pixelIndex += 1;

	*result = 4200 + pixelIndex;
} */
