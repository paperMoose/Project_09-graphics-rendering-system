/*
  Bruce A. Maxwell
  Fall 2013

  Creates a Mandelbrot set at a selected location
*/
#include "ppmMethods.h"
#include "Image.h"
#include "fract.h"

int main(int argc, char *argv[]) {
  Image *src1;
  Image* src2;
  Image* dst;

  src1 = image_create(750, 1000);
  src2 = image_create(750, 1000);
  dst = image_create(750, 1000);

  //mandelbrot( src1, -2.0, 1.0, 0.0025);
  julia( src1, -2, -1, 3 );
  //makeNoise(src2);
  //image_blend(src1, src2, dst, 0.9);

  image_write( src1, "../images/fractal.ppm");
  //image_write( src2, "../images/noise.ppm");
  //image_write( dst, "../images/noise_blend.ppm");
  //image_fwrite( dst, "../images/noise_blend.jpg");


  image_free( src1 );
  image_free( src2 );
  image_free( dst );
  return(0);
}
