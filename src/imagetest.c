/*
  Bruce A. Maxwell
  fall 2013

  Test of the image library functions
*/
#include <math.h>
#include "Image.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  Image *src;
  FPixel p = { {0.4, 0.6, 0.8}, 0.0, 1.0};
  int rows = 300;
  int cols = 400;
  int i, j;

  // initialize an image
  src = image_create(rows, cols);


  // fill it with colors
  for(i=0;i<rows;i++) {
    for(j=0;j<cols;j++) {
      image_setc( src, i, j, 0, fabs( ((float)(i-rows/2)) / (rows/2) ) );
      image_setc( src, i, j, 1, fabs( ((float)(j-cols/2)) / (cols/2) ) );
      image_setc( src, i, j, 2, ((float)(i+j)) / (rows + cols) );
    }
  }
  printf("Writing color.ppm\n");
  // write it out
  image_write( src, "../images/colors.ppm");

  // free it
  image_free( src );

  src = image_create(0, 0);
  rows = 400;
  cols = 600;

  image_alloc( src, rows, cols );

  image_reset( src );

  printf("Writing black.ppm\n");
  image_write( src, "../images/black.ppm");

  for(i=0;i<rows;i++) {
    for(j=0;j<cols;j++) {
      image_setf( src, i, j, p );
    }
  }

  printf("Writing skyblue.ppm\n");
  image_write( src, "../images/skyblue.ppm" );
  
  image_dealloc(src);

  image_free( src );

  return(0);
}
