/* Erfan Azad
this file includes extra methods to be used in the main 
program as needed
*/
#include "ppmIO.h"
#include "ppmMethods.h"
#include <stdio.h>
#include <stdlib.h>

/*
Creates a mask of the picture with GREEN screen.
Returns a pointer to the masked image.
*/
Pixel* makeMask(Pixel* image, int imageSize){
	int i;
	for (i =0; i<imageSize; i++){
		//creates a mask of image. puts 255 on green screen and 0 elsewhere.
    	if (((int)image[i].g > 2*(int)image[i].r) && ((int)image[i].g > (int)image[i].b)){
      		image[i].r = 255;
      		image[i].g = 255;
      		image[i].b = 255;
    	}
    	else{
      		image[i].r = 0;
      		image[i].g = 0;
      		image[i].b = 0;
    	}
	}
	return image;
}

/* This mathod sets the background image of the input image using the mask created by "makeMask" 
method.
Returns a pointer to the final image
*/
Pixel* setBackground(Pixel* background, Pixel* image, Pixel* imageMask, int imageSize){
	int i;
	for(i=0;i<imageSize;i++) {
		float alpha_r = imageMask[i].r/255.0;
	    float alpha_g = imageMask[i].g/255.0; 
		float alpha_b = imageMask[i].b/255.0;

	    background[i].r = (alpha_r * background[i].r) + ((1-alpha_r) * image[i].r);
	    background[i].g = (alpha_g * background[i].g) + ((1-alpha_g) * image[i].g);
	   	background[i].b = (alpha_b * background[i].b) + ((1-alpha_b) * image[i].b);
  }
	return background;
}

/* translate the foground image using the mask.
Returns a pointer to the translated image
*/
Pixel* translate(Pixel* background, Pixel* image, Pixel* imageMask, int imageSize, int cols, int rows, int dx, int dy){
  int i;
  for(i=0;i<imageSize;i++) {
  	int translation = i+dx+dy*cols;
  	int transIndex_X = i+dx;
  	int transIndex_Y = i+dy*cols;
  	int lowBound = i - (i % cols);
  	int highBound = i + ((cols-1) - (i % cols));
  	if ((lowBound > transIndex_X) || (transIndex_X > highBound) || (transIndex_Y < 0) || (transIndex_Y > imageSize)){ //if destination index in out of the bounds
  		continue;
  	}else{
  		float alpha = imageMask[translation].g/255.0; /*r,g,b of the mask are the same*/

	    background[i].r = (alpha * background[i].r) + ((1-alpha) * image[translation].r);
	    background[i].g = (alpha * background[i].g) + ((1-alpha) * image[translation].g);
	   	background[i].b = (alpha * background[i].b) + ((1-alpha) * image[translation].b);
  	}
  }
  return background;
}

/*This method turns the given image into its negative
Returns a pointer to the final image
*/
Pixel* toNegative(Pixel* image, int imageSize){
	int i;
	for (i=0; i<imageSize; i++){
	    int intensity = ((int)image[i].r + (int)image[i].g + (int)image[i].b) / 3;
	    image[i].r = 255 - intensity;
	    image[i].g = 255 - intensity;
	    image[i].b = 255 - intensity;
	}
	return image;
}

/*
This method turns the image into greyscale.
Returns a pointer to the greyscale image
*/
Pixel* toGreyscale(Pixel* image, int imageSize){
	int i;
	for (i=0; i<imageSize; i++){
		int intensity = ((int)image[i].r + (int)image[i].g + (int)image[i].b) / 3;
	    image[i].r = intensity;
	    image[i].g = intensity;
	    image[i].b = intensity;
	}
	return image;
}

/*
This method adds a horizontal blur to the image.
Returns a pointer to the image.
*/
Pixel* horizontalBlur(Pixel* image, int imageSize){
	int i;
	int avg_r; 
	int avg_g;
	int avg_b;
	for (i=0; i<imageSize; i++){
		// this piece of code creates a horizontal blur by averaging 5 adjacent pixels
		if (i < 5) {
	      avg_r = ((int)image[0].r + (int)image[1].r + (int)image[2].r + (int)image[3].r + (int)image[4].r)/5;
	      avg_g = ((int)image[0].g + (int)image[1].g + (int)image[2].g + (int)image[3].g + (int)image[4].g)/5;
	      avg_b = ((int)image[0].b + (int)image[1].b + (int)image[2].b + (int)image[3].b + (int)image[4].b)/5;

	      image[i].r = avg_r;
	      image[i].g = avg_g;
	      image[i].b = avg_b;
    	}
    	else{
	      avg_r = ((int)image[i-4].r + (int)image[i-3].r + (int)image[i-2].r + (int)image[i-1].r + (int)image[i].r)/5;
	      avg_g = ((int)image[i-4].g + (int)image[i-3].g + (int)image[i-2].g + (int)image[i-1].g + (int)image[i].g)/5;
	      avg_b = ((int)image[i-4].b + (int)image[i-3].b + (int)image[i-2].b + (int)image[i-1].b + (int)image[i].b)/5;

	      image[i].r = avg_r;
	      image[i].g = avg_g;
	      image[i].b = avg_b;
	    }
	}
	return image;
}



