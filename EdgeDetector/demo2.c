#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"netpbm.h"

Matrix convolve(Matrix,Matrix);

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
 for(i = 0; i<filterMx.height;i++){
          for(j = 0;j < filterMx.width; j++){
                  filterMx.map[i][j]=3;
                  }
          }


   /*outputImageMx =  createMatrix(inputImage.height,inputImage.width);
   for(i = 0; i<imageMx.height;i++){
	for(j = 0;j < imageMx.width; j++){
		imageMx.map[i][j]=(double)inputImage.map[i][j].i;
		}
	}*/
/*   for(column = 0; column<imageMx.height;column++){
	for(row = 0;row < imageMx.width; row++){
		printf("%f\n",imageMx.map[column][row]);
		}
	}*/
    
    
    /*printf("enter your filter matrix details\n");
    printf("enter row: ");
    scanf("%d",&row);
    printf("enter column: ");
    scanf("%d",&column);
    printf("enter your matrix data\n");
    filterMx =  createMatrix(row,column);
    convMx = createMatrix(row,column);*/
  
/*// input filter values 
	for(i = 0; i<filterMx.height;i++){
	for(j = 0;j < filterMx.width; j++){
		scanf("%lf",&filterMx.map[i][j]);
		}
	}*/

    /*storing the filter by 180 deg rotation for convolution
    for(i=0;i<filterMx.height;i++){
	for(j=0;j<filterMx.width;j++){
              convMx.map[convMx.height - i - 1][convMx.width - j - 1] = filterMx.map[i][j]; 
		}
	}*/


/*	for(i = 0; i<filterMx.height;i++){
	for(j = 0;j < filterMx.width; j++){
		printf("%lf ",convMx.map[i][j]);
		}
	}*/


outputImageMx = convolve(imageMx,filterMx);
}






Matrix convolve(Matrix imageMx,Matrix filterMx){
Matrix resultMx,convMx;
int status,i,j,l,k,sum=0,x,y;
    
convMx = createMatrix(filterMx.height,filterMx.width);

//storing the filter by 180 deg rotation for convolution
    for(i=0;i<filterMx.height;i++){
	for(j=0;j<filterMx.width;j++){
              convMx.map[convMx.height - i - 1][convMx.width - j - 1] = filterMx.map[i][j]; 
		}
	}
// draw input image
/* for(i = 0; i < imageMx.height;i++){
          for(j = 0;j < imageMx.width; j++){
                  printf("%lf       ",imageMx.map[i][j]);
			if(j == (imageMx.width -1))
			    printf("\n");	
                  }
          }
printf("------------------------\n------------------------\n");
//draw convolution filter.
 for(i = 0; i < convMx.height;i++){
          for(j = 0;j < convMx.width; j++){
                  printf("%lf       ",convMx.map[i][j]);
			if(j == (convMx.width -1))
			    printf("\n");	
                  }
          }
printf("------------------------\n------------------------\n");
*/





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
// draw resultant matrices
/* for(i = 0; i < resultMx.height;i++){
          for(j = 0;j < resultMx.width; j++){
                  printf("%lf       ",resultMx.map[i][j]);
			if(j == (resultMx.width -1))
			    printf("\n");	
                  }
          }*/

}




