/*
Erfan Azad
Date: 22 November 2014
this is the file that helps us debug the zbuffer algorithm
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




// makes 3 X-wing fighters in a loose formation
int main(int argc, char *argv[]) {
  int i, j; //loop variables

  Image *src;
  Module* wall;
  Module* ray;
  Module* ray2;
  Module *scene1;
  Module* scene2;
  Polygon p;
  Line l;
  Point point[4];
  Point point2[2];
  View3D view;
  Matrix vtm, gtm;
  DrawState *ds;
  char filename[100];
  Color Flame = { { 1.0, 0.7, 0.2 } };
  Color Red =  { { 1.0, 0.2, 0.1 } };
  Color Grey =  { { 0.745, 0.745, 0.745} };
  Color Blue = {{0.117647, 0.564706, 1}};
  // Color Grey = {{1, 1, 1}};


  // set up the view
  point_set3D( &(view.vrp), 4, 4, 4 );
  vector_set( &(view.vpn), -view.vrp.val[0], -view.vrp.val[1], -view.vrp.val[2] );
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

  // //wall
  wall = module_create();
  module_color(wall, &Red);
  polygon_init(&p);
  point_set3D(&point[0], 1,1,2);
  point_set3D(&point[1], 1,0,2);
  point_set3D(&point[2], 0,0,2);
  point_set3D(&point[3], 0,1,2);
  polygon_set(&p, 4, &point[0]);
  module_polygon(wall, &p);

//ray
  ray = module_create();
  module_color(ray, &Blue);
  for(i=0; i< 5; i++){
  point_set3D(&point2[0], -1+0.01*i, -1, 1);
  point_set3D(&point2[1], 1+0.01*i, 1, 1);
  line_set(&l, point2[0], point2[1]);
  module_line(ray, &l);
 }

 //ray2

  ray2 = module_create();
  module_color(ray2, &Red);
  for(i=0; i< 5; i++){
  point_set3D(&point2[0], -1+0.01*i, 1, -1);
  point_set3D(&point2[1], 1+0.01*i, -1, -1);
  line_set(&l, point2[0], point2[1]);
  // line_zBuffer(&l, 0);
  module_line(ray2, &l);
 }



//scene
    // scene = module_create();
    // module_module(scene, wall);
    // module_module(scene, ray);
    // module_module(scene, ray2);
    // module_module(scene, wall);
    



    

for(i=0; i< 36; i++){

	//scene
    scene1 = module_create();
    scene2 = module_create();
    module_rotateZ(scene1, cos(i*10 * M_PI/180), sin(i*10 * M_PI/180));
    module_scale( scene1, 3, 1, 2 );
    module_color( scene1, &Blue );
    module_cube( scene1, 1);


    module_scale(scene2, 0.5, 0.5, 0.5);
    module_cylinder(scene2, 30);
	// create the image and drawstate
	src = image_create( 360, 640 );
	ds = drawstate_create();
  drawstate_setAlpha(ds, 1);
	ds->shade = ShadeDepth;

	// draw into the scene
  // module_draw( scene1, &vtm, &gtm, ds, src );
  drawstate_setAlpha(ds, 1 );
	module_draw( scene1, &vtm, &gtm, ds, src );

	// write out the scene
	sprintf(filename, "frame_%.2d.ppm", i);
	image_write( src, filename );
	module_delete( scene1);

}
	 


	//free the polygon data
	// polygon_clear( &p );

	// free the modules
	// module_delete( scene);
	// module_delete( wall );


	// free the drawstate
	free(ds);

	// free the image
	image_free( src );

	return(0);
}

