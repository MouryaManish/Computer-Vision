#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"netpbm.h"

Matrix convolve(Matrix,Matrix);
void sobel(Matrix);
void main(){
    Matrix imageMx,filterMx,convMx,outputImageMx;
    int row,column,i,j,status,count;
    Image inputImage,tempImage;
imageMx = createMatrix(5,7);
filterMx = createMatrix(2,2);

    
count=1;

 for(i = 0; i<imageMx.height;i++){
          for(j = 0;j < imageMx.width; j++){
                  imageMx.map[i][j]=count++;
                  }
          }
 sobel(imageMx);
}






void sobel(Matrix imageMx){
Matrix sobelHFilterMx,sobelVFilterMx,sobelH,sobelV,resultMagMx;
int i,j,vFilterData[9]={1,2,1,0,0,0,-1,-2,-1},hFilterData[9]={1,0,-1,2,0,-2,1,0,-1};

sobelHFilterMx = createMatrix(3,3);
sobelVFilterMx = createMatrix(3,3);

// putting data in the horizontal and vertical filter.
for(i=0;i<sobelVFilterMx.height;i++)
	for(j=0;j<sobelVFilterMx.width;j++){
	     sobelVFilterMx.map[i][j] = vFilterData[j+(i*3)];
	}
for(i=0;i<sobelHFilterMx.height;i++)
	for(j=0;j<sobelHFilterMx.width;j++){
	     sobelHFilterMx.map[i][j] = hFilterData[j+(i*3)];
	}

sobelV = convolve(imageMx,sobelVFilterMx);
sobelH = convolve(imageMx,sobelHFilterMx);

//------------testing-----------------------------------

for(i=0;i<sobelV.height;i++)
	for(j=0;j<sobelV.width;j++){
		printf("%lf  ",sobelV.map[i][j]);
		if(j==(sobelV.width-1))
			printf("\n");
	}
printf("\n-----------------------------\n---------------------------------\n");
//-----------------------------------------------------


//------------testing-----------------------------------
for(i=0;i<sobelH.height;i++)
	for(j=0;j<sobelH.width;j++){
		printf("%lf  ",sobelH.map[i][j]);
		if(j==(sobelH.width-1))
			printf("\n");
	}
printf("\n-----------------------------\n---------------------------------\n");
//-----------------------------------------------------

resultMagMx = createMatrix(sobelV.height,sobelV.width);

//------------testing-----------------------------------
/*for(i=0;i<sobelVFilterMx.height;i++)
	for(j=0;j<sobelVFilterMx.width;j++){
		printf("%lf  ",sobelVFilterMx.map[i][j]);
	}
printf("\n-----------------------------\n---------------------------------\n");*/
//-----------------------------------------------------

for(i=0;i<sobelH.height;i++){
	for(j=0;j<sobelH.width;j++){
		resultMagMx.map[i][j] = sqrt(pow((sobelH.map[i][j]),2) + pow((sobelV.map[i][j]),2)); 
		}
	}

//------------testing-----------------------------------
for(i=0;i<resultMagMx.height;i++)
	for(j=0;j<resultMagMx.width;j++){
		printf("%lf  ",resultMagMx.map[i][j]);
		if(j==(resultMagMx.width-1))
			printf("\n");
	}
printf("\n-----------------------------\n---------------------------------\n");
//-----------------------------------------------------

}


Matrix convolve(Matrix imageMx,Matrix filterMx){
Matrix resultMx,convMx;
int status=0,i,j,l,k,x,y;
double sum=0;
    
convMx = createMatrix(filterMx.height,filterMx.width);
/*storing the filter by 180 deg rotation for convolution*/
    for(i=0;i<filterMx.height;i++){
	for(j=0;j<filterMx.width;j++){
              convMx.map[convMx.height - i - 1][convMx.width - j - 1] = filterMx.map[i][j]; 
		}
	}

x = (int)round(convMx.height/2.0) - 1;
y = (int)round(convMx.width/2.0)-1;
resultMx = createMatrix(imageMx.height,imageMx.width);
if((convMx.width%2 != 0) && (convMx.height%2 != 0))
	status = 1;// indication of odd width and height.
//printf("status = %d\n",status);

if(status){
  for(i=0;i<(imageMx.height - convMx.height + 1);i++){
          for(j=0;j<(imageMx.width - convMx.width +1);j++){
                 for(l=0;l<convMx.height;l++){
                          for(k=0;k<convMx.width;k++){
                                  sum = (sum + imageMx.map[i+l][j+k] *  convMx.map[l][k]) ;
                                 }
                                
                         }
                          resultMx.map[x+i][j+y] = sum;
                          sum=0;
                  }
          }
	
	}
 
 else{
  for(i=0;i<(imageMx.height - convMx.height + 1);i++){
          for(j=0;j<(imageMx.width - convMx.width +1);j++){
                 for(l=0;l<convMx.height;l++){
                          for(k=0;k<convMx.width;k++){
                                  sum = (sum + imageMx.map[i+l][j+k] *  convMx.map[l][k]) ;
                                }

                         }
                          resultMx.map[i][j] = sum;
                          sum=0;
                  }
          }
    

	}
return resultMx;

}

