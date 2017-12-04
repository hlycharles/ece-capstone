#ifndef __HAAR_H__
#define __HAAR_H__

#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "ap_int.h"

const int ROWS = 25;
const int COLS = 25;

const int NUM_BANKS = 12;
const int SIZE = 2913;

typedef ap_uint<18> uint18_t;
typedef ap_uint<10> uint10_t;
typedef ap_uint<1>  bit;
typedef ap_uint<5> uint5_t;

#ifndef __PARAMETERS__
#define __PARAMETERS__

#define IMAGE_HEIGHT 120
#define IMAGE_WIDTH 160
#define IMAGE_MAXGREY 255
#define IMAGE_SIZE  ( IMAGE_HEIGHT * IMAGE_WIDTH )

#define RESULT_SIZE 100

#define TOTAL_NODES 2913
#define TOTAL_STAGES 25
#define TOTAL_COORDINATES TOTAL_NODES*12
#define TOTAL_WEIGHTS TOTAL_NODES*3

#define WINDOW_SIZE 25
#define SQ_SIZE 2
#define PYRAMID_HEIGHT 12

#define OUTPUT_FILENAME "Output.pgm"

#endif

typedef ap_uint<13> int_I;  
typedef ap_uint<21> int_SI;
typedef ap_uint<18> int_II;             
typedef ap_uint<26> int_SII;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MyPoint
{
    int x;
    int y;
}
MyPoint;

typedef struct
{
    int width;
    int height;
}
MySize;

typedef struct
{
    int x; 
    int y;
    int width;
    int height;
}
MyRect;

typedef struct 
{
        int width;
        int height;
        int maxgrey;
        int flag;
}
MyInputImage;

void integralImages( int height, int width, unsigned char Data[IMAGE_HEIGHT][IMAGE_WIDTH], int Sum[IMAGE_HEIGHT][IMAGE_WIDTH]);

void imageScaler        ( int src_height,
			  int src_width,
                          unsigned char Data[IMAGE_HEIGHT][IMAGE_WIDTH],
                          int dest_height,
			  int dest_width,
                          unsigned char IMG1_data[IMAGE_HEIGHT][IMAGE_WIDTH] 
                        );

void processImage       ( float factor,
                          int sum_row,
                          int sum_col,
                          int *AllCandidates_x,
                          int *AllCandidates_y,
                          int *AllCandidates_w,
                          int *AllCandidates_h,
                          int *AllCandidates_size,
                          unsigned char IMG1_data[IMAGE_HEIGHT][IMAGE_WIDTH],
                          MySize winSize
                        );


int cascadeClassifier  ( int SUM1_data[IMAGE_HEIGHT][IMAGE_WIDTH],
                         MyPoint pt
                       ); 

int weakClassifier      ( int stddev,
                          ap_uint<25> coord[12],
                          int haar_counter,
                          int w_id                      
                        );

void groupRectangles    ( MyRect *rectList, 
                          int *rectList_size, 
                          int groupThreshold, 
                          float eps
                        );

unsigned int int_sqrt   ( unsigned int  value
                        );

int  myRound ( float value );

void detectFaces( unsigned char Data[IMAGE_WIDTH], 
                  int _result_x[RESULT_SIZE], 
                  int _result_y[RESULT_SIZE], 
                  int _result_w[RESULT_SIZE], 
                  int _result_h[RESULT_SIZE], 
                  int *result_size);
		
#ifdef __cplusplus
}
#endif

#endif
