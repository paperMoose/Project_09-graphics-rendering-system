/*
  This is an example of reading and writing an image using the ppmIO.c
  routines.  To manipulate the image, change the pixel color values.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ppmIO.h"
#include "ppmMethods.h"

#define USECPP 0

int main(int argc, char *argv[]) {
  Pixel *image1;
  Pixel *image2;
  Pixel *image3;
  int rows, cols, colors;
  long imagesize;
  // //int intensity, avg_r, avg_g, avg_b;


  if(argc < 5) {
    printf("Usage: %s <input file_1><input file_2><input mask><output file>\n", argv[0]);
    exit(-1);
  }

  /* read in the image */
  image1 = readPPM(&rows, &cols, &colors, argv[1]);
  if(!image1) {
    fprintf(stderr, "Unable to read file_1 %s\n", argv[1]);
    exit(-1);
  }else{
    printf("successfully opened image %s\n", argv[1]);
  }

  image2 = readPPM(&rows, &cols, &colors, argv[2]);
  if(!image2) {
    fprintf(stderr, "Unable to read file_2 %s\n", argv[2]);
    exit(-1);
  }else{
    printf("successfully opened image %s\n", argv[2]);
  }

  image3 = readPPM(&rows, &cols, &colors, argv[3]);
  if(!image3) {
    fprintf(stderr, "Unable to read mask %s\n", argv[3]);
    exit(-1);
  }else{
    printf("successfully opened image %s\n", argv[3]);
  }

  /* calculate the image size */
  imagesize = (long)rows * (long)cols;

  //image1 = makeMask(image1, imagesize); // use for creating the mask
  //image1 = setBackground(image1, image2, image3, imagesize); // setting the back ground ***use either this method or "translate" method. NOT BOTH
  image1 = translate(image1, image2, image3, imagesize, cols, rows,0,0); // sets the back ground as well as translates the front image.
  //image1 = toGreyscale(image1, imagesize); // turns an image to greyscale
  image1 = toNegative(image1, imagesize); // turns an image to negative
  //image1 = horizontalBlur(image1, imagesize); // turns the  image blury



//   /* write out the resulting image */
  writePPM(image1, rows, cols, colors /* s/b 255 */, argv[4]);

//   /* free the image memory */
#if USECPP
  delete[] image;
#else
  free(image1);
  free(image2);
  free(image3);
#endif

  return(0);
}

