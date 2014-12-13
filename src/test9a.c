/*
  Bruce A. Maxwell
  Fall 2014

  Example code for drawing a single cube

  C Version
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "module.h"
#include "drawState.h"
#include "line.h"
#include "point.h"
#include "polyline.h"
#include "polygon.h"
#include "circle.h"
#include "color.h"
#include "Image.h"
#include "matrix.h"
#include "view.h"
#include "light.h"


int main(int argc, char *argv[]) {
  Image *src;
  Matrix VTM;
  Matrix GTM;
  Module *cube;
  int rows = 360;
  int cols = 640;

  Color White;
  Color Grey;
  Color BlueGrey;

  DrawState *ds;
  View3D view;

  Lighting *light;

	Color_set( &White, 1.0, 1.0, 1.0 );
	Color_set( &Grey, 0.6, 0.62, 0.64 );
  Color_set(&BlueGrey, 0.2, 0.25, 0.3);

  // initialize the image
  src = image_create(rows, cols);

  // initialize matrices
  matrix_identity(&GTM);
  matrix_identity(&VTM);

  // set the View parameters
  point_set3D(&(view.vrp), 5, 5, -7.0);
  vector_set(&(view.vpn), -5, -5, 7);
  vector_set(&(view.vup), 0.0, 1.0, 0.0);
  view.d = 2.0;
  view.du = 1.6;
  view.dv = .9;
  view.f = 0.0;
  view.b = 15;
  view.screenx = cols;
  view.screeny = rows;
  matrix_setView3D(&VTM, &view);

  // print out VTM
  printf("Final VTM: \n");
  matrix_print(&VTM, stdout);

  // make a simple cube module
  cube = module_create();
  module_scale( cube, 3, 1, 2 );
  module_scale(cube, 0.5, 0.5, 0.5);

  // this would color the cube in ShadeConstant mode
  module_color( cube, &Grey );

  // the example cube is blue (Y/-Y), red (Z/-Z), yellow (X/-X)
  // these colors should be the body colors
  module_cube( cube);

  // manually add a light source to the Lighting structure
  // put it in the same place as the eye in world space
  light = lighting_create();
  Vector new;
  Vector new1;
  vector_set(&new,-3,-4,7);
  vector_set(&new1,5, 5, -14.0);

  lighting_add( light, LightSpot, &White, &(new), &new1, cos(11*M_PI/180), 100 );
    lighting_add( light, LightAmbient, &BlueGrey, NULL, NULL, 0, 0 );

  // set the shading to Gouraud
  ds = drawstate_create();
  point_copy(&(ds->viewer), &(view.vrp));
	// ds->shade = ShadeGouraud;
		ds->shade = ShadePhong;

  matrix_identity(&GTM);
  module_draw(cube, &VTM, &GTM, ds, light, src);

  // write out the image
  image_write(src, "test9a.ppm");

  // free stuff here
  module_delete( cube );
  image_free( src );
  

  return(0);
}
