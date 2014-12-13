/*
  Assignment 4, required image 2

  C version
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "polygon.h"
#include "line.h"
#include "point.h"
#include "Image.h"
#include "color.h"

/*
  Program to test polygon functionality using barycentric coordinates
*/
int main(int argc, char *argv[]) {
  Image *src;
  const int rows = 1000;
  const int cols = 600;
  Polygon *p;
  Color Red;
  Color Green;
  Color Blue;
  Point pt[100];

  Color_set(&Red, 0.9, 0.2, 0.1 );
  Color_set(&Green, 0.1, 0.9, 0.1 );
  Color_set(&Blue, 0.2, 0.1, 0.95 );
  
  src = image_create(rows, cols);

  // make a simple square to test proper areas and locations
  // the square ought to be 20x20, include pixel (30,30) and exclude pixel (50, 50)
//  point_set2D(&(pt[0]), 300, 100);
//  point_set2D(&(pt[1]), 700, 100);
//  point_set2D(&(pt[2]), 900, 500);
//  point_set2D(&(pt[3]), 700, 900);
//  point_set2D(&(pt[4]), 300, 900);
//  point_set2D(&(pt[5]), 100, 500);
//  point_set2D(&(pt[6]), 500, 500);
    

  point_set2D(&(pt[0]), 300, 100);
  point_set2D(&(pt[1]), 100, 501);
  point_set2D(&(pt[2]), 500, 501);
    
  p = polygon_createp(3, pt);


  printf("drawing polygon 2\n");
  polygon_drawFillB_Gradient(p, src, Blue, Green, Red);
    
    
    point_set2D(&(pt[0]), 300, 900);
    point_set2D(&(pt[1]), 100, 500);
    point_set2D(&(pt[2]), 500, 500);

    polygon_set(p, 3, &(pt[0]) );

  printf("drawing polygon 4\n");
  polygon_drawFillB_Gradient(p, src, Blue, Green, Red);
    


  printf("writing output\n");
  image_write(src, "hexagon.ppm");

  image_free(src);

  return(0);
}