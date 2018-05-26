// netpbm_hough.c
// Test and demo program for the Hough transform using the netpbm.c library.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "netpbm.h"
#include "hw2.h"

#define MIN(X,Y) ((X)<(Y)?(X):(Y))
#define MAX(X,Y) ((X)>(Y)?(X):(Y))
//#define PI 3.141592653589793


// Build a Hough parameter map for matrix mxSpatial for detecting straight lines.
// Rows in this map represent the normal alpha and columns represent the distance d from the origin.
// Increasing the size of the map in each dimension improves the resolution of the corresponding parameter.
double*** houghTransformLines(Matrix mxSpatial, int mapHeight, int mapWidth)
{
	int m, n, angle, dist,r=60,h=0,k=0;
	double ***mxParam;
        mxParam = (double***) malloc(sizeof(double**)*mapHeight);
	for(m=0;m<mapHeight;m++){
		mxParam[m]= (double**) malloc(sizeof(double*)*mapWidth);
			for(n=0;n<mapWidth;n++)
				mxParam[m][n] = (double *)malloc(sizeof(double)*r);
}
       

	// max distance possible in the image. see text book
	double alpha;

	Matrix sincos = createMatrix(mapHeight, 2);


	for(angle=0;angle<mapHeight;angle++){
		alpha = angle * PI/180;
		sincos.map[angle][0] = (double) sin(alpha);
		sincos.map[angle][1] = (double) cos(alpha);	
	}


	for (m = 0; m < mapHeight; m++) // image height traverse
		for (n = 0; n < mapWidth; n++) // image width traverse
			for(r=10;r<60;r++)
			{
			mxParam[r][m][n] = 0;
			}

// the output space will have angle fix but distance will very.
	for (m = 0; m < mxSpatial.height; m++) // image height traverse
		for (n = 0; n < mxSpatial.width; n++) // image width traverse
			for(r=10;r<50;r++)
				for (angle = 0; angle < mapHeight; angle++) // angle travele in mapheight.
				{
					h = m - (double)r*sincos.map[angle][1];
					k = n - (double)r*sincos.map[angle][0];
					printf("h=%d   k=%d\n",h,k);
	                        //        mxParam[h][k][r] += mxSpatial.map[m][n];
				}

	deleteMatrix(sincos);
	return mxParam;
}





// Read image "desk.ppm" and write Hough transform related output images. 
void main()
{
	int i, m, n, m1, n1, m2, n2,r=60,mapHeight=361,mapWidth=500;
	double gaussFilter[3][3] = {{1.0, 2.0, 1.0}, {2.0, 4.0, 2.0}, {1.0, 2.0, 1.0}};
	double ***mxParam;
	Matrix gauss = createMatrixFromArray(&gaussFilter[0][0], 3, 3); 
	Image inputImage = readImage("desk.ppm");
	Matrix inputMatrix = image2Matrix(inputImage);
	Image edgeImage, houghImage; 
	Matrix edgeMatrix = createMatrix(inputImage.height, inputImage.width);
	Matrix houghMatrix, maxMatrix;
	double maxLength, alpha, dist;

	// Add code for generating edge matrix here!!!
        edgeMatrix = edgeDetection("desk.ppm");

	edgeImage = matrix2Image(edgeMatrix, 1, 1.0);
	writeImage(edgeImage, "desk_edges.pgm");
        mxParam = (double***) malloc(sizeof(double**)*mapHeight);
	for(m=0;m<mapHeight;m++){
		mxParam[m]= (double**) malloc(sizeof(double*)*mapWidth);
			for(n=0;n<mapWidth;n++)
				mxParam[m][n] = (double *)malloc(sizeof(double)*r);
}
	
	//houghMatrix = houghTransformLines(edgeMatrix, 360, 500);
      houghTransformLines(edgeMatrix, 361, 500);
/*	for(m=0;m<edgeMatrix.height;m++)
		for(n=0;n<edgeMatrix.width;n++)
			houghMatrix.map[m][n] = mxParam[m][n][50];
	houghImage = matrix2Image(houghMatrix, 1, 1.0);
	writeImage(houghImage, "desk_hough.pgm");*/



	deleteMatrix(edgeMatrix);
	deleteMatrix(houghMatrix);
	deleteImage(inputImage);
	deleteImage(edgeImage);
	deleteImage(houghImage);
}
