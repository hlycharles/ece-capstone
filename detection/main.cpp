#include <stdio.h>
#include <stdlib.h>
#include "./haar.h"
#include "./image.h"
#include "../recognition/recognitionModule.h"
#include "./120p.h"

using namespace std;

int in_flag = 1;
int in_width = 160;
int in_height = 120;
int in_maxgrey = 255;

void nami(int rd[19200], int ot[60]) {
#pragma HLS INTERFACE axis port=rd
#pragma HLS INTERFACE axis port=ot

	int flag;
		 
	printf ("-- entering main function --\r\n");
	printf ("-- loading image --\r\n");

	static int inHolder[10];
	for (int i = 0; i < 10; i++) {
		inHolder[i] = rd[i];
	}

	// Arguments to be passed to DUT  
	MyRect result[RESULT_SIZE];
	int result_x[RESULT_SIZE];
	int result_y[RESULT_SIZE];
	int result_w[RESULT_SIZE];
	int result_h[RESULT_SIZE];

	int res_size=0;
	int *result_size = &res_size;

	printf ("-- detecting faces --\r\n");

	for (int i = 0; i < IMAGE_HEIGHT; i+=1 ){
		static unsigned char row[160];
		for (int j = 0; j < 160; j++) {
			row[j] = rd[i * 160 + j];
		}
		detectFaces (row, result_x, result_y, result_w, result_h, result_size);
	}

	printf("\nresult_size = %d", *result_size);

	for (int j = 0; j < RESULT_SIZE; j++){
	result[j].x = result_x[j];
	result[j].y = result_y[j];
	result[j].width = result_w[j];
	result[j].height = result_h[j];
	}

	for( int i=0 ; i < *result_size ; i++ )
	printf("\n [Test Bench (main) ] detected rects: %d %d %d %d",result[i].x,result[i].y,result[i].width,result[i].height);

	// save detection results
	if (*result_size == 0) {
		printf("No face detected\n");
		for (int k = 0; k < 60; k++) {
			ot[k] = 20;
		}
 		return;
	}

	MyRect r = result[0];

	// convert to 1D image
	static int inImg[19200];
	static int dists[6];
	for (i = 0; i < r.height; i++) {
		for (int j = 0; j < r.width; j++) {
			inImg[i * r.width + j] = rd[(r.y + i) * 160 + (r.x + j)];
		}
    }
	
	int faceIndex = recognition(inImg, r.height, r.width, dists);
	for (int k = 0; k < 60; k++) {
		if (k < 15) {
			ot[k] = faceIndex;
		} else if (k <= 26) {
			ot[k] = dists[(k - 15) % 6];
		} else if (k < 45) {
			ot[k] = r.x;
		} else {
			ot[k] = r.y;
		}
	}
}
