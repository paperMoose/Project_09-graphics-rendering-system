/* 
Erfan Azad
Date: Mon 15 Sep 2014
This file contains the definitions of all the method declarations in
image.c which deals with Constructors & Destructors, I/O functions, Access, Utility
*/
#include "Image.h"
#include "color.h"
#include "ppmIO.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* constructors & destructors */

/*
Allocates an Image structure and initializesthe top level fields to appropriate values.
Allocates space for an image of the specified size, unless
either rows or cols is 0.
Returns a pointer to the allocated Image structure. Returns a NULL pointer if
the operation fails.
*/
Image* image_create(int rows, int cols){
	if((rows == 0) || (cols == 0)){
		printf("WARNING: image_create >> rows or cols is Zero\n");
	}
	Image* image = malloc(sizeof(Image));
	if(NULL == image){return NULL;}
	image_init(image);
	image_alloc(image, rows, cols);
	return image;
}

/*
De-allocates image data and frees the Image structure
*/
void image_free(Image* src){
	if(NULL != src){
		if(NULL != src->data){
			int i;
			for (i=0; i<src->rows; i++){ 
				free(src->data[i]); // free all the data cols
			}
			free(src->data); // free the data rows
			src->data = NULL;
			free(src);
			src = NULL;
		}else{
			free(src);
			src = NULL;
		}
	}else{
		printf("WARNING: image_free >> src is NULL\n");
	}
}

/*
Given an uninitialized Image structure, sets the rows and cols
fields to zero and the data field to NULL.
*/
void image_init(Image* src){
	if(NULL != src){
		src->data = NULL;
		src->rows = 0;
		src->cols = 0;
	}else{
		printf("ERROR: cannot initialize image >> image pointer is NULL\n");
	}
}

/*
Allocates space for the image data given rows and columns and initializes the image data to appropriate values,
such as 0.0 for RGBA and 1.0 for Z.
Returns 0 if the operation is successful. Returns a non-zero value if the operation fails.
This function does free existing memory if rows and cols are both non-zero.
*/
int image_alloc(Image* src, int rows, int cols){
	if(NULL != src){
		if(rows==0 || cols==0){
			printf("WARNING: image_alloc >> rows or cols is Zero\n");
		}else{ // if rows and cols are non-zero, deallocate the src first
			image_dealloc(src);
		}

		src->data = malloc(sizeof(FPixel*)*rows); // creating "rows". an array of FPixel pointers
		if(NULL == src->data){return -1;}
		int k;
		for(k=0; k<rows; k++){
			src->data[k] = malloc(sizeof(FPixel)*cols); // creating "cols". arrays of FPixels.
			if(NULL == src->data[k]){return -1;}
		}
		src->rows = rows;
		src->cols = cols;

		image_reset(src); // resets all FPixels to black, a=1.0, z=1.0.
		image_filla(src, 0.0); // sets the alpha value of each FPixel to 0.0
		return 0;
	}else{
		printf("ERROR: image_alloc >> image pointer in NULL\n");
		return -1;
	}
}

/*
De-allocates image data and resets the Image structure
fields. The function does not free the Image structure.
*/
void image_dealloc(Image* src){
	if(NULL != src){
		if(NULL != src->data){
			int i;
			for (i=0; i<src->rows; i++){ 
				free(src->data[i]); // free all the data cols
			}
			free(src->data); // free the data rows
			src->data = NULL;
			src->rows = 0;
			src->cols = 0;
		}
	}else{
		printf("WARNING: image_dealloc >> image pointer is NULL\n");
	}
}

/*I/O functions */
	
/*
Reads a PPM image from the given filename. 
Initializes the alpha channel to 1.0 and the z channel to 1.0. 
Returns a NULL pointer if the operation fails.
*** IT will free the memory of readPPM() after copying the data into the new Image structure
*/
Image* image_read(char* filename){
	if(strlen(filename)==0){
		printf("ERROR: could not read image >> filename is empty\n");
		return NULL;
	}else{
		int rows;
		int cols;
		int color;
		Pixel* pixel = readPPM(&rows, &cols, &color, filename); //reading the .ppm file

		Image* image = image_create(rows, cols); // creating an image using our image structure.
		image_alloc(image, rows, cols); // allocate memory for the FPixels and initialize them
		int i;
		int j;
		int p;
		for(i=0, p=0; i<rows; i++){ // transfering the rgb values from ppm file Pixel array to the new FPixels of Image structure
			for(j=0; j<cols; j++, p++){
				image->data[i][j].rgb[0] = (float)(pixel[p].r/255.0);
				image->data[i][j].rgb[1] = (float)(pixel[p].g/255.0);
				image->data[i][j].rgb[2] = (float)(pixel[p].b/255.0);
				image->data[i][j].a = 1.0;
				image->data[i][j].z = 1.0;
			}
		}
		// free the ppm array
		free(pixel);
		pixel = NULL;

		//final checkpoint and return
		if (NULL != image){
			return image;
		}else{
			return NULL;
		}
	}
}

/*
Writes a PPm image to the given filename.
Returns 0 on success.
This function DOES NOT free the memory of the Image src.
*/
int image_write(Image* src, char* filename){
	if((NULL != src)&&(NULL != src->data)){
		int imagesize = src->rows * src->cols; // number of pixels (image size)
		Pixel* image = malloc(sizeof(Pixel)*imagesize);//allocate memory for the PPM array that will be written later to a file
		if(NULL == image){return -1;} // checking the malloc
		
		int i;
		int j;
		int k;
		for(i=0, k=0; i<src->rows; i++){
			for(j=0; j<src->cols; j++, k++){
				image[k].r = (unsigned char)(src->data[i][j].rgb[0] * 255);
				image[k].g = (unsigned char)(src->data[i][j].rgb[1] * 255);
				image[k].b = (unsigned char)(src->data[i][j].rgb[2] * 255);
			}
		}
		writePPM(image, src->rows, src->cols, 255, filename);
		free(image);	
		return 0;
	}else{
		return -1;
	}
}

/*
Writes a PPm image to the given filename.
The format should be included in the filename. e.g. "../images/test.jpg"
Returns 0 on success.
This function DOES NOT free the memory of the Image src.
*/
int image_fwrite(Image* src, char* filename){
	if((NULL != src)&&(NULL != src->data)){
		int imagesize = src->rows * src->cols; // number of pixels (image size)
		Pixel* image = malloc(sizeof(Pixel)*imagesize);//allocate memory for the PPM array that will be written later to a file
		if(NULL == image){return -1;} // checking the malloc
		
		int i;
		int j;
		int k;
		for(i=0, k=0; i<src->rows; i++){
			for(j=0; j<src->cols; j++, k++){
				image[k].r = (unsigned char)(src->data[i][j].rgb[0] * 255);
				image[k].g = (unsigned char)(src->data[i][j].rgb[1] * 255);
				image[k].b = (unsigned char)(src->data[i][j].rgb[2] * 255);
			}
		}
		// write the file using writePPM and then convert it using system call
		writePPM(image, src->rows, src->cols, 255, filename);
		free(image);	
		if(NULL != filename){
			char command[200] = "convert ";
			strcat(command, filename);
			strcat(command, " ");
			strcat(command, filename);
			int s = system(command);
			if(0 != s){
				printf("ERROR: image_fwrite >> format conversion failed");
				return -1;
			}
		}
		return 0;
	}else{
		return -1;
	}
}

/* Acces functions */

/*
Returns the FPixel at (r, c)
*/
FPixel image_getf(Image* src, int r, int c){
	return src->data[r][c];
}

/*
Returns the value of band b at
pixel (r, c).
*/
float image_getc(Image* src, int r, int c, int b){
	if((NULL != src) && (NULL != src->data)){
		switch (b){
			case 0:
				return src->data[r][c].rgb[0];
			case 1: 
				return src->data[r][c].rgb[1];
			case 2:
				return src->data[r][c].rgb[2]; 
			default:
				printf("ERROR: invalid band request: b = %d.\n", b);
				return -1.0;
		}
	}else{
		printf("ERROR: Image src or src->data is NULL");
		return -1.0;
	}
}

/*
Returns the alpha value at pixel (r, c)
*/
float image_geta(Image* src, int r, int c){
	return src->data[r][c].a;
}

/*
Returns the depth value at pixel (r, c)
*/
float image_getz(Image* src, int r, int c){
	return src->data[r][c].z;
}

/*
Sets the values of pixel (r,c) to the FPixel val
*/
void image_setf(Image* src, int r, int c, FPixel val){
	if((NULL != src)&&(NULL != src->data)){
		src->data[r][c].rgb[0] = val.rgb[0];
		src->data[r][c].rgb[1] = val.rgb[1];
		src->data[r][c].rgb[2] = val.rgb[2];
		src->data[r][c].a = val.a;
		src->data[r][c].z = val.z;
	}
}

/*
Sets the value of pixel (r, c) band b to val.
*/
void image_setc(Image* src, int r, int c, int b, float val){
	if((NULL != src) || (NULL != src->data)){
		if (r<0 || r>src->rows-1 || c<0 || c>src->cols-1)
		{
			return;
		}
		switch(b){
			case 0:
				src->data[r][c].rgb[0] = val;
				break;
			case 1:
				src->data[r][c].rgb[1] = val;
				break;
			case 2:
				src->data[r][c].rgb[2] = val;
				break;
		}
	}else{
		printf("ERROR: can't set the color >> src or src->data is NULL\n");
		exit(-1);
	}
}

/*
sets the RGB value of pixel (r,c) to the RGB value of Color c.
*/
void image_setColor(Image* src, int r, int c, Color clr){
	if((NULL == src)&&(NULL == src->data)){
		printf("ERROR: image_setColor >> src or src->data is NULL");
		exit(-1);
	}
	if (r<0 || r>src->rows-1 || c<0 || c>src->cols-1)
	{
		return;
	}
	src->data[r][c].rgb[0] = clr.rgb[0];
	src->data[r][c].rgb[1] = clr.rgb[1];
	src->data[r][c].rgb[2] = clr.rgb[2];
}

/*
Sets the alpha value of pixel (r, c) to val.
*/
void image_seta(Image* src, int r, int c, float val){
	if((NULL != src)&&(NULL != src->data)){
		src->data[r][c].a = val;
	}
}

/*
Sets the depth value of pixel (r, c) to val.
*/
void image_setz(Image* src, int r, int c, float val){
	if((NULL != src)&&(NULL != src->data)){
		if (r<0 || r>src->rows-1 || c<0 || c>src->cols-1)
		{
			return;
		}
		src->data[r][c].z = val;
	}
}

/* Utility functions */

/*
Resets every pixel to a default value (e.g. Black, alpha value
of 1.0, z value of 1.0)
*/
void image_reset(Image* src){
	if ((NULL != src)&&(NULL != src->data)){
		int i;
		int j;
		for(i=0; i<src->rows; i++){
			for(j=0; j<src->cols; j++){
				src->data[i][j].rgb[0] = 0.0;
				src->data[i][j].rgb[1] = 0.0;
				src->data[i][j].rgb[2] = 0.0;
				src->data[i][j].a = 1.0;
				src->data[i][j].z = 1.0;
				src->data[i][j].list = NULL;
			}
		}
	}
}

/*
Sets every FPixel to the given value
*/
void image_fill(Image* src, FPixel val){
	if((NULL != src)&&(NULL != src->data)){
		int i;
		int j;
		for(i=0; i<src->rows; i++){
			for(j=0; j<src->cols; j++){
				src->data[i][j].rgb[0] = val.rgb[0];
				src->data[i][j].rgb[1] = val.rgb[1];
				src->data[i][j].rgb[2] = val.rgb[2];
				src->data[i][j].a = val.a;
				src->data[i][j].z = val.z;
			}
		}
	}
}

/*
Sets the (r,g,b) value of each pixel to the given color.
*/
void image_fillrgb(Image* src, float r, float g, float b){
	if((NULL != src)&&(NULL != src->data)){
		int i;
		int j;
		for(i=0; i<src->rows; i++){
			for(j=0; j<src->cols; j++){
				src->data[i][j].rgb[0] = r;
				src->data[i][j].rgb[1] = g;
				src->data[i][j].rgb[2] = b;
			}
		}
	}
}

/*
Sets the alpha value of each pixel to the given value.
*/
void image_filla(Image* src, float a){
	if ((NULL != src)&&(NULL != src->data)){
		int i;
		int j;
		for(i=0; i<src->rows; i++){
			for(j=0; j<src->cols; j++){
				src->data[i][j].a = a;
			}
		}
	}
}

/*
Sets the z value of each pixel to the given value.
*/
void image_fillz(Image *src, float z){
	if((NULL != src)&&(NULL != src->data)){
		int i;
		int j;
		for(i=0; i<src->rows; i++){
			for(j=0; j<src->cols; j++){
				src->data[i][j].z = z;
			}
		}
	}
}

/* Blending */

/*
This function takes three Image pointers and blends src1 and src2 together according to the 
alpha value of 0 to 1 and writed it into Image* dst.
*** ALL THREE IMAGE* HAVE TO BE OF THE SAME SIZE **
Returns dst, Image pointer.
*/
Image* image_blend(Image* src1, Image* src2, Image* dst, float alpha){
	if((NULL != src1) || (NULL != src2)|| (NULL != dst) ){
		if((src1->rows == dst->rows) && (src1->cols == dst->cols) && (src2->rows == dst->rows) && (src2->cols == dst->cols)){
			if((alpha > 1.0) || (alpha < 0.0)){
				printf("ERROR: image_blend >> alpha value out of range!\n");
				return NULL;
			}
			float alpha_r;
			float alpha_g;
			float alpha_b;

			int i;
			int j;
			for (i=0; i< src1->rows; i++){
				for (j=0; j< src1->cols; j++){
					alpha_r = (alpha * src1->data[i][j].rgb[0]) + ((1.0-alpha) * src2->data[i][j].rgb[0]); 
					alpha_g = (alpha * src1->data[i][j].rgb[1]) + ((1.0-alpha) * src2->data[i][j].rgb[1]);
					alpha_b = (alpha * src1->data[i][j].rgb[2]) + ((1.0-alpha) * src2->data[i][j].rgb[2]);

					image_setc(dst, i, j, 0, alpha_r);
					image_setc(dst, i, j, 1, alpha_g);
					image_setc(dst, i, j, 2, alpha_b);
				}
			}
			return dst;
		}else{
			printf("ERROR: image_blend >> Image pointers have different sizes.\n");
			return NULL;
		}
	}else{
		printf("ERROR: image_blend >> One or more Image pointers are NULL.\n");
			return NULL;
	}
}














