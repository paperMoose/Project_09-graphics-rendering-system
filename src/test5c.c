
/*
  Bruce Maxwell
  Fall 2014

  Test function for 3D matrix viewing
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

void cubeDraw(Matrix vtm, Polygon tpoly, Polygon* side, Image* src, Color* color );
void cubeDraw(Matrix vtm, Polygon tpoly, Polygon* side, Image* src, Color* color){
  int i;
  for(i=0;i<6;i++) {
    polygon_copy( &tpoly, &side[i] );
    matrix_xformPolygon( &vtm, &tpoly );

    // normalize by homogeneous coordinate before drawing
    polygon_normalize( &tpoly );


    polygon_drawFill( &tpoly, src, color[i] );
    polygon_print( &tpoly, stdout );
  }
}
int main(int argc, char *argv[]) {
  const int rows = 1000;
  const int cols = 1000;
  View3D view;
  Matrix vtm;
  Polygon side[6];
  Polygon tpoly;
  Point  tv[4];
  Point  v[8];
  Color  color[6];
  Image *src;
  int i;

  // set some colors
  Color_set( &color[0], 0, 0, 1 );
  Color_set( &color[1], 0, 1, 0 );
  Color_set( &color[2], 1, 0, 0 );
  Color_set( &color[3], 1, 0, 1 );
  Color_set( &color[4], 0, 1, 1 );
  Color_set( &color[5], 1, 1, 0 );

  // initialize polygons
  for(i=0;i<6;i++) {
    polygon_init( &side[i] );
  }

  // corners of a cube, centered at (0, 0, 0)
  point_set3D( &v[0], -1, -1, -1 );
  point_set3D( &v[1],  1, -1, -1 );
  point_set3D( &v[2],  1,  1, -1 );
  point_set3D( &v[3], -1,  1, -1 );
  point_set3D( &v[4], -1, -1,  1 );
  point_set3D( &v[5],  1, -1,  1 );
  point_set3D( &v[6],  1,  1,  1 );
  point_set3D( &v[7], -1,  1,  1 );

  // front side
  polygon_set( &side[4], 4, &(v[0]) );

  // back side
  polygon_set( &side[2], 4, &(v[4]) );

  // top side
  point_copy( &tv[0], &v[2] );
  point_copy( &tv[1], &v[3] );
  point_copy( &tv[2], &v[7] );
  point_copy( &tv[3], &v[6] );

  polygon_set( &side[5], 4, tv );

  // bottom side
  point_copy( &tv[0], &v[0] );
  point_copy( &tv[1], &v[1] );
  point_copy( &tv[2], &v[5] );
  point_copy( &tv[3], &v[4] );

  polygon_set( &side[1], 4, tv );

  // left side
  point_copy( &tv[0], &v[0] );
  point_copy( &tv[1], &v[3] );
  point_copy( &tv[2], &v[7] );
  point_copy( &tv[3], &v[4] );

  polygon_set( &side[0], 4, tv );

  // right side
  point_copy( &tv[0], &v[1] );
  point_copy( &tv[1], &v[2] );
  point_copy( &tv[2], &v[6] );
  point_copy( &tv[3], &v[5] );

  polygon_set( &side[3], 4, tv );

  // grab command line argument to determine viewpoint
  // and set up the view structure
  if( argc > 1 ) {
    float alpha = atof( argv[1] );
    if( alpha < 0.0 || alpha > 1.0 )
      alpha = 0.0;

    point_set3D( &(view.vrp), 3*alpha, 2*alpha, -2*alpha - (1.0-alpha)*3 );
  }
  else {
    point_set3D( &(view.vrp), 9, 6, -6 );
  }
  vector_set( &(view.vpn), -view.vrp.val[0], -view.vrp.val[1], -view.vrp.val[2] );

  vector_set( &(view.vup), 0, 1, 0 );
  view.d = 1;  // focal length
  view.du = 2;
  view.dv = view.du * (float)rows / cols;
  view.f = 0; // front clip plane
  view.b = 4; // back clip plane
  view.screenx = cols;
  view.screeny = rows;

  matrix_setView3D( &vtm, &view );


  // create image
  src = image_create( rows, cols );

  // use a temprary polygon to transform stuff
  polygon_init( &tpoly );
  matrix_translate(&vtm, 0.0 ,100.0 ,0.0);
  printf("Drawing Polygons\n");
  cubeDraw( vtm,  tpoly,  side,  src, color);
 

  matrix_translate(&vtm, 50.0 ,0.0 ,0.0);
  cubeDraw( vtm,  tpoly,  side,  src, color);
  
  printf("Writing image\n");
  image_write( src, "cube3.ppm" );

  return(0);
}
