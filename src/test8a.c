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


int main(int argc, char *argv[]) {
  Image *src;
  Matrix VTM;
  Matrix GTM;
  Module *cube;
  Module* cylinder;
  Lighting *light;
  int rows = 360;
  int cols = 640;

  Color Grey = {{1, 1, 1}};
  Color Blue = {{0.117647, 0.564706, 1}};

  DrawState *ds;
  View3D view;

  // initialize the image
  src = image_create(rows, cols);

  // initialize matrices
  matrix_identity(&GTM);
  matrix_identity(&VTM);

  // set the View parameters
  point_set3D(&(view.vrp), 5, 5, -7.0);
  vector_set(&(view.vpn), -5, -5, 7);
  vector_set(&(view.vup), 0.0, 1.0, 0.0);
  view.d = 1.0;
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
  module_color( cube, &Blue );
  module_cube( cube);

  cylinder = module_create();
  module_scale(cylinder, 1,3,1);
  module_translate(cylinder, 0, -1, 0);
  module_color(cylinder, &Grey);
  module_cylinder(cylinder, 20);


  ds = drawstate_create();
  // ds->shade = ShadeDepth;
  ds->shade = ShadeGouraud;

  light = lighting_create();
  lighting_add( light, LightPoint, &Grey, NULL, &view.vrp, 0, 0);

  matrix_identity(&GTM);
  drawstate_setAlpha(ds, 1);
  module_draw(cylinder,&VTM, &GTM, ds, light, src);
  drawstate_setAlpha(ds, 0.8);
  module_draw(cube, &VTM, &GTM, ds, light, src);
  


  // write out the image
  image_write(src, "test8a.ppm");

  // free stuff here
  module_delete( cube );
  image_free( src );
  

  return(0);
}
