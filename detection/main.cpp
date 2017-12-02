#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include "./haar.h"
#include "image.h"
#include "../recognition/recognitionModule.h"
#include "./120p.h"

using namespace std;

int in_flag =1;
int in_width = 160;
int in_height = 120;
int in_maxgrey = 255;

int main() {
	int flag;
		 
	printf ("-- entering main function --\r\n");
	printf ("-- loading image --\r\n");

	double duration;

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

	std::clock_t start = std::clock();
	detectFaces ( Data[IMAGE_HEIGHT-1], result_x, result_y, result_w, result_h, result_size);
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

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
		return 0;
	}

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

	// convert to 1D image
	int inImg[r.width * r.height];
	for (int i = 0; i < r.height; i++) {
		for (int j = 0; j < r.width; j++) {
			inImg[i * r.width + j] = Data[r.y + i][r.x + j];
		}
    }
	
	recognition(inImg, r.height, r.width);

	return 0;
}
