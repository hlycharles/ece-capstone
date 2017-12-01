#include <stdio.h>
#include <stdlib.h>
#include "./haar.h"
#include "image.h"
#include "../recognition/recognitionModule.h"

using namespace std;

void nami(int rd[10], int ot[60]) {
#pragma HLS INTERFACE axis port=rd
#pragma HLS INTERFACE axis port=ot

	int flag;
	int in_flag , in_width , in_height , in_maxgrey;
		 
	printf ("-- entering main function --\r\n");
	printf ("-- loading image --\r\n");

	static int inHolder[10];
	for (int i = 0; i < 10; i++) {
		inHolder[i] = rd[i];
	}

	// 320 X 240 Input image in hex format 
	#include INPUT_IMAGE

	// Arguments to be passed to DUT  
	MyRect result[RESULT_SIZE];
	int result_x[RESULT_SIZE];
	int result_y[RESULT_SIZE];
	int result_w[RESULT_SIZE];
	int result_h[RESULT_SIZE];

	int res_size=0;
	int *result_size = &res_size;
	int i;

	for ( i = 0; i < IMAGE_HEIGHT-1; i+=1 ){
	  detectFaces ( Data[i], result_x, result_y, result_w, result_h, result_size);
	}

	printf ("-- detecting faces --\r\n");

	// std::clock_t start = std::clock();
	detectFaces ( Data[IMAGE_HEIGHT-1], result_x, result_y, result_w, result_h, result_size);
	// duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

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



	/*
	printf("\n-- starting saving result --\n");
	MyRect r = result[0];
    FILE *fp;
    fp = fopen("./result.txt", "w");
	fprintf(fp, "%d\n", r.height);
	fprintf(fp, "%d\n", r.width);
    for (int i = r.y; i < r.y + r.height; i++) {
		for (int j = r.x; j < r.x + r.width; j++) {
			fprintf(fp, "%d\n", Data[i][j]);
		}
    }
    fclose(fp);
	printf("-- DONE starting saving result --\n");
	*/

	// convert to 1D image
	/*
	static int inImg[19200];
	for (int i = 0; i < r.height; i++) {
		for (int j = 0; j < r.width; j++) {
			inImg[i * r.width + j] = Data[r.y + i][r.x + j];
		}
    }
	
	int faceIndex = recognition(inImg, r.height, r.width); */
	for (int k = 0; k < 60; k++) {
		ot[k] = (k < 30) ? r.x : r.y;
	}
}
