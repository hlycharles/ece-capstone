#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "./haar.h"
#include "./image.h"
#include "../recognition/recognitionModule.h"
// #include "./120p.h"
#include "ap_int.h"

#include "hls_stream.h";

using namespace std;

#define IN_WIDTH 19200

void face_sys(int rd[IN_WIDTH], int ot[5]) {
#pragma HLS INTERFACE axis port=rd
#pragma HLS INTERFACE axis port=ot

	unsigned char Data[120][160];
	for (int i = 0; i < 120; i++) {
		for (int j = 0; j < 160; j++) {
			Data[i][j] = (unsigned char)(rd[i * 160 + j]);
		}
	}

	// Arguments to be passed to DUT  
	MyRect result[RESULT_SIZE];
	int result_x[RESULT_SIZE];
	int result_y[RESULT_SIZE];
	int result_w[RESULT_SIZE];
	int result_h[RESULT_SIZE];

	int result_size = detectFaces(Data, result_x, result_y, result_w, result_h);

	for (int j = 0; j < RESULT_SIZE; j++){
	result[j].x = result_x[j];
	result[j].y = result_y[j];
	result[j].width = result_w[j];
	result[j].height = result_h[j];
	}

	int bestImage = -1;
	int bestWidth = -1;
	for (int i = 0; i < result_size; i++) {
		if (result[i].width >= 20 && result[i].height >= 20 && result[i].width > bestWidth) {
			bestWidth = result[i].width;
			bestImage = i;
		}
	}

	if (bestImage < 0) {
		for (int k = 0; k < 5; k++) {
			ot[k] = 0;
		}
	 	return;
	}

	MyRect r = result[bestImage];

	// convert to 1D image
	int inImg[19200];
	int dists[100];
	for (int i = 0; i < r.height; i++) {
		for (int j = 0; j < r.width; j++) {
			inImg[i * r.width + j] = (int)(Data[(r.y + i)][(r.x + j)]);
		}
    }
	
	int faceIndex = recognition(inImg, r.height, r.width, dists);
	for (int k = 0; k < 5; k++) {
		if (k == 0) {
			ot[k] = faceIndex;
		} else if (k == 1) {
			ot[k] = r.x;
		} else if (k == 2) {
			ot[k] = r.y;
		} else if (k == 3) {
			ot[k] = r.width;
		} else if (k == 4) {
			ot[k] = r.height;
		}
	}
}
