#include <stdio.h>
#include <stdlib.h>
#include "./haar.h"
#include "./image.h"
#include "../recognition/recognitionModule.h"
// #include "./120p.h"
#include "ap_int.h"

using namespace std;

#define IN_WIDTH 19200

int in_flag = 1;
int in_width = 160;
int in_height = 120;
int in_maxgrey = 255;

void nami(int rd[IN_WIDTH], int ot[20]) {
#pragma HLS INTERFACE axis port=rd
#pragma HLS INTERFACE axis port=ot

	unsigned char Data[120][160];
	for (int i = 0; i < 120; i++) {
		for (int j = 0; j < 160; j++) {
			Data[i][j] = rd[i * 160 + j];
		}
	}

	int checkSum = 0;
	for (int i = 0; i < 120; i++) {
		for (int j = 0; j < 160; j++) {
			checkSum += (int)(Data[i][j]);
		}
	}
	checkSum = checkSum % 997;

	// Arguments to be passed to DUT  
	MyRect result[RESULT_SIZE];
	int result_x[RESULT_SIZE];
	int result_y[RESULT_SIZE];
	int result_w[RESULT_SIZE];
	int result_h[RESULT_SIZE];

	int res_size=0;
	int *result_size = &res_size;

	/*
	for (int i = 0; i < IMAGE_HEIGHT; i+=1 ){
		unsigned char row[160];
		for (int j = 0; j < 160; j++) {
			row[j] = Data[i * 160 + j];
		}
		detectFaces (row, result_x, result_y, result_w, result_h, result_size);
	}*/
	detectFaces(Data, result_x, result_y, result_w, result_h, result_size);


	for (int j = 0; j < RESULT_SIZE; j++){
	result[j].x = result_x[j];
	result[j].y = result_y[j];
	result[j].width = result_w[j];
	result[j].height = result_h[j];
	}

	// save detection results
	if (*result_size == 0 || (result[0].width < 20 && result[0].height < 20)) {
		for (int k = 0; k < 20; k++) {
			ot[k] = 100;
		}
 		return;
	}

	MyRect r = result[0];

	// convert to 1D image
	int inImg[19200];
	int dists[8];
	for (int i = 0; i < r.height; i++) {
		for (int j = 0; j < r.width; j++) {
			// inImg[i * r.width + j] = (int)(Data[(r.y + i)][(r.x + j)]);
			inImg[i * r.width + j] = (int)(Data[(r.y + i)][(r.x + j)]);
		}
    }
	
	int faceIndex = recognition(inImg, r.height, r.width, dists);
	for (int k = 0; k < 20; k++) {
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
		} else if (k < 13) {
			ot[k] = dists[k - 5];
		} else {
			ot[k] = checkSum;
		}
	}
}
