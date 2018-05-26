// netpbm_texture.c
// Some code for computing four different texture measures 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include "netpbm.h"

#define MIN(X,Y) ((X)<(Y)?(X):(Y))
#define MAX(X,Y) ((X)>(Y)?(X):(Y))

typedef struct patchDiscriptor{
    int row;
    int column;
    int sector;
    double distance ;
    double vector[60];
}vectorCollection;

int segment = 6;
vectorCollection vCollection[256];


//index for discriptors;
int indexMain=0,indexSub=0;
//double laws[25][5][5];


// Compute the gray level co-occurrence matrix for displacement vector (vi, vj) within a rectangular area of image <img>.
// The area's top-left and bottom-rght corner are given by (i1, j1) and (i2, j2), respectively.
// <levels> is the number of intensity levels used for creating the matrix.
Matrix GLCM(Matrix imgMx, int i1, int j1, int i2, int j2, int vi, int vj, int levels)
{ 
	Matrix mxOutput = createMatrix(levels, levels);
	int i, j, minIntensity = 255, maxIntensity = 0, row, column;
  //  printf("iMin=%d  imax=%d   jmin=%d   jmax=%d   vi=%d   vj=%d\n",i1,i2,j1,j2,vi,vj);
    // gets me the location where we can place displacement vectors and still get the final matrix.
	int iMin = MAX(i1 - vi, i1);	// Displacement vector determines the set of pixels to serve as starting points
	int iMax = MIN(i2 - vi, i2);
	int jMin = MAX(j1 - vj, j1);
	int jMax = MIN(j2 - vj, j2);
 //   printf("iMin=%d  imax=%d   jmin=%d   jmax=%d\n",iMin,iMax,jMin,jMax);
	double sum = 0.0;


//printf("imin=%d  imax=%d jmin=%d  jmax=%d\n",iMin,iMax,jMin,jMax);
	for (i = iMin; i <= iMax; i++)
		for (j = jMin; j <= jMax; j++)
		{
            row = (int) imgMx.map[i][j];
            column = (int) imgMx.map[i + vi][j + vj];         
			mxOutput.map[row][column]++;
		}

	// Normalize the GLCM
	for (i = 0; i < levels; i++)
		for (j = 0; j < levels; j++)
			sum += mxOutput.map[i][j];
	
	for (i = 0; i < levels; i++)
		for (j = 0; j < levels; j++)
			mxOutput.map[i][j] /= sum;

	return mxOutput;
}


// Compute vector of features entropy, uniformity, maximum probability, and contrast
// for given gray level co-occurrence matrix <glcm>.
Matrix GLCMFeatureVector(Matrix glcm)
{
	Matrix mxOutput = createMatrix(4, 1);
	double entropy = 0.0, uniform = 0.0, maxprob = 0.0, contrast = 0.0;
	double entropyWeight = 22.0, uniformWeight = 300.0, maxprobWeight = 150.0, contrastWeight = 1.0;
	int i, j;

	for (i = 0; i < glcm.height; i++)
		for (j = 0; j < glcm.width; j++)
		{
			if (glcm.map[i][j] > 0.0)
				entropy -= glcm.map[i][j]*log(glcm.map[i][j]);
			uniform += SQR(glcm.map[i][j]);
			maxprob = MAX(maxprob, glcm.map[i][j]);
			contrast += (double) SQR(i - j)*glcm.map[i][j];
		}

	mxOutput.map[0][0] = entropy*entropyWeight;
	mxOutput.map[1][0] = uniform*uniformWeight;
	mxOutput.map[2][0] = maxprob*maxprobWeight;
	mxOutput.map[3][0] = contrast*contrastWeight;
	return mxOutput;
}

// using global variables to store the structure array that consists of complete vector of each patch. This discriptor
// is assigned to center pixel of the patch, keeping record of it row and column. 
int allPatchVectorFormation(Matrix columnMx,int row,int column,int status){
    int i,j;
    if(indexMain<256 && status ==0){
        for(i=0;i<4;i++){        
            vCollection[indexMain].vector[indexSub]=columnMx.map[i][0];
            indexSub++;
             }
        }

     if(indexMain<256 && status ==1){
        for(i=0;i<4;i++){        
            vCollection[indexMain].vector[indexSub]=columnMx.map[i][0];
            indexSub++;
             }
            indexSub=0;
            vCollection[indexMain].row= row;
            vCollection[indexMain].column= column;
            vCollection[indexMain].distance= 10000; // dummy value
        indexMain++;           
        }
return 0;
}

// randomly generating the centroids
int * randomCentroidSelection(){
    int i,j,*initPt;
    initPt = malloc(sizeof(int)*segment);
     srand(time(0));
    for(i=0;i < segment;i++)
        initPt[i] = rand() % 256;

return initPt;
}

// I maintain clusterCollection. its like stacking the similar discriptors in seperate bins based on segments.
// actual running of k-mean clustering.it take eclidean distance between centroid and each and every pixel.
vectorCollection** kMeanClustering(vectorCollection **centroidRecord, vectorCollection **clusterCollection,int *clusterIndex){
        int i,j,k,l;
        double sum=0,temp=0;
         
        for(i=0;i<256;i++){
                for(k=0;k<segment;k++){
                    for(j=0;j<60;j++){
                             temp = (double)(vCollection[i].vector[j] - centroidRecord[k][1].vector[j]);
                             temp*=temp;  
                            sum = sum + temp;
                        }
                        sum = sqrt(sum);
                        if( sum < vCollection[i].distance){
                                vCollection[i].distance = sum;
                                vCollection[i].sector = k;
                            }
                        sum = 0;
                    }
                for(l=0;l<segment;l++){
                        if(vCollection[i].sector == l){
                                    clusterCollection[l][clusterIndex[l]] = vCollection[i];
                                     clusterIndex[l] = clusterIndex[l] + 1 ;
                                     break;
                                }
                    } 
                
            }
return clusterCollection;
}



// iteration of the k centroids 
vectorCollection** kMeanSelection(vectorCollection **clusterCollection,int * clusterIndex){
    int i,j,*initPt, state = 1,k;
    double sum = 0, temp = 0, movementRecord[segment];
    vectorCollection **centroidRecord;

  centroidRecord = malloc(sizeof(vectorCollection *)*segment);
for(i=0;i<segment;i++)
       centroidRecord[i] = malloc(sizeof(vectorCollection)*2);


    initPt = randomCentroidSelection();

    for(i=0;i<segment;i++){
            centroidRecord[i][1] = vCollection[initPt[i]];
            centroidRecord[i][1].sector = i;
             centroidRecord[i][0] =  centroidRecord[i][1];
        }
        
   
      clusterCollection =  kMeanClustering(centroidRecord,clusterCollection,clusterIndex);
/*	for(i=0;i<segment;i++)
		for(j=0;j<clusterIndex[i];j++)
			printf("**%d\n",clusterCollection[i][j].sector);*/


    while(state){
      for(i=0;i<segment;i++){
            centroidRecord[i][0] = centroidRecord[i][1];
        }

	sum=0;
        for(i=0;i<segment;i++){
                for(k=0;k<60;k++){
                        for(j=0;j < clusterIndex[i]; j++ ){
                                 
                                  sum = sum +  clusterCollection[i][j].vector[k]; 
                            }
				if(sum != 0){
                            		sum = sum/j;
				}
                            centroidRecord[i][1].vector[k] = sum;  
                            sum = 0;
                                   
                    }
                    // dummy value
                     centroidRecord[i][1].row = 10000;
                     centroidRecord[i][1].column = 10000;
                     centroidRecord[i][1].distance = 10000;
                     centroidRecord[i][1].sector = 1700;
            }

/*	for(i=0;i<segment;i++)
			printf("0=%d ##  1=%d\n",centroidRecord[i][0].sector,centroidRecord[i][1].sector);*/
 

        sum = 0;
        for(i=0;i<segment;i++){
               for(j=0;j<60;j++){
                  //           printf("one = %lf      zero = %lf      i=%d\n",centroidRecord[i][1].vector[j],centroidRecord[i][0].vector[j],i);
                             temp =(double)(centroidRecord[i][1].vector[j] - centroidRecord[i][0].vector[j]);
                             temp *=temp;
                             sum = sum + temp ;                               
                    }
		   
               sum = sqrt(sum);
                movementRecord[i] = sum;
                sum = 0;      
            }


        sum =0;
        for(i=0;i<segment;i++){
//	   printf("%lf\n",movementRecord[i]);
            sum = sum + movementRecord[i];
            }  
        sum = (double) sum/segment;

// printf("final movement %lf\n",sum); 
    if(sum == 0){
        state = 0;
       }
    else{ 
            for(i=0; i < segment; i++){          
                      free(clusterCollection[i]);
                      clusterCollection[i]= malloc(sizeof(vectorCollection)*256);
                       clusterIndex[i] = 0;
                    }
       		clusterCollection =  kMeanClustering(centroidRecord,clusterCollection,clusterIndex);
    }
}
return clusterCollection;
}



Image segmentTexture(Image inputImage, int segment){
int i,j,m,n,a[6]={0,1,2,4,8,16},i1,i2,j2,j1,k;
Pixel colour[segment]; 
Matrix ipImageMx,glcmMx,discriptor,finalMatrix;
Image output;
int *clusterIndex;
vectorCollection **clusterCollection;

// collection of each segment


clusterCollection = malloc(sizeof(vectorCollection *)*segment); 
for(i=0;i<segment;i++){
        clusterCollection[i] = malloc(sizeof(vectorCollection)*256); 
}

//index for each cluster

clusterIndex = malloc(sizeof(int)*segment); 
for(i=0;i<segment;i++){
    clusterIndex[i] = 0; 
}


ipImageMx = image2Matrix(inputImage);
finalMatrix = createMatrix(inputImage.height,inputImage.width);
for(i=0;i<inputImage.height;i++)
    for(j=0;j<inputImage.width;j++){
// getting 16 levels
      ipImageMx.map[i][j] = round(ipImageMx.map[i][j]/17.0);      
}


for(i=0;i<16;i++){
	m=i*32;
	n=0;
	for(j=0;j<16;j++){

       glcmMx =  GLCM(ipImageMx,m,n,m+31,n+31,0,1,17);
           discriptor =  GLCMFeatureVector(glcmMx);
           allPatchVectorFormation(discriptor,m+15,n+15,0);

        glcmMx = GLCM(ipImageMx,m,n,m+31,n+31,0,2,17);
            discriptor =  GLCMFeatureVector(glcmMx);  
           allPatchVectorFormation(discriptor,m+15,n+15,0);

       glcmMx = GLCM(ipImageMx,m,n,m+31,n+31,0,4,17);
            discriptor =  GLCMFeatureVector(glcmMx);
           allPatchVectorFormation(discriptor,m+15,n+15,0);

       glcmMx =   GLCM(ipImageMx,m,n,m+31,n+31,0,8,17);
            discriptor =  GLCMFeatureVector(glcmMx);
           allPatchVectorFormation(discriptor,m+15,n+15,0);

       glcmMx =  GLCM(ipImageMx,m,n,m+31,n+31,0,16,17);
            discriptor =  GLCMFeatureVector(glcmMx);
           allPatchVectorFormation(discriptor,m+15,n+15,0);
//--------------------------------------------------------------------------



       glcmMx =   GLCM(ipImageMx,m,n,m+31,n+31,1,1,17);
            discriptor =  GLCMFeatureVector(glcmMx);
           allPatchVectorFormation(discriptor,m+15,n+15,0);

        glcmMx =  GLCM(ipImageMx,m,n,m+31,n+31,2,2,17);
            discriptor =  GLCMFeatureVector(glcmMx);
           allPatchVectorFormation(discriptor,m+15,n+15,0);

        glcmMx =  GLCM(ipImageMx,m,n,m+31,n+31,4,4,17);
            discriptor =  GLCMFeatureVector(glcmMx);
           allPatchVectorFormation(discriptor,m+15,n+15,0);

       glcmMx =   GLCM(ipImageMx,m,n,m+31,n+31,8,8,17);
            discriptor =  GLCMFeatureVector(glcmMx);
           allPatchVectorFormation(discriptor,m+15,n+15,0);

        glcmMx =  GLCM(ipImageMx,m,n,m+31,n+31,16,16,17);
            discriptor =  GLCMFeatureVector(glcmMx);
           allPatchVectorFormation(discriptor,m+15,n+15,0);
//------------------------------------------------------------------------------


        glcmMx =  GLCM(ipImageMx,m,n,m+31,n+31,1,0,17);
            discriptor =  GLCMFeatureVector(glcmMx);
           allPatchVectorFormation(discriptor,m+15,n+15,0);

        glcmMx =  GLCM(ipImageMx,m,n,m+31,n+31,2,0,17);
            discriptor =  GLCMFeatureVector(glcmMx);
           allPatchVectorFormation(discriptor,m+15,n+15,0);

       glcmMx =   GLCM(ipImageMx,m,n,m+31,n+31,4,0,17);
            discriptor =  GLCMFeatureVector(glcmMx);
           allPatchVectorFormation(discriptor,m+15,n+15,0);

       glcmMx= GLCM(ipImageMx,m,n,m+31,n+31,8,0,17);
            discriptor =  GLCMFeatureVector(glcmMx);
           allPatchVectorFormation(discriptor,m+15,n+15,0);

        glcmMx =  GLCM(ipImageMx,m,n,m+31,n+31,16,0,17);
            discriptor =  GLCMFeatureVector(glcmMx);
           allPatchVectorFormation(discriptor,m+15,n+15,1);

		n +=  32;	
		}
	}


clusterCollection = kMeanSelection(clusterCollection,clusterIndex);
	
	for(i=0;i<256;i++){
		i1= vCollection[i].row - 15;
		j1= vCollection[i].column - 15;
		i2= vCollection[i].row + 16;
		j2= vCollection[i].column + 16;
		for(j=i1;j<=i2;j++){
			for(k=j1;k<=j2;k++){
				finalMatrix.map[j][k]= vCollection[i].sector;
				}
			}
	}

for(i=0;i<segment;i++){
	colour[i].r = rand()%256; 
	colour[i].g = rand()%256; 
	colour[i].b = rand()%256; 
	colour[i].i = (colour[i].r + colour[i].g + colour[i].b)/3; 
}

	/*	for(j=0;j<finalMatrix.height;j++){
			for(k=0;k<finalMatrix.width;k++){
				printf("%lf   ",finalMatrix.map[j][k]);
				}
				printf("\n");
			}*/


/*for(i=0;i<segment;i++){
	colour[i]= (256/segment)*(i+1);
}
	for(j=0;j<finalMatrix.height;j++){
			for(k=0;k<finalMatrix.width;k++){
				for(i=0;i<segment;i++){
					if(finalMatrix.map[j][k]== i){
						finalMatrix.map[j][k]= colour[i];
						break;
						}	
					}	
				}
			}
*/
output = createImage(inputImage.height,inputImage.width);

for(i=0;i<finalMatrix.height;i++){
	for(j=0;j<finalMatrix.width;j++){
		for(k=0;k<segment;k++){
		if(finalMatrix.map[i][j]== k){
			output.map[i][j].r = colour[k].r;
			output.map[i][j].g = colour[k].g;
			output.map[i][j].b = colour[k].b;
			output.map[i][j].i = (output.map[i][j].r+ output.map[i][j].g +output.map[i][j].b)/3;
			break;
			}
		}
		
	}
}
return output;
}




void main()
{
//	initLawsMatrices();
	// Your code here...
Image inputImage, outputImage;	
inputImage = readImage("testImage.pgm");
outputImage = segmentTexture(inputImage,segment);
writeImage(outputImage,"texture.ppm");
}



