#include "./recognition.h"
#include "../resample/resample.h"

const int imgSize = 20;

void recognition(int a, int *result) {
#pragma HLS INTERFACE s_axilite port=return bundle=HLS_MACC_PERIPH_BUS
#pragma HLS INTERFACE s_axilite port=a bundle=HLS_MACC_PERIPH_BUS
#pragma HLS INTERFACE s_axilite port=result bundle=HLS_MACC_PERIPH_BUS

	static int inputImg[400];
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

		resample(imgSize, imgSize, inputImg, resizedImg);

		int faceIndex = processImage(resizedImg, dists);
		*result = faceIndex;
		return;
	}

	inputImg[pixelIndex] = a;
	pixelIndex += 1;

	*result = 4200 + pixelIndex;
}
