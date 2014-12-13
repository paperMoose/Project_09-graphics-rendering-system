/*
  Assignment 4, required image 1

  C version
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "list.h"
#include "polygon.h"
#include "Image.h"
#include "drawstate.h"

/*
  Program to test polygon functionality
*/
int main(int argc, char *argv[]) {
  Image *src;
  const int rows = 300;
  const int cols = 400;
  Polygon *p;
  DrawState* ds;
  Color Red;
  Color Orange;
  Color White;
  Color Blue;
  Point pt[100];
  int i;
  

  ds = drawstate_create();
  ds->shade = ShadeConstant;
  srand(42);

  Color_set(&Red, 0.9, 0.2, 0.1 );
  Color_set(&Orange, 0.95, 0.7, 0.3 );
  Color_set(&White, 1.0, 1.0, 1.0 );
  Color_set(&Blue, 0.2, 0.1, 0.95 );

  src = image_create(rows, cols);

  // make a simple square to test proper areas and locations
  // the square ought to be 20x20, include pixel (30,30) and exclude pixel (50, 50)
  point_set2D(&(pt[0]), 30, 30);
  point_set2D(&(pt[1]), 50, 30);
  point_set2D(&(pt[2]), 50, 50);
  point_set2D(&(pt[3]), 30, 50);

  p = polygon_createp(4, pt);

  printf("drawing a square\n");
  polygon_drawFill(p, ds,src);

  // something more interesting
  for(i=0;i<50;i++) {
    float dr = rand() % 20;
    point_set2D(&(pt[i]), 
    200 + cos((float)i * M_PI * 2.0 / 50.0)*(70 + dr),
    150 + sin((float)i * M_PI * 2.0 / 50.0)*(70 + dr));
  }
  polygon_set(p, 50, pt);

  printf("drawing first big polygon\n");
  polygon_drawFill(p, ds, src);

  for(i=0;i<50;i++) {
    float dr = rand() % 15;
    point_set2D(&(pt[i]), 
    200 + cos((float)i * M_PI * 2.0 / 50.0)*(50 + dr),
    150 + sin((float)i * M_PI * 2.0 / 50.0)*(50 + dr));
  }
  polygon_set(p, 50, pt);

  printf("drawing second big polygon\n");
   polygon_drawFill(p, ds,src);

  for(i=0;i<50;i++) {
    float dr = rand() % 10;
    point_set2D(&(pt[i]), 
    200 + cos((float)i * M_PI * 2.0 / 50.0)*(30 + dr),
    150 + sin((float)i * M_PI * 2.0 / 50.0)*(30 + dr));
  }
  polygon_set(p, 50, pt);

  printf("drawing third big polygon\n");
   polygon_drawFill(p, ds,src);

  printf("writing output\n");
  image_write(src, "test4a.ppm");

  image_free(src);

  return(0);
}
