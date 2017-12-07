#include <stdio.h>
#include <stdlib.h>
#include "./haar.h"
#include "../recognition/recognitionModule.h"
#include "./120p.h"

using namespace std;

#define DETECT_TIMES 4

int in_flag =1;
int in_width = 160;
int in_height = 120;
int in_maxgrey = 255;

int main() {
#pragma HLS INTERFACE axis port=rd
#pragma HLS INTERFACE axis port=ot

	int flag;
		 
	printf ("-- entering main function --\r\n");
	printf ("-- loading image --\r\n");


	// Arguments to be passed to DUT  
	MyRect result[RESULT_SIZE];
	int result_x[RESULT_SIZE];
	int result_y[RESULT_SIZE];
	int result_w[RESULT_SIZE];
	int result_h[RESULT_SIZE];

	// turn into 1D
	unsigned char data[120][160];
	for (int i = 0; i < 120; i++) {
		for (int j = 0; j < 160; j++) {
			data[i][j] = Data[i][j];
		}
	}

	printf ("-- detecting faces --\r\n");

	for (int t = 0; t < DETECT_TIMES; t++) {

	int result_size = detectFaces(data, result_x, result_y, result_w, result_h);

	printf("\nresult_size = %d", result_size);

	for (int j = 0; j < RESULT_SIZE; j++){
	result[j].x = result_x[j];
	result[j].y = result_y[j];
	result[j].width = result_w[j];
	result[j].height = result_h[j];
	}

	for( int i=0 ; i < result_size ; i++ )
	printf("\n [Test Bench (main) ] detected rects: %d %d %d %d\n",result[i].x,result[i].y,result[i].width,result[i].height);

	// MyRect r = result[0];
		int bestImage = -1;
		int bestWidth = -1;
		for (int i = 0; i < result_size; i++) {
			if (result[i].width >= 20 && result[i].height >= 20 && result[i].width > bestWidth) {
				bestWidth = result[i].width;
				bestImage = i;
			}
		}

	// save detection results
		if (bestImage < 0) {
		 	return -1;
		}

		MyRect r = result[bestImage];

		// convert to 1D image
		int inImg[19200];
		int dists[8];
		r.x = r.x - 45;
		if (r.x < 0) {
			r.x = 0;
		}
		for (int i = 0; i < r.height; i++) {
			for (int j = 0; j < r.width; j++) {
				// inImg[i * r.width + j] = (int)(Data[(r.y + i)][(r.x + j)]);
				inImg[i * r.width + j] = (int)(Data[(r.y + i)][(r.x + j)]);
			}
	    }
	
	int faceIndex = recognition(inImg, r.height, r.width, dists);
	for (int i = 0; i < 8; i++) {
		printf("dist: %d\n", dists[i]);
	}

	printf("face index: %d\n", faceIndex);

	}

	return 0;
}
