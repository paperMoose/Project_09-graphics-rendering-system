/*
Erfan Azad
Date: 17 Oct 2014
File: proj5_ext1.c
This file creates a pyramid and creates a gif of it rotating
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"
#include "point.h"
#include "polyline.h"
#include "polygon.h"
#include "line.h"
#include "view.h"

int main(int argc, char *argv[]) {
  const int rows = 1000;
  const int cols = 1000;
  View3D view;
  Matrix vtm;
  Polygon side[4];
  Polygon tpoly;
  Point  tv[3];
  Point  v[4];
  Color  color[4];
  Image *src;
  char filename[100];
  double x;
  int i;

  // set some colors
  Color_set( &color[0], 0, 0, 1 );
  Color_set( &color[1], 0, 1, 0 );
  Color_set( &color[2], 1, 0, 0 );
  Color_set( &color[3], 1, 0, 1 );


  // initialize polygons
  for(i=0;i<4;i++) {
    polygon_init( &side[i] );
  }

  // corners of a cube, centered at (0, 0, 0)
  point_set3D( &v[0], 0, 1, 0 );
  point_set3D( &v[1], 1, -1, 0);
  point_set3D( &v[2], -1, -1, 0 );
  point_set3D( &v[3], 0,  0, 1 );

  //base of the pyramid
  polygon_set(&side[3], 3, &(v[0]));

  //side1
  point_copy(&tv[0], &v[0]);
  point_copy(&tv[1], &v[2]);
  point_copy(&tv[2], &v[3]);

  polygon_set(&side[0], 3, tv);

  //side2
  point_copy(&tv[0], &v[0]);
  point_copy(&tv[1], &v[1]);
  point_copy(&tv[2], &v[3]);

  polygon_set(&side[1], 3, tv);

  //side3
  point_copy(&tv[0], &v[1]);
  point_copy(&tv[1], &v[2]);
  point_copy(&tv[2], &v[3]);

  polygon_set(&side[2], 3, tv);


  point_set3D( &(view.vrp), 1, 0, 0);
  vector_set( &(view.vpn), -view.vrp.val[0], -view.vrp.val[1], -view.vrp.val[2] );

  vector_set( &(view.vup), 0, 0, 1 );
  view.d = 1;  // focal length
  view.du = 2;
  view.dv = view.du * (float)rows / cols;
  view.f = 0; // front clip plane
  view.b = 4; // back clip plane
  view.screenx = cols;
  view.screeny = rows;

  matrix_setView3D( &vtm, &view );

  // use a temprary polygon to transform stuff
  polygon_init( &tpoly );
  printf("Drawing pyramid\n");

x = -2;
int j;
for (j=0; j<50; j++){
// create image
  src = image_create( rows, cols );
  point_set3D(&(view.vrp), x , 2, 0.5);
  vector_set( &(view.vpn), -x, -2, -0.5);
  matrix_setView3D(&vtm, &view);  
  i=0;
  for(i=0;i<4;i++) {
    polygon_copy( &tpoly, &side[i] );
    matrix_xformPolygon( &vtm, &tpoly );

    // normalize by homogeneous coordinate before drawing
    polygon_normalize( &tpoly );


    polygon_draw( &tpoly, src, color[i] );
    //polygon_print( &tpoly, stdout );
  }
  
  printf("Writing image\n");
  sprintf(filename, "pyramid_%04d.ppm", j);
  image_write( src, filename);
  free(src);
  if(j<25){
  	x += 0.08;
  }else{
  	x -= 0.08;
  }
}

printf("Making the .gif file...\n");
system("convert -delay 10 pyramid_*.ppm ../images/ext1.gif");
system("rm -f pyramid*");




  return(0);
}
