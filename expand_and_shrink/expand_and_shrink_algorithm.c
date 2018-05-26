#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"netpbm.h"

Image shrinking(Image,Image);
Image expanding(Image,Image);
Image threshold(Image);
Matrix labelling(Image);

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

img1 = createImage(img.height,img.width);


// invert the shrink and expanding



/*img2 = shrinking(img,img1);
img2 = expanding(img,img1);
img2 = expanding(img,img1);
img2 = shrinking(img,img1);
img2 = expanding(img,img1);
img2 = shrinking(img,img1);
img2 = expanding(img,img1);
img2 = shrinking(img,img1);
img2 = expanding(img,img1);
img2 = shrinking(img,img1);*/


/*img2 = shrinking(img,img1);
img2 = shrinking(img,img1);
img2 = expanding(img,img1);
//img2 = shrinking(img,img1);
img2 = expanding(img,img1);
img2 = expanding(img,img1);
img2 = expanding(img,img1);
//img2 = shrinking(img,img1);*/

img2 = shrinking(img,img1);
img2 = expanding(img,img1);
img2 = shrinking(img,img1);
img2 = expanding(img,img1);



writeImage(img2,"test_cleaned.pbm");


/*mx = labelling(img);
for(i=0;i<727;i++){
	for(j=0;j<1261;j++){
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


Image expanding(Image img,Image img1){
int i,j,x,y;
for(i=0;i<img.height;i++){

	for(j=0;j<img.width;j++){
// code for top line in the matrix. 
		if((i-1) < 0 && (j-1) < 0){// top left most
			if( img.map[i][j+1].i || img.map[i+1][j].i){
					img1.map[i][j].i = 255;
				}
				
		} 
		else if((i-1)<0 && j<(img.width -1) && (j-1) >= 0 ) {// top most inbetween 
			if(img.map[i][j-1].i || img.map[i][j+1].i || img.map[i+1][j].i )
				img1.map[i][j].i = 255;
				}
		else if((i-1) < 0 && j == (img.width-1)){// tomost rightmost
			if(img.map[i][j-1].i || img.map[i+1][j].i ){
					img1.map[i][j].i=255;
				}
			}
//code for bottom line matrix.
		else if((j-1)<0 && (i+1) == img.height){//bottom most left most
			if(img.map[i-1][j].i || img.map[i][j+1].i)
				img1.map[i][j].i = 255;
	}
		else if((j==(img.width -1) && (i+1) == img.height)){//bottom most right most
			if(img.map[i-1][j].i || img.map[i][j-1].i)
				img1.map[i][j].i = 255;
	}
		else if(j!=(img.width -1) && (j-1)>=0  && (i+1)== img.height){// bottom most inbetween 
			if(img.map[i-1][j].i || img.map[i][j+1].i || img.map[i][j-1].i)
				img1.map[i][j].i = 255;
	}

// code for sandwich extreems	

		else if((j-1) < 0 && i>=1 && i<(img.height -1)){// leftmost inbetween top and bottom
			if(img.map[i+1][j].i || img.map[i-1][j].i || img.map[i][j+1].i)
				img1.map[i][j].i = 255;
	}
		else if(j==(img.width -1) && i>=1 && i<(img.height -1)){// rightmost inbetween top and bottom
			if(img.map[i-1][j].i || img.map[i][j-1].i || img.map[i+1][j].i)
				img1.map[i][j].i = 255;
	}

		else {
			if(img.map[i-1][j].i || img.map[i][j+1].i || img.map[i][j-1].i || img.map[i+1][j].i)
				img1.map[i][j].i = 255;
	}
		}
	}
return img1;
}

Image shrinking(Image img,Image img1){
int i,j,x,y;
for(i=0;i<img.height;i++){

	for(j=0;j<img.width;j++){
// code for top line in the matrix. 
		if((i-1) < 0 && (j-1) < 0){// top left most
			if( img.map[i][j+1].i == 0 || img.map[i+1][j].i == 0){
					img1.map[i][j].i = 0;
				}
				
		} 
		else if((i-1)<0 && j<(img.width -1) && (j-1) >= 0 ) {// top most inbetween 
			if(img.map[i][j-1].i == 0 || img.map[i][j+1].i == 0 || img.map[i+1][j].i ==0 )
				img1.map[i][j].i = 0;
				}
		else if((i-1) < 0 && j == (img.width-1)){// tomost rightmost
			if(img.map[i][j-1].i == 0 || img.map[i+1][j].i == 0){
					img1.map[i][j].i=0;
				}
			}
//code for bottom line matrix.
		else if((j-1)<0 && (i+1) == img.height){//bottom most left most
			if(img.map[i-1][j].i == 0 || img.map[i][j+1].i == 0)
				img1.map[i][j].i = 0;
	}
		else if((j==(img.width -1) && (i+1) == img.height)){//bottom most right most
			if(img.map[i-1][j].i ==0|| img.map[i][j-1].i == 0)
				img1.map[i][j].i = 0;
	}
		else if(j!=(img.width -1) && (j-1)>=0  && (i+1)== img.height){// bottom most inbetween 
			if(img.map[i-1][j].i==0 || img.map[i][j+1].i == 0|| img.map[i][j-1].i == 0)
				img1.map[i][j].i = 0;
	}

// code for sandwich extreems	

		else if((j-1) < 0 && i>=1 && i<(img.height -1)){// leftmost inbetween top and bottom
			if(img.map[i+1][j].i==0 || img.map[i-1][j].i==0 || img.map[i][j+1].i==0)
				img1.map[i][j].i = 0;
	}
		else if(j==(img.width -1) && i>=1 && i<(img.height -1)){// rightmost inbetween top and bottom
			if(img.map[i-1][j].i==0 || img.map[i][j-1].i ==0|| img.map[i+1][j].i==0)
				img1.map[i][j].i = 0;
	}

		else {
			if(img.map[i-1][j].i ==0 || img.map[i][j+1].i==0 || img.map[i][j-1].i==0 || img.map[i+1][j].i==0)
				img1.map[i][j].i = 0;
	}
		}
	}
return img1;
}


Matrix labelling(Image img){
int i,j,count=0;
Matrix labelArr;
labelArr = createMatrix(img.height,img.width);
for(i=0;i<727;i++){
	for(j=0;j<1261;j++){
		if(img.map[i][j].i<128){//found a component
			if((labelArr.map[i-1][j] == labelArr.map[i][j-1]) && labelArr.map[i-1][j]!= 0.0 && labelArr.map[i][j-1]!=0.0)
				labelArr.map[i][j] = labelArr.map[i-1][j];
			else if(labelArr.map[i-1][j] != 0.0 && labelArr.map[i][j-1]!=0.0 && labelArr.map[i-1][j]!= labelArr.map[i][j-1] )
				 labelArr.map[i][j] = labelArr.map[i-1][j];	
			else if(labelArr.map[i-1][j] != 0.0 && labelArr.map[i][j-1] == 0.0 )
				labelArr.map[i][j] = labelArr.map[i-1][j];
			else if(labelArr.map[i][j-1]!=0.0 && labelArr.map[i-1][j] == 0.0)
				labelArr.map[i][j] = labelArr.map[i][j-1];
			else
				labelArr.map[i][j]= ++count;
			
			}
		 

		}
	
}
return labelArr;	
}
