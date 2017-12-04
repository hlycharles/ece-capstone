#include <stdio.h>
#include <stdlib.h>
#include "./haar.h"
#include "./image.h"
#include "../recognition/recognitionModule.h"
#include "./120p.h"

using namespace std;

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

	int res_size=0;
	int *result_size = &res_size;
	int i;

	printf ("-- detecting faces --\r\n");

	for ( i = 0; i < IMAGE_HEIGHT; i+=1 ){
	  detectFaces ( Data[i], result_x, result_y, result_w, result_h, result_size);
	}

	printf("\nresult_size = %d", *result_size);

	for (int j = 0; j < RESULT_SIZE; j++){
	result[j].x = result_x[j];
	result[j].y = result_y[j];
	result[j].width = result_w[j];
	result[j].height = result_h[j];
	}

	for( int i=0 ; i < *result_size ; i++ )
	printf("\n [Test Bench (main) ] detected rects: %d %d %d %d\n",result[i].x,result[i].y,result[i].width,result[i].height);

	// save detection results
	if (*result_size == 0) {
 		return -1;
	}

	MyRect r = result[0];

	// convert to 1D image
	static int inImg[19200];
	static int dists[6];
	for (i = 0; i < r.height; i++) {
		for (int j = 0; j < r.width; j++) {
			inImg[i * r.width + j] = Data[r.y + i][r.x + j];
		}
    }
	
	int faceIndex = recognition(inImg, r.height, r.width, dists);
	for (i = 0; i < 6; i++) {
		printf("dist: %d\n", dists[i]);
	}

	printf("face index: %d\n", faceIndex);

	return 0;
}
