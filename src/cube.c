/*
  Bruce Maxwell
  Fall 2014

  A simple test function for Module_draw that creates 3 X-wing fighters as line drawings

  Requires the following functions to be defined

  element_create
  element_init
  element_delete

  module_create
  module_add
  module_delete
  module_translate2D
  module_scale2D
  module_point
  module_line
  module_module
  module_draw

  drawstate_create
  drawstate_setColor
  drawstate_copy

  vector_set

  point_set2D
  point_copy

  line_set2D
  line_set
  line_copy

  matrix_setView2D
 */
#include <stdlib.h>
#include <stdio.h>
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
  View3D view;
  Matrix vtm;
  Matrix gtm;
  Module *scene;
  DrawState *ds;
  Image *src;

	srand48(42);

  // setup gtm
  matrix_identity( &gtm );

  // setup vtm
  point_set3D( &view.vrp, -3, -2, -2);
  //view2D_set( &view, &vrp, 2, &xaxis, 640, 360 );
  // view.vrp = vrp;
  vector_set( &(view.vpn), -view.vrp.val[0], -view.vrp.val[1], -view.vrp.val[2] );
  view.du = 3.0;
  view.screeny = 360;
  view.screenx = 640;
  view.dv = view.du * (float) view.screeny / view.screenx;
  view.d = 5.0;
  vector_set( &(view.vup), 0, 1, 0 );
  view.f = 0; // front clip plane
  view.b = 4; // back clip plane
  matrix_setView3D( &vtm, &view );

  // make a scene
  scene = module_create();
  module_cube( scene, 1 );

	// // create the image and draw the module
  src = image_create( view.screeny, view.screenx );
  ds = drawstate_create(); // default color is white
  ds->shade = ShadeConstant;

  module_draw( scene, &vtm, &gtm, ds, /*&view.vpn,*/ src );

	// write out the image
  image_write( src, "cube.ppm" );

	// // free modules
  module_delete( scene );
	// free drawstate
  free( ds );

	// // free image
  image_free( src );

  return(0);
}
