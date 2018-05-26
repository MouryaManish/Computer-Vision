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

typedef struct{
int height,width,radius;
double ***map;
}Matrix_3D;
	


Matrix_3D create3DMatrix(int height,int width,int radius){
int m,n,k;
Matrix_3D mx;
        mx.map= (double ***) malloc(sizeof(double **)*height);
	for(m=0;m<height;m++){
		mx.map[m]= (double **) malloc(sizeof(double *)*width);
			for(n=0;n<width;n++){
				mx.map[m][n] = (double *)malloc(sizeof(double)*radius);
				for(k=0;k<radius;k++){
					mx.map[m][n][k] = 0.0;
				}
			}
}
mx.height = height;
mx.width = width;
mx.radius = radius;
return mx;
}

void delete_3DMatrix(Matrix_3D mx){
int m,n,k;
	for(m=0;m<mx.height;m++){
//		mx.map[m]= (double **) malloc(sizeof(double *)*width);
			for(n=0;n<mx.width;n++){
				free(mx.map[m][n]) ;
				}
			free(mx.map[m]);
			}
free(mx.map);
}




// Build a Hough parameter map for matrix mxSpatial for detecting straight lines.
// Rows in this map represent the normal alpha and columns represent the distance d from the origin.
// Increasing the size of the map in each dimension improves the resolution of the corresponding parameter.
Matrix_3D houghTransformLines(Matrix mxSpatial, int mapHeight, int mapWidth,Matrix output,int r)
{
	int m, n, angle, dist,h,k,o;
	double alpha/*, maxD = sqrt((double) (SQR(mxSpatial.height) + SQR(mxSpatial.width)))*/;
	Matrix_3D mxParam;
	mxParam = create3DMatrix(mxSpatial.height,mxSpatial.width,r); //(mapHeight,mapWidth,r);
	Matrix sincos = createMatrix(mapHeight, 2);
	// Generate lookup table for sin and cos values to speed up subsequent computation.
	for (angle = 0; angle < mapHeight; angle++)
	{
		alpha = angle * PI/180;
		sincos.map[angle][0] = (double) sin(alpha);
		sincos.map[angle][1] = (double) cos(alpha);
	}
	for (m = 0; m < mxSpatial.height; m++)
		for (n = 0; n < mxSpatial.width; n++)
			if(mxSpatial.map[m][n] >= 200){
	        	    for(o=45; o< r/2 ;o++)
				    for (angle =0; angle < mapHeight; angle++)
				    {
						h =(int) (m - (double)r*sincos.map[angle][0]); // sin vertical
						k = (int)(n - (double)r*sincos.map[angle][1]); // cos horizontal
						if(h>0 && k>0 && h<mxSpatial.height && k<mxSpatial.width){
	                        			        mxParam.map[h][k][o] += mxSpatial.map[m][n];
								output.map[h][k] +=mxSpatial.map[m][n];
						}
				    }
			}	

	deleteMatrix(sincos);
	printf("hough main completed\n");
	return mxParam; 
}


// Test whether entry (m, n) in matrix mx is a local maximum, i.e., is not exceeded by any of its 
// maximally 8 neighbors. Return 1 if true, 0 otherwise.
int isLocalMaximum(Matrix_3D mx,int m, int n,int r)
{
	double strength = mx.map[m][n][r];
	int i, j,z;
	int zMin = (m == 0)? 0:(m - 1);
	int zMax = (m == mx.height -1)? m:(m + 1);
	int iMin = (n == 0)? 0:(n - 1);
	int iMax = (n == mx.width -1)? n:(n + 1); 
	int jMin =(r == 0)?0:(r-1);
        int jMax =(r== mx.radius-1)? r:(r+1);
	for (z = zMin; z <= zMax; z++)
		for (i = iMin; i <= iMax; i++)
			for (j = jMin; j <= jMax; j++)
				if (mx.map[z][i][j] > strength)
					return 0;
	return 1;
}


// Insert a new entry, consisting of vPos, hPos, and strength, into the list of maxima mx.
void insertMaxEntry(Matrix  mx, int vPos, int hPos,int radius, double strength)
{
	int m, n = mx.width - 1;

	while (n > 0 && mx.map[3][n - 1] < strength)
	{
		for (m = 0; m < 4; m++)
			mx.map[m][n] = mx.map[m][n - 1];
		n--;
	}
	mx.map[0][n] = (double) vPos;
	mx.map[1][n] = (double) hPos;
	mx.map[2][n] = (double) radius;
	mx.map[3][n] = strength;
}


// Delete entry number i from the list of maxima mx.
void deleteMaxEntry(Matrix mx, int i)
{
	int m, n;

	for (n = i; n < mx.width - 1; n++)
		for (m = 0; m < 4; m++)
			mx.map[m][n] = mx.map[m][n + 1];

	mx.map[3][mx.width - 1] = -1.0;
}


// Find the <number> highest maxima in a Hough parameter map that are separated by a Euclidean distance 
// of at least <minSeparation> in the map. The result is a three-row matrix with each column representing
// the row, the column, and the strength of one maximum, in descending order of strength.
Matrix findHoughMaxima(Matrix_3D mx, int number, double minSeparation)
{
	int j, m, n, k, l, index, do_not_insert;
	double minSepSquare = SQR(minSeparation);
	double strength;
	Matrix maxima = createMatrix(4, number);

	for (j = 0; j < number; j++)
		maxima.map[3][j] = -1.0;
	


for (k = 0; k < mx.height; k++)
	for (m = 0; m < mx.width; m++)
		for (n = 0; n < mx.radius; n++)
		{
			strength = mx.map[k][m][n];
			index = 0;
			do_not_insert = 0;
			if (strength > 0.0 && strength > maxima.map[3][number - 1] && isLocalMaximum(mx,k, m, n))
			{

				while (index < number && !do_not_insert)
				{
					if (maxima.map[3][index] > 0.0 && SQR(maxima.map[1][index] - (double) m) + SQR(maxima.map[2][index] - (double) n) < minSepSquare)
						if (strength > maxima.map[3][index])
							deleteMaxEntry(maxima, index);
						else
							do_not_insert = 1;
					index++;
				}
				if (!do_not_insert)
					insertMaxEntry(maxima,k, m, n, strength);
				index = 0;
			}
		}
	return maxima;
}


// Read image "desk.ppm" and write Hough transform related output images. 
void main()
{
	int i,j, m, n,radius=0,number=2;
	double minDist =45;
	Image inputImage = readImage("coins.pgm");
	Matrix inputMatrix = image2Matrix(inputImage);
	Image edgeImage, houghImage; 
	Matrix  houghMatrix,maxMatrix;
	Matrix edgeMatrix = createMatrix(inputImage.height, inputImage.width);
	houghMatrix = createMatrix(inputImage.height, inputImage.width);// changed again

	Matrix_3D houghArray;

	 edgeMatrix = edgeDetection("coins.pgm");

	edgeImage = matrix2Image(edgeMatrix, 1, 1.0);
	writeImage(edgeImage, "coins_edge.pgm");
	radius =(int) (edgeImage.height - edgeImage.width)> 0? edgeImage.width/2:( edgeImage.height/2);
	 
	
	houghArray = houghTransformLines(edgeMatrix, 360, 500,houghMatrix,radius);

	houghImage = matrix2Image(houghMatrix, 1, 1.0);
	writeImage(houghImage, "conis_hough.pgm");
	printf("initiated maxima\n");

	maxMatrix = findHoughMaxima(houghArray, number,minDist);
	printf("completed maxima\n");
/*	for(i=0;i<maxMatrix.width;i++){
		for(j=0;j<maxMatrix.height;j++){
			printf("%lf ",maxMatrix.map[j][i]);
		}
		printf("\n");
}*/
	printf("ellipse started\n");
	for (i = 0; i < number; i++){// 20 20 before
			ellipse(inputImage, maxMatrix.map[0][i], maxMatrix.map[1][i],maxMatrix.map[2][i],maxMatrix.map[2][i], 2, 10, 7, 255, 255, 255, 0);
		 }
	writeImage(inputImage, "coins_hough_max.pgm");
	printf("completed ellipse\n");

	delete_3DMatrix(houghArray);
	deleteMatrix(edgeMatrix);
	deleteMatrix(houghMatrix);
	deleteImage(inputImage);
	deleteImage(edgeImage) ;
	
}
