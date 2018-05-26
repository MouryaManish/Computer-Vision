#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"netpbm.h"

Image shrinking(Image,Image);
Image expanding(Image,Image);
Image threshold(Image);
Matrix labelling(Image,Matrix);

void main(){
int i,j;
Image img,img1,img2;Matrix mx;

img = readImage("text_image.pbm");
img = threshold(img);
/*for(i=0;i<img.height;i++){
	for(j=0;j<img.width;j++){
		printf("%d \n",img.map[i][j].i);
		}
	}*/

//img1 = createImage(img.height,img.width);
mx = createMatrix(img.height,img.width);
mx = labelling(img,mx);
/*for(i=0;i<mx.height;i++){
	for(j=0;j<mx.width;j++){
		printf("%f",mx.map[i][j]);
	
	}
}*/

}


Image threshold(Image img){
int i,j;
for(i=0;i<727;i++){
	for(j=0;j<1261;j++){
		if(img.map[i][j].i <128)
			img.map[i][j].i = 0;
		else
			img.map[i][j].i = 255;
		}
	}
return img;
}





Matrix labelling(Image img,Matrix mxl){
int i,j,count=0;
for(i=0;i<mxl.height;i++){
	for(j=0;j<mxl.width;j++){
		if(img.map[i][j].i = 255){
		
			if((i-1) < 0 && (j-1) < 0){
				mxl.map[i][j] = ++count;}
			else if((i-1)<0 && j<(img.width -1)){
				if(mxl.map[i][j-1]>0)
					mxl.map[i][j]= mxl.map[i][j-1];
				else 
					mxl.map[i][j]=++count;
				}
			else if((j-1)< 0 && i>=1){
				if(mxl.map[i-1][j]>0){
					mxl.map[i][j]=mxl.map[i-1][j];
					}
				else
					mxl.map[i][j]= ++count;
				}
			else{
				if((mxl.map[i-1][j]>0 && mxl.map[i][j-1]>0) && (mxl.map[i-1][j] == mxl.map[i][j-1]) ){
					mxl.map[i][j]=mxl.map[i-1][j];
					}
			/*	else if((mxl.map[i-1][j]>0 && mxl.map[i][j-1]>0) && (mxl.map[i-1][j] != mxl.map[i][j-1]) ){
					mxl.map[i][j] = mxl.map[i-1][j];
							}
				else if(mxl.map[i-1][j]>0 && mxl.map[i][j-1]==0.0 ){
						mxl.map[i][j] = mxl.map[i-1][j];
						}	
				else if(mxl.map[i-1][j]==0 && mxl.map[i][j-1]>0){
						mxl.map[i][j]=mxl.map[i][j-1];
						}
				else {
						mxl.map[i][j] = ++count;
					
						}*/	
					}
				}
			}
		}

return (mxl);
	}
