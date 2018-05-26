// netpbm_hough.c
// Test and demo program for the Hough transform using the netpbm.c library.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "netpbm.h"
#include "homework2.h"


#define MIN(X,Y) ((X)<(Y)?(X):(Y))
#define MAX(X,Y) ((X)>(Y)?(X):(Y))
//#define PI 3.141592653589793

//3D Matrix
typedef struct 
{
	int height, width, radius;
	double ***map;
} MatrixTD;

//Create 3D Matrix
MatrixTD createTDMatrix(int height, int width, int radius)
{
	int i, j, k;
	MatrixTD mx;

	mx.map = (double ***) malloc(sizeof(double *)*height);
	for (i = 0; i < height; i++)
	{
		mx.map[i] = (double **) malloc(sizeof(double *)*width);
		for (j = 0; j < width; j++) {
		    mx.map[i][j] = (double *) malloc(sizeof(double *)*radius);
		    for (k = 0; k < radius; k++){
			    mx.map[i][j][k] = 0.0;
	       }
	   }
 }	   
	mx.height = height;
	mx.width = width;
	mx.radius = radius;
	return mx;
	
}

MatrixTD mx;
// Build a Hough parameter map for matrix mxSpatial for detecting straight lines.
// Rows in this map represent the normal alpha and columns represent the distance d from the origin.
// Increasing the size of the map in each dimension improves the resolution of the corresponding parameter.
Matrix houghTransformLines(Matrix mxSpatial, int mapHeight, int mapWidth)
{
	int  radii, angle;
	int radius = 35;
	if(mapHeight < mapWidth){
	    radii = mapHeight/2;
	} else {
	    radii = mapWidth/2;
	}
	double alpha, maxD = sqrt((double) (SQR(mxSpatial.height) + SQR(mxSpatial.width)));
	mx = createTDMatrix(mapHeight, mapWidth, 100);  
	
	Matrix mxParam = createMatrix(mapHeight, mapWidth);
	Matrix sincos = createMatrix(mapHeight, 2);

	// Generate lookup table for sin and cos values to speed up subsequent computation.
	for (angle = 0; angle < 360; angle++)
	{
		alpha = angle * (PI / 180);
		sincos.map[angle][0] = (double) sin(alpha);
		sincos.map[angle][1] = (double) cos(alpha);
	}
	

	for (int m = 0; m < mxSpatial.height; m++){
		for (int n = 0; n < mxSpatial.width; n++){
		    for(int  r = radius; r < radii; r++){
			    for (int angle = 0; angle < 360; angle++) {
			        int x =  (int) (m - (double) r*sincos.map[angle][0]);
			        int y = (int) (n - (double) r*sincos.map[angle][1]);
			        if((x >= 0 && x <mapHeight)  && (y >=0 && y <mapWidth)){
			            mx.map[x][y][r] += mxSpatial.map[m][n]/r;
			            mxParam.map[x][y] += mxSpatial.map[m][n]/r;
			        }
			}
       }
   }
}   
        
	deleteMatrix(sincos);
	return mxParam; 
}


// Test whether entry (m, n) in matrix mx is a local maximum, i.e., is not exceeded by any of its 
// maximally 8 neighbors. Return 1 if true, 0 otherwise.
int isLocalMaximum(MatrixTD mx, int m, int n, int k)
{ 
	double strength = mx.map[m][n][k];
	int i, j, l; 
	int iMin = (m == 0)? 0:(m - 1);
	int iMax = (m == mx.height -1)? m:(m + 1);
	int jMin = (n == 0)? 0:(n - 1);
	int jMax = (n == mx.width -1)? n:(n + 1);
	int kMin = (k == 0)? 0:(k - 1);
	int kMax = (k == mx.radius -1)? k:(k + 1);

	for (i = iMin; i <= iMax; i++) {
		for (j = jMin; j <= jMax; j++) {
		    for(l = kMin; l <= kMax; l++)  {   
			   if (mx.map[i][j][l] > strength) {
				   return 0;
				} else {   
	                return 1;
                }
            }
       }
   }
}        


// Insert a new entry, consisting of vPos, hPos, and strength, into the list of maxima mx.
void insertMaxEntry(Matrix mx, int vPos, int hPos, int radius, double strength)
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

	for (n = i; n < mx.width - 1; n++) {
		for (m = 0; m < 4; m++) {
			mx.map[m][n] = mx.map[m][n + 1];
        }
    }    
	mx.map[3][mx.width - 1] = -1.0;
}


// Find the <number> highest maxima in a Hough parameter map that are separated by a Euclidean distance 
// of at least <minSeparation> in the map. The result is a three-row matrix with each column representing
// the row, the column, and the strength of one maximum, in descending order of strength.
Matrix findHoughMaxima(MatrixTD mx, int number, double minSeparation)
{
	int k, j, m, n, index, do_not_insert;
	double minSepSquare = SQR(minSeparation);
	double strength;
	Matrix maxima = createMatrix(4, number);

	for (j = 0; j < number; j++)
		maxima.map[3][j] = -1.0;

	for (m = 0; m < mx.height; m++) 
		for (n = 0; n < mx.width; n++) 
		    for(k = 0; k < mx.radius; k++){
		       strength = mx.map[m][n][k];
			   index = 0;
			   do_not_insert = 0;
			if (strength > 0.0 && strength > maxima.map[3][number - 1] && isLocalMaximum(mx, m, n, k))
			{
				while (index < number && !do_not_insert)
				{
					if (maxima.map[3][index] > 0.0 && SQR(maxima.map[0][index] - (double) m) + SQR(maxima.map[1][index] - (double) n) + SQR(maxima.map[2][index] - (double) k) < minSepSquare) 
						if (strength > maxima.map[3][index])
							deleteMaxEntry(maxima, index);
						else
							do_not_insert = 1;
					index++;
				}
				if (!do_not_insert)
					insertMaxEntry(maxima, m, n, k, strength);
				index = 0;
			}
		}
	return maxima;
}



// Read image "desk.ppm" and write Hough transform related output images. 
int main(void)
{
	//int i, m, n, m1, n1, m2, n2;
	//double gaussFilter[3][3] = {{1.0, 2.0, 1.0}, {2.0, 4.0, 2.0}, {1.0, 2.0, 1.0}};
	//Matrix gauss = createMatrixFromArray(&gaussFilter[0][0], 3, 3);
    //gauss = gauss; 
	Image inputImage = readImage("desk.ppm");
	Matrix inputMatrix = image2Matrix(inputImage);
	inputMatrix = inputMatrix;
	Image edgeImage, houghImage; 
	Matrix edgeMatrix = createMatrix(inputImage.height, inputImage.width);
	Matrix houghMatrix, maxMatrix;
	

	// Add code for generating edge matrix here!!!
    
    edgeMatrix = canny(inputImage);
	edgeImage = matrix2Image(edgeMatrix, 1, 1.0);
	writeImage(edgeImage, "desk_edges.pgm");
	
	houghMatrix = houghTransformLines(edgeMatrix, 360, 500);
	houghImage = matrix2Image(houghMatrix, 1, 1.0);
	
	writeImage(houghImage, "desk_hough.pgm");
    
	maxMatrix = findHoughMaxima(mx, 4, 50.0);
	for (int i = 0; i < maxMatrix.width; i++)
		ellipse(houghImage, maxMatrix.map[0][i], maxMatrix.map[1][i], 20, 20, 2, 10, 7, 255, 255, 255, 0); 
	    writeImage(houghImage, "desk_hough_max.ppm");
     
    for (int i = 0; i < maxMatrix.width; i++)
		ellipse(inputImage, maxMatrix.map[0][i], maxMatrix.map[1][i], maxMatrix.map[2][i], maxMatrix.map[2][i], 2, 10, 7, 0, 0 , 0, 0); 
	    writeImage(inputImage, "desk_hough_circles.ppm");

	deleteMatrix(edgeMatrix);
	deleteMatrix(houghMatrix);
	deleteImage(inputImage);
	deleteImage(edgeImage);
	deleteImage(houghImage);
	
	return 0;
}
