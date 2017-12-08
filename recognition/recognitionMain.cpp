#include "./recognition.h"
#include "../resample/resample.h"


void nomi(int inputImg[400], int result[10]) {
#pragma HLS INTERFACE axis port=inputImg
#pragma HLS INTERFACE axis port=result

	int dists[100];
	int faceIndex = recognizeImage(inputImg, dists);

	for (int i = 0; i < 10; i++) {
		if (i < 2) {
			result[i] = faceIndex;
		} else {
			result[i] = dists[i - 2];
		}
	}
}
