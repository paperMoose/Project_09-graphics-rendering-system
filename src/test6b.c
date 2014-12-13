/*
Bruce A. Maxwell
Fall 2014

Example of a 3D scene model

Draws three 3D X-wings as line drawings
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

/*
  Adds to the module a unit cylinder, aligned along the Y-axis

  Takes in the number of subdivisions to use when creating the cylinder
 */
void cylinder( Module *mod, int sides );
void cylinder( Module *mod, int sides ) {
  Polygon p;
  Line l;
  Point xtop, xbot;
  double x1, x2, z1, z2;
  int i;

  polygon_init( &p );
  point_set3D( &xtop, 0, 1.0, 0.0 );
  point_set3D( &xbot, 0, 0.0, 0.0 );

  // make a fan for the top and bottom sides
  // and quadrilaterals for the sides
  for(i=0;i<sides;i++) {
    Point pt[4];

    x1 = cos( i * M_PI * 2.0 / sides );
    z1 = sin( i * M_PI * 2.0 / sides );
    x2 = cos( ( (i+1)%sides ) * M_PI * 2.0 / sides );
    z2 = sin( ( (i+1)%sides ) * M_PI * 2.0 / sides );

    point_copy( &pt[0], &xtop );
    point_set3D( &pt[1], x1, 1.0, z1 );
    point_set3D( &pt[2], x2, 1.0, z2 );

    polygon_set( &p, 3, pt );
    module_polygon( mod, &p );

    //point_copy( &pt[0], &xbot );
    // point_set3D( &pt[1], x1, 0.0, z1 );
    // point_set3D( &pt[2], x2, 0.0, z2 );

    // polygon_set( &p, 3, pt );
    // module_polygon( mod, &p );

    // point_set3D( &pt[0], x1, 0.0, z1 );
    // point_set3D( &pt[1], x2, 0.0, z2 );
    // point_set3D( &pt[2], x2, 1.0, z2 );
    // point_set3D( &pt[3], x1, 1.0, z1 );
    
    // polygon_set( &p, 4, pt );
    // module_polygon( mod, &p );
  }

  polygon_clear( &p );
}

// // adds a cube centered on the origin of width 2x2x2 to the module
// void cube( Module *mod );
// void cube( Module *mod ) {
//   Point pt[4];
//   Polygon p;

//   polygon_init( &p );
//   point_set3D( &pt[0], -1, -1, -1 );
//   point_set3D( &pt[1], -1, -1,  1 );
//   point_set3D( &pt[2], -1,  1,  1 );
//   point_set3D( &pt[3], -1,  1, -1 );
//   polygon_set( &p, 4, pt );
//   module_polygon( mod, &p );

//   point_set3D( &pt[0], 1, -1, -1 );
//   point_set3D( &pt[1], 1, -1,  1 );
//   point_set3D( &pt[2], 1,  1,  1 );
//   point_set3D( &pt[3], 1,  1, -1 );
//   polygon_set( &p, 4, pt );
//   module_polygon( mod, &p );

//   point_set3D( &pt[0], -1, -1, -1 );
//   point_set3D( &pt[1], -1, -1,  1 );
//   point_set3D( &pt[2],  1, -1,  1 );
//   point_set3D( &pt[3],  1, -1, -1 );
//   polygon_set( &p, 4, pt );
//   module_polygon( mod, &p );

//   point_set3D( &pt[0], -1, 1, -1 );
//   point_set3D( &pt[1], -1, 1,  1 );
//   point_set3D( &pt[2],  1, 1,  1 );
//   point_set3D( &pt[3],  1, 1, -1 );
//   polygon_set( &p, 4, pt );
//   module_polygon( mod, &p );

//   point_set3D( &pt[0], -1, -1, -1 );
//   point_set3D( &pt[1], -1,  1, -1 );
//   point_set3D( &pt[2],  1,  1, -1 );
//   point_set3D( &pt[3],  1, -1, -1 );
//   polygon_set( &p, 4, pt );
//   module_polygon( mod, &p );

//   point_set3D( &pt[0], -1, -1, 1 );
//   point_set3D( &pt[1], -1,  1, 1 );
//   point_set3D( &pt[2],  1,  1, 1 );
//   point_set3D( &pt[3],  1, -1, 1 );
//   polygon_set( &p, 4, pt );
//   module_polygon( mod, &p );

// }

// makes 3 X-wing fighters in a loose formation
int main(int argc, char *argv[]) {

  Image *src;
  Module *scene;
  Module *engine;
  Module *wing;
  Module *wings;
  Module *laser;
  Module *body;
  Polygon p;
  Point pt[4];
  View3D view;
  Matrix vtm, gtm;
  DrawState *ds;
  Color Flame = { { 1.0, 0.7, 0.2 } };
  Color Red =  { { 1.0, 0.2, 0.1 } };
  float bodyWidth = 4.0;
  char filename[100];
  Polygon view_poly_vrp;
  Point view_center;
  int view_numSides;

  // set up the view
  point_set3D( &(view.vrp), 0.1, 5, 0 );
  vector_set( &(view.vpn), -0.1, -5,0 );
  vector_set( &(view.vup), 0, 1, 0 );
  view.d = 1;
  view.du = 1.6;
  view.dv = 0.9;
  view.f = 1;
  view.b = 50;
  view.screenx = 640;
  view.screeny = 360;

  matrix_setView3D( &vtm, &view );
  matrix_identity( &gtm );

  // // engine
  engine = module_create();
  // module_scale( engine, 1.3, 6, 1.3);
  //module_rotateX( engine, 0, 1 );
  //cylinder( engine, 10 );
  //module_scale( engine, .8, .8, 1 );
  module_color( engine, &Flame );
  //module_cylinder( engine, 10 );
  //module_cube(engine,1);
  // module_circle(engine, 50);
  //module_scale(engine, 0.5, 0.5,0.5);
  //module_ellipse(engine, 1, 2, 50);
  module_circleFrame(engine, 25);

  // // // laser
  // laser = module_create();
  // module_scale( laser, 0.5, 5, 0.5 );
  // module_rotateX( laser, 0, 1 );
  // cylinder( laser, 6 );
  // module_scale( laser, 0.4, 0.4, 1.0 );
  // module_translate( laser, 0, 0, 4.5 );
  // module_color( laser, &Red );
  // cylinder( laser, 10 );

  // // // wing
  // wing = module_create();
  // polygon_init( &p );
  // point_set3D( &pt[0], 0.0, 0.0, 0.0 );
  // point_set3D( &pt[1], 0.0, 0.0, 5.0 );
  // point_set3D( &pt[2], 15.0, 0.0, 3.0 );
  // point_set3D( &pt[3], 15.0, 0.0, 0.0 );
  // polygon_set( &p, 4, pt );
  // module_polygon( wing, &p );
  // module_translate( wing, 0, 0.5, 0 );
  // module_polygon( wing, &p );
  
  // module_identity(wing);
  // module_translate( wing, 3, 1.6, -1 );
  // module_module( wing, engine );

  // module_identity(wing);
  // point_set3D( &pt[0], 15.0, 0.0, 3.0 );
  // point_set3D( &pt[1], 15.0, 0.0, 0.0 );
  // point_set3D( &pt[2], 15.0, 0.5, 0.0 );
  // point_set3D( &pt[3], 15.0, 0.5, 3.0 );
  // polygon_set( &p, 4, pt );
  // module_polygon( wing, &p );

  // point_set3D( &pt[0], 15.0, 0.0, 0.0 );
  // point_set3D( &pt[1], 0.0, 0.0, 0.0 );
  // point_set3D( &pt[2], 0.0, 0.5, 0.0 );
  // point_set3D( &pt[3], 15.0, 0.5, 0.0 );
  // polygon_set( &p, 4, pt );
  // module_polygon( wing, &p );

  // point_set3D( &pt[0], 15.0, 0.0, 3.0 );
  // point_set3D( &pt[1], 15.0, 0.5, 3.0 );
  // point_set3D( &pt[2], 0.0, 0.5, 5.0 );
  // point_set3D( &pt[3], 0.0, 0.0, 5.0 );
  // polygon_set( &p, 4, pt );
  // module_polygon( wing, &p );

  // module_translate( wing, 15, 0.25, 0 );
  // module_module( wing, laser );

  // // 4 wings
  // wings = module_create();
  // module_rotateZ(wings, cos( 0.3 ), sin( 0.3 ) );
  // module_translate(wings, bodyWidth, 0, 0 );
  // module_module( wings, wing );

  // module_identity( wings );
  // module_scale( wings, 1, -1, 1 );
  // module_rotateZ(wings, cos( -0.3) , sin(-0.3) );
  // module_translate(wings, bodyWidth, 0, 0 );
  // module_module( wings, wing );

  // module_identity( wings );
  // module_scale( wings, -1, 1, 1 );
  // module_rotateZ(wings, cos( -0.3) , sin(-0.3) );
  // module_translate( wings, -bodyWidth, 0, 0 );
  // module_module( wings, wing );

  // module_identity( wings );
  // module_scale( wings, -1, -1, 1 );
  // module_rotateZ(wings, cos( 0.3) , sin(0.3) );
  // module_translate( wings, -bodyWidth, 0, 0 );
  // module_module( wings, wing );

  // // body
  // body = module_create();

  // module_module( body, wings );

  // module_scale(body, bodyWidth, bodyWidth, 8 );
  // module_translate(body, 0, 0, 3 );
  // cube( body );

  // module_identity(body);
  // point_set3D( &pt[0], bodyWidth, bodyWidth, 12 );
  // point_set3D( &pt[1], bodyWidth, -bodyWidth, 12 );
  // point_set3D( &pt[2], bodyWidth*0.5, -bodyWidth*0.3, 35 );
  // point_set3D( &pt[3], bodyWidth*0.5, bodyWidth*0.3, 35 );
  // polygon_set( &p, 4, pt );
  // module_polygon( body, &p );

  // point_set3D( &pt[0], -bodyWidth, bodyWidth, 12 );
  // point_set3D( &pt[1], -bodyWidth, -bodyWidth, 12 );
  // point_set3D( &pt[2], -bodyWidth*0.5, -bodyWidth*0.3, 35 );
  // point_set3D( &pt[3], -bodyWidth*0.5, bodyWidth*0.3, 35 );
  // polygon_set( &p, 4, pt );
  // module_polygon( body, &p );

  // point_set3D( &pt[0], -bodyWidth, bodyWidth, 12 );
  // point_set3D( &pt[1],  bodyWidth, bodyWidth, 12 );
  // point_set3D( &pt[2],  bodyWidth*0.5, bodyWidth*0.3, 35 );
  // point_set3D( &pt[3], -bodyWidth*0.5, bodyWidth*0.3, 35 );
  // polygon_set( &p, 4, pt );
  // module_polygon( body, &p );

  // point_set3D( &pt[0], -bodyWidth, -bodyWidth, 12 );
  // point_set3D( &pt[1],  bodyWidth, -bodyWidth, 12 );
  // point_set3D( &pt[2],  bodyWidth*0.5, -bodyWidth*0.3, 35 );
  // point_set3D( &pt[3], -bodyWidth*0.5, -bodyWidth*0.3, 35 );
  // polygon_set( &p, 4, pt );
  // module_polygon( body, &p );

  // point_set3D( &pt[0], -bodyWidth*0.5,  bodyWidth*0.3, 35 );
  // point_set3D( &pt[1],  bodyWidth*0.5,  bodyWidth*0.3, 35 );
  // point_set3D( &pt[2],  bodyWidth*0.5, -bodyWidth*0.3, 35 );
  // point_set3D( &pt[3], -bodyWidth*0.5, -bodyWidth*0.3, 35 );
  // polygon_set( &p, 4, pt );
  // module_polygon( body, &p );

  // scene with one ships (body module)
  scene = module_create();
  //module_scale( scene, 0.7, 0.7, 0.7 );
  //module_rotateZ( scene, cos( 0.5 ), sin( 0.5 ) );
  //module_rotateY( scene, cos(-0.1), sin(-0.1) );
  //module_rotateX( scene, cos( 0.2), sin(0.2) );
  //module_translate( scene, -15, -10, -10 );
  // module_module( scene, body );
  module_module(scene, engine);


  // create the image and drawstate
  
  // ds = drawstate_create();
  // ds->shade = ShadeFrame;
  // // draw into the scene
  // src = image_create( 360, 640 );
  // module_draw( scene, &vtm, &gtm, ds, &view.vpn, src );
  // //  write out the scene
  // image_write( src, "frame.ppm" );
  // // free the image
  // image_free( src );

  //for animation
  view_numSides = 50;
  point_set3D(&view_center, 0.0,0.0,0.0);
  view_rotate_circle(&view_poly_vrp, &view_center, view_numSides, 4.0, 0.0, 0.0, 90);
  printf("Printing view_poly_vrp\n");
  polygon_print(&view_poly_vrp, stdout);
  int k;
  for(k=0; k<view_poly_vrp.numVertex; k++){
    point_set3D( &(view.vrp), view_poly_vrp.vertex[k].val[0], view_poly_vrp.vertex[k].val[1], view_poly_vrp.vertex[k].val[2] );
    vector_set( &(view.vpn), -view.vrp.val[0], -view.vrp.val[1],-view.vrp.val[2]);
    matrix_setView3D( &vtm, &view );


    src = image_create( 360, 640 );
    module_draw( scene, &vtm, &gtm, ds, /*&view.vpn* ,*/ src );
    //  write out the scene
    sprintf(filename, "frame_%.3d.ppm",k);
    image_write( src, filename );
    // free the image
    image_free( src );
  }
  system("convert -delay 20 ./frame_*.ppm ../images/cubeFrame.gif");
  system("rm -f ./frame_*");

	// free the polygon data
  //polygon_clear( &p );

	// free the modules
  module_clear( scene );
  // module_clear( wing );
  // module_clear( wings );
  // module_clear( laser );
  // module_clear( body );
  module_clear( engine );

	// free the drawstate
	free(ds);

	

  return(0);
}
