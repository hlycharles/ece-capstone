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
