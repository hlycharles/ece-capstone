#include "./recognition.h"
#include "../resample/resample.h"

const int imgSize = 20;

int recognition(int inImg[], int r, int c, int dists[]) {

	int resizedImg[400];

	resample(r, c, inImg, resizedImg);

	int faceIndex = recognizeImage(resizedImg, dists);

	return faceIndex;
}
