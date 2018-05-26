#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"netpbm.h"

Matrix convolve(Matrix,Matrix);
Image sobel(Image);
Matrix canny(Image);
Matrix edgeDetection(char *);

//   edgeDetection("gray.pgm","SobelImage.pgm","CannyImage.pgm");

/**** canny detector ****/
Matrix canny(Image inputImage){
Matrix magnitudeMx,orientationMx,sectorMx,cannyResultMx,tempMx,imageMx,gaussFilterMx,sobelVFilterMx, sobelHFilterMx,sobelV,sobelH;
double vFilterData[9]={1,2,1,0,0,0,-1,-2,-1},hFilterData[9]={1,0,-1,2,0,-2,1,0,-1},gaussData[9]={1,1,1,1,1,1,1,1,1},sum=0;
Image cannyImage;
int i,j;

imageMx = createMatrix(inputImage.height,inputImage.width);
   for(i = 0; i<imageMx.height;i++){
	for(j = 0;j < imageMx.width; j++){
		imageMx.map[i][j]=(double)inputImage.map[i][j].i;
		}
	}

gaussFilterMx = createMatrix(3,3);
sobelVFilterMx = createMatrix(3,3);
sobelHFilterMx = createMatrix(3,3);

// putting data in the horizontal and vertical filter.
// smoothing gauss filter building
for(i=0;i<9;i++){  //smoothing 
	sum = sum + gaussData[i];
	}

for(i=0;i<gaussFilterMx.height;i++)
	for(j=0;j<gaussFilterMx.width;j++){
	     gaussFilterMx.map[i][j] = (1/sum) * gaussData[j+(i*3)];
	}

// testing gauss
/*for(i=0;i<gaussFilterMx.height;i++)
	for(j=0;j<gaussFilterMx.width;j++){
	     printf("%lf  ", gaussFilterMx.map[i][j]);
	}*/


for(i=0;i<sobelVFilterMx.height;i++)
	for(j=0;j<sobelVFilterMx.width;j++){
	     sobelVFilterMx.map[i][j] = vFilterData[j+(i*3)];
	}
for(i=0;i<sobelHFilterMx.height;i++)
	for(j=0;j<sobelHFilterMx.width;j++){
	     sobelHFilterMx.map[i][j] = hFilterData[j+(i*3)];
	}
//imageMx = --------------------------------------------
imageMx = convolve(imageMx,gaussFilterMx);
sobelV = convolve(imageMx,sobelVFilterMx);
sobelH = convolve(imageMx,sobelHFilterMx);

magnitudeMx = createMatrix(sobelV.height,sobelV.width);
orientationMx = createMatrix(sobelV.height,sobelV.width);

// building magnitude matrix
for(i=0;i<sobelH.height;i++){
	for(j=0;j<sobelH.width;j++){
		magnitudeMx.map[i][j] = sqrt(pow((sobelH.map[i][j]),2) + pow((sobelV.map[i][j]),2)); 
		}
	}

// building orientation matrix in degree and negative angles is converted to positive
for(i=0;i<sobelH.height;i++){
	for(j=0;j<sobelH.width;j++){
		orientationMx.map[i][j] = (((atan2(sobelH.map[i][j],sobelV.map[i][j])) *(180.0/PI))<0)? (360 - ((atan2(sobelH.map[i][j],sobelV.map[i][j])) *(180.0/PI))):((atan2(sobelH.map[i][j],sobelV.map[i][j])) *(180.0/PI)) ;
		}
	}
// orientation testing
/*for(i=0;i<orientationMx.height;i++){
	for(j=0;j<orientationMx.width;j++){
		printf("%5.2lf  ",orientationMx.map[i][j]);
        if(orientationMx.width == 8)
            printf("\n"); 
		}
	}*/

// testing magnitude
/*for(i=0;i<magnitudeMx.height;i++){
	for(j=0;j<magnitudeMx.width;j++){
		printf("%5.2lf  ",magnitudeMx.map[i][j]);
        if(magnitudeMx.width == 8)
            printf("\n"); 
		}
	}*/

sectorMx = createMatrix(orientationMx.height,orientationMx.width);
for(i=0;i<sectorMx.height;i++){
   for(j=0;j<sectorMx.width;j++){
            if((orientationMx.map[i][j] >= 67.5 && orientationMx.map[i][j] < 112.5) || (orientationMx.map[i][j] >= 247.5 && orientationMx.map[i][j] < 292.5)){
                        sectorMx.map[i][j] = 2.0;                                
                }
            else if((orientationMx.map[i][j] >= 112.5 && orientationMx.map[i][j] < 157.5) || (orientationMx.map[i][j] >= 292.5 && orientationMx.map[i][j] < 337.5)){
                        sectorMx.map[i][j] = 3.0;                                
                }
            else if((orientationMx.map[i][j] >= 157.5 && orientationMx.map[i][j] < 202.5) || (orientationMx.map[i][j] >= 337.5 && orientationMx.map[i][j] < 22.5)){
                        sectorMx.map[i][j] = 0.0;                                
                }
            else if((orientationMx.map[i][j] >= 202.5 && orientationMx.map[i][j] < 247.5) || (orientationMx.map[i][j] >= 22.5 && orientationMx.map[i][j] < 67.5)){
                        sectorMx.map[i][j] = 1.0;                                
                }

        }
    }
/*// orientation testing
for(i=0;i<orientationMx.height;i++){
	for(j=0;j<orientationMx.width;j++){
		printf("%5.2lf  ",sectorMx.map[i][j]);
        if(orientationMx.width == 8)
            printf("\n"); 
		}
	}*/



cannyResultMx = createMatrix(magnitudeMx.height,magnitudeMx.width);
for(i=0;i<cannyResultMx.height;i++){
	for(j=0;j<cannyResultMx.width;j++){
		cannyResultMx.map[i][j] = magnitudeMx.map[i][j];
		}
	}

// as we know that it is 3x3 window
// reducing the edge size to one
for(i=0;i<(sectorMx.height-2);i++){
for(j=0;j<(sectorMx.width-2);j++){
    if(sectorMx.map[i+1][j+1] == 2){
         if((magnitudeMx.map[i+1][j] > magnitudeMx.map[i+1][j+1]) || (magnitudeMx.map[i+1][j+2] > magnitudeMx.map[i+1][j+1])){
                cannyResultMx.map[i+1][j+1] = 0; 
            }   
        }
   
    if(sectorMx.map[i+1][j+1] == 3){
         if((magnitudeMx.map[i+2][j] > magnitudeMx.map[i+1][j+1]) || (magnitudeMx.map[i][j+2] > magnitudeMx.map[i+1][j+1])){
                cannyResultMx.map[i+1][j+1] = 0; 
            }   
        } 
    

     if(sectorMx.map[i+1][j+1] == 0){
         if((magnitudeMx.map[i][j+1] > magnitudeMx.map[i+1][j+1]) || (magnitudeMx.map[i+2][j+1] > magnitudeMx.map[i+1][j+1])){
                cannyResultMx.map[i+1][j+1] = 0; 
            }   
        } 


    if(sectorMx.map[i+1][j+1] == 1){
         if((magnitudeMx.map[i][j] > magnitudeMx.map[i+1][j+1]) || (magnitudeMx.map[i+2][j+2] > magnitudeMx.map[i+1][j+1])){
                cannyResultMx.map[i+1][j+1] = 0; 
            }   
        } 

         
    }
}

// testing cannyResultMx
/*for(i=0;i<cannyResultMx.height;i++){
	for(j=0;j<cannyResultMx.width;j++){
		printf("%5.2lf  ",cannyResultMx.map[i][j]);
        if(orientationMx.width == 8)
            printf("\n"); 
		}

	}*/
//stage 4
tempMx = createMatrix(cannyResultMx.height,cannyResultMx.width);
//labelMx = createMatrix(cannyResultMx.height,cannyResultMx.width);


/* assigning candidates*/
for(i=0;i<cannyResultMx.height;i++){
	for(j=0;j< cannyResultMx.width;j++){
       //theta1 = 30 and theta2 = 60
        if(cannyResultMx.map[i][j] > 60){
            tempMx.map[i][j] = 1;
                }
        else if(cannyResultMx.map[i][j]<= 20){
                tempMx.map[i][j] = 0;           
            }
        else{
             tempMx.map[i][j] = 2;  // not decided  
                }
        }
    }


/*//testing temp
for(i=0;i<tempMx.height;i++){
	for(j=0;j< tempMx.width;j++){
		printf("%4.2lf   ",tempMx.map[i][j]);
		if(tempMx.width == 10)
			printf("\n");
	}
}*/

/*for(i=0;i<(tempMx.height -3);i++){
	for(j=0;j< (tempMx.width-3);j++){
		if(tempMx.map[i+1][j+1] == 2){
                if(tempMx.map[i][j]==1 || tempMx.map[i][j+1] == 1 || tempMx.map[i][j+2] == 1 || tempMx.map[i+1][j] == 1 ||  tempMx.map[i+1][j+ 2] == 1 || 
			tempMx.map[i+2][j] == 1 ||  tempMx.map[i+2][j+1] == 1 ||  tempMx.map[i+2][j+2] == 1){
                                     tempMx.map[i+1][j+1] == 1;                           
                     }
		     else{
				tempMx.map[i+1][j+1] = 0;}          
            }
	           		
	}
}*/

for(i=0;i<tempMx.height;i++){
	for(j=0;j < tempMx.width;j++){
        if(i==0 && j==0)// first element
    		continue;
	else if(i==0 && j>0 && tempMx.map[i][j] == 2.0 ){ // top row element
			if(tempMx.map[i][j-1] == 1){
               tempMx.map[i][j] == 1;     }
            else
                tempMx.map[i][j] == 0; 
       		}

    else if(i>0 && j == 0 && tempMx.map[i][j] == 2.0 ){ //leftmost column
			if(tempMx.map[i-1][j] == 1 || tempMx.map[i-1][j+1] == 1){
               tempMx.map[i][j] == 1;}
            else 
                tempMx.map[i][j] == 0; 
       		}

    else if((i>0 && j == (tempMx.width -1)) && tempMx.map[i][j] == 2.0 ){ // rightmost column
			if(tempMx.map[i][j-1] == 1 || tempMx.map[i-1][j-1] == 1 || tempMx.map[i-1][j] == 1 ){
               tempMx.map[i][j] == 1;}
            else
                tempMx.map[i][j] == 0; 
       		}
    else{
                if(tempMx.map[i][j] == 2.0){
			if(tempMx.map[i][j-1] == 1 || tempMx.map[i-1][j-1] == 1 || tempMx.map[i -1][j] == 1  || tempMx.map[i-1][j+1] == 1 ){
                       tempMx.map[i][j] = 1;

				}   
               		 else{
                    		tempMx.map[i][j] = 0;
					 }     
               
      			}


    		}
	}
}

/*///testing temp
for(i=0;i<tempMx.height;i++){
	for(j=0;j< tempMx.width;j++){
		if(tempMx.map[i][j] == 2)
			printf("%2.2lf  i=%d  j=%d\n",tempMx.map[i][j],i,j);
		if(tempMx.width == 10)
			printf("\n");
	}
}*/


for(i=0;i<tempMx.height;i++){
	for(j=0;j< tempMx.width;j++){
		cannyResultMx.map[i][j] = cannyResultMx.map[i][j] * tempMx.map[i][j];
	}
}


/*cannyImage = createImage(cannyResultMx.height,cannyResultMx.width);

for(i=0;i<cannyImage.height;i++){
	for(j=0;j<cannyImage.width;j++){
		cannyImage.map[i][j].i = cannyResultMx.map[i][j];	 
        cannyImage.map[i][j].r = cannyResultMx.map[i][j];	
        cannyImage.map[i][j].g = cannyResultMx.map[i][j];	
        cannyImage.map[i][j].b = cannyResultMx.map[i][j];		 
		}
	}*/

//testing final image
/*for(i=0;i<cannyResultMx.height;i++){
	for(j=0;j< cannyResultMx.width;j++){
		printf("   %lf   ",cannyResultMx.map[i][j]);
		if(cannyResultMx.width == 10)
			printf("\n");
	}
}*/

deleteMatrix(magnitudeMx);
deleteMatrix(orientationMx);
deleteMatrix(sectorMx);
//deleteMatrix(cannyResultMx);
deleteMatrix(tempMx);
deleteMatrix(sobelV);
deleteMatrix(sobelH);
return (cannyResultMx);
}

/****** sobel detector ******/
Image sobel(Image inputImage){
Matrix imageMx,sobelVFilterMx,sobelHFilterMx,resultMagMx,gaussFilterMx,sobelH,sobelV;
int i,j;
double vFilterData[9]={1,2,1,0,0,0,-1,-2,-1},hFilterData[9]={1,0,-1,2,0,-2,1,0,-1},gaussData[9]={1,1,1,1,1,1,1,1,1},sum=0,max=0,min=10000,temp=0;
Image sobelImage;

imageMx = createMatrix(inputImage.height,inputImage.width);
   for(i = 0; i<imageMx.height;i++){
	for(j = 0;j < imageMx.width; j++){
		imageMx.map[i][j]=(double)inputImage.map[i][j].i;
		}
	}

gaussFilterMx = createMatrix(3,3);
sobelVFilterMx = createMatrix(3,3);
sobelHFilterMx = createMatrix(3,3);

// putting data in the horizontal and vertical filter.
// smoothing gauss filter building
for(i=0;i<9;i++){  //smoothing 
	sum = sum + gaussData[i];
	}

for(i=0;i<gaussFilterMx.height;i++)
	for(j=0;j<gaussFilterMx.width;j++){
	     gaussFilterMx.map[i][j] = (1/sum) * gaussData[j+(i*3)];
	}

// testing gauss
/*for(i=0;i<gaussFilterMx.height;i++)
	for(j=0;j<gaussFilterMx.width;j++){
	     printf("%lf  ", gaussFilterMx.map[i][j]);
	}*/


for(i=0;i<sobelVFilterMx.height;i++)
	for(j=0;j<sobelVFilterMx.width;j++){
	     sobelVFilterMx.map[i][j] = vFilterData[j+(i*3)];
	}
for(i=0;i<sobelHFilterMx.height;i++)
	for(j=0;j<sobelHFilterMx.width;j++){
	     sobelHFilterMx.map[i][j] = hFilterData[j+(i*3)];
	}
//imageMx = --------------------------------------------
imageMx = convolve(imageMx,gaussFilterMx);
sobelV = convolve(imageMx,sobelVFilterMx);
sobelH = convolve(imageMx,sobelHFilterMx);
/*for(i=0;i<sobelVFilterMx.height;i++)
	for(j=0;j<sobelVFilterMx.width;j++){
		printf("%lf  ",sobelVFilterMx.map[i][j]);
	}*/

resultMagMx = createMatrix(sobelV.height,sobelV.width);

for(i=0;i<sobelH.height;i++){
	for(j=0;j<sobelH.width;j++){
		resultMagMx.map[i][j] = sqrt(pow((sobelH.map[i][j]),2) + pow((sobelV.map[i][j]),2)); 
	/*	if(temp <= resultMagMx.map[i][j]){
			temp = resultMagMx.map[i][j];
			max = temp;
			}
		else{
			if(resultMagMx.map[i][j] < min)
				min = resultMagMx.map[i][j];
			}*/
		}
	}

sobelImage = createImage(resultMagMx.height,resultMagMx.width);

//I am not using linear scaling as it deteriorates the image 
/*for(i=0;i<resultMagMx.height;i++){
	for(j=0;j<resultMagMx.width;j++){
			resultMagMx.map[i][j]= 255/(max-min)*(resultMagMx.map[i][j] - min);
			}
		}*/



for(i=0;i<sobelImage.height;i++){
	for(j=0;j<sobelImage.width;j++){
		sobelImage.map[i][j].i = resultMagMx.map[i][j];	 
		sobelImage.map[i][j].r = resultMagMx.map[i][j];	 
		sobelImage.map[i][j].g = resultMagMx.map[i][j];	 
		sobelImage.map[i][j].b = resultMagMx.map[i][j];	 
		}
	}

// threshold theta = 70 for sobel
/*for(i=0;i<sobelImage.height;i++){
	for(j=0;j<sobelImage.width;j++){
		sobelImage.map[i][j].i = ((sobelImage.map[i][j].i <= 70)? 0:255); 	 
		sobelImage.map[i][j].r = sobelImage.map[i][j].i; 	 
		sobelImage.map[i][j].g = sobelImage.map[i][j].i;
		sobelImage.map[i][j].b = sobelImage.map[i][j].i;	 
		}
	}*/

deleteMatrix(imageMx);
deleteMatrix(sobelV);
deleteMatrix(sobelH);
deleteMatrix(gaussFilterMx);
deleteMatrix(sobelVFilterMx);
deleteMatrix(sobelHFilterMx);
return sobelImage;
}




Matrix convolve(Matrix imageMx,Matrix filterMx){
Matrix resultMx,convMx;
int status=0,i,j,l,k,sum=0,x,y;
    
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

if(status){
  for(i=0;i<(imageMx.height - convMx.height+1);i++){
          for(j=0;j<(imageMx.width - convMx.width-1);j++){
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
  for(i=0;i<(imageMx.height - convMx.height+1);i++){
          for(j=0;j<(imageMx.width - convMx.width-1);j++){
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

Matrix edgeDetection(char *inputFilename){
	  Matrix imageMx,filterMx,convMx,cannyMx;
    int row,column,i,j;
    Image inputImage,sobelImage,cannyImage;
    
    inputImage = readImage(inputFilename);
//sobelImage = sobel(inputImage);
cannyMx = canny(inputImage);
//writeImage(sobelImage,sobelFilename);
//writeImage(cannyImage,cannyFilename);
//deleteImage(sobelImage);
deleteImage(inputImage);
return cannyMx;
}


