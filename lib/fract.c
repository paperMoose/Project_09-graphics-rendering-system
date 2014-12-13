/*
Erfan Azad
Date: Wed 17 Sep 2014
File: fract.c
This file contains two functions for making fractiles.
*/

#include <stdio.h>
#include <math.h>
#include "fract.h"
#include "Image.h"

/*
This function creates and plots a mandelbrot set with coloring.
x0 and y0 are the coordinates of the upper left pixel of the image.
dx is the distance between the pixels in cartesian coordinates.
Returns a pointer to the Image structure containing the final image.
*/
void mandelbrot( Image *dst, float x0, float y0, float dx){
	if(NULL != dst){
		float MinRe = x0;
		float MaxIm = y0;
		float MaxRe = MinRe + (float)((dst->cols - 1) * dx);
		float MinIm = MaxIm - (float)((dst->rows - 1) * dx);
		printf("MaxRe, MinIm: %f, %f\n",MaxRe, MinIm);
		int maxIterations = 100;

		float Re_factor = (float)((MaxRe - MinRe) / (dst->cols -1));
		float Im_factor = (float)((MaxIm - MinIm) / (dst->rows -1));

		int x;
		int y;
		int p=0;
		for(y=0; y< dst->rows; y++){
			float c_im = MaxIm - (float)(y)*Im_factor;
			for(x=0; x< dst->cols; x++){
				float c_re = MinRe + (float)(x)*Re_factor;
				
				float z_re = c_re;
				float z_im = c_im;
				int is_inside = 1;
				int n;
				for(n=0; n< maxIterations; n++){
					float z_re2 = z_re * z_re;
					float z_im2 = z_im * z_im;
					if((z_re2 + z_im2) > 4.0){
						is_inside = 0;
						p = n;
						break;
					}
					z_im = 2.0 * z_re * z_im + c_im;
					z_re = z_re2 - z_im2 + c_re;
				}
				if(is_inside==1){ // paint black
					image_setc(dst,y, x, 0, 0.0);
					image_setc(dst,y, x, 1, 0.0);
					image_setc(dst,y, x, 2, 0.0);
				}else{ // paint reddish
					float color = (float)((float)(p) / (float)(maxIterations));
					if (color <= 0.5){//red to black
						image_setc(dst,y, x, 0, color);
						image_setc(dst,y, x, 1, 0.0);
						image_setc(dst,y, x, 2, 0.0);
					}else{ // red to white
						image_setc(dst,y, x, 0, color);
						image_setc(dst,y, x, 1, 1.0);
						image_setc(dst,y, x, 2, 1.0);	
					}
				}
			}
		} 
	}else{
		printf("ERROR: Mandelbrot >> dst is NULL.\n");
	}
}

/*
This function creates and plots a julia set with coloring.
x0 and y0 are the coordinates of the lower left pixel of the image.
dx is the distance between the pixels in cartesian coordinates.
Returns a pointer to the Image structure containing the final image.
*/
void julia(Image *dst, float x0, float y0, float dx){
	if(NULL != dst){
		float MinRe = x0;
		float MaxIm = y0 + (dst->rows*dx)/dst->cols;
		float MaxRe = x0 + dx;
		float MinIm = y0;
		printf("MaxRe, MinIm: %f, %f\n",MaxRe, MinIm);
		int maxIterations = 500;

		float step = dx / dst->cols;

		float c_re = 0.7454054;
		float c_im = 0.1130063;
		int p = 0;
		int x;
		int y;
		for(y=0; y< dst->rows; y++){
			for(x=0; x< dst->cols; x++){
				
				float z_re = x*step + MinRe;
				float z_im = MaxIm - y*step;
				int is_inside = 1;
				int n;
				for(n=0; n< maxIterations; n++){
					float z_re2 = z_re * z_re;
					float z_im2 = z_im * z_im;
					if((z_re2 + z_im2) > 4.0){
						is_inside = 0;
						p=n;
						//printf("%d\n",p);
						break;
					}
					z_im = 2.0 * z_re * z_im - c_im;
					z_re = z_re2 - z_im2 - c_re;
				}
				if(is_inside==1){ // paint blue
					//printf("inside\n");
					image_setc(dst,y, x, 0, 0.4);
					image_setc(dst,y, x, 1, 0.4);
					image_setc(dst,y, x, 2, 0.2);
				}else{ // paint reddish
					//printf("~~inside\n");
					float color = (float)((float)(p) / (float)(maxIterations));
					if(color < 0.7){//black o red
						image_setc(dst,y, x, 0, color);
						image_setc(dst,y, x, 1, 0.0);
						image_setc(dst,y, x, 2, 0.0);
					}else{ //red to white
						image_setc(dst,y, x, 0, color);
						image_setc(dst,y, x, 1, 1.0);
						image_setc(dst,y, x, 2, 1.0);
					}	
				}
			}
		} 
	}else{
		printf("ERROR: Julia >> dst is NULL.\n");
	}
}

/*
This function gets an Image pointer.
Returns a noise picture to be blended with the fractile picture.
*/
Image* makeNoise(Image* img){
	int i;
	int j;
	for (i=0; i< img->rows; i++){
		for (j=0; j < img->cols; j++){
			float color = (float)((float)sin(j));
			image_setc(img,i, j, 0, color);
			image_setc(img,i, j, 1, 0.0);
			image_setc(img,i, j, 2, 0.0);
		}
	}
	return img;
}


