#include "./recognition.h"
#include "../resample/resample.h"

const int imgSize = 20;

int recognition(int inImg[], int r, int c, int dists[]) {
// #pragma HLS INTERFACE axis port=inputImg
// #pragma HLS INTERFACE axis port=result

	int resizedImg[400];

	// int i;

	resample(r, c, inImg, resizedImg);

	int faceIndex = recognizeImage(resizedImg, dists);

	// leave blank space for fault tolerance
	/*
	for (i = 0; i < 30; i++) {
		if (i < 18) {
			result[i] = faceIndex;
		} else {
			result[i] = dists[i % 6];
		}
	} */

	return faceIndex;
}
