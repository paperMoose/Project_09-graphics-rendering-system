/* Erfan Azad
Date: 12 November 2014
File: task6_3.c
Description : a personal scene 
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
  int i, j, k; //loop variables
  Point viewCenter; //center of the animation
  Image *src;
  Module* square_tower;
  Module* pyramid_tower;
  Module* circular_tower;
  Module* castle;
  Module* wall;
  Module *scene;
  Polygon p;
  Polygon poly_vrp;
  View3D view;
  Matrix vtm, gtm;
  DrawState *ds;
  char filename[100];
  Color Flame = { { 1.0, 0.7, 0.2 } };
  Color Red =  { { 1.0, 0.2, 0.1 } };
  Color Grey =  { { 0.745, 0.745, 0.745} };
  Color Blue = {{0.117647, 0.564706, 1}};


  // set up the view
  point_set3D( &(view.vrp), 40, 10, 30 );
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

  //wall
  wall = module_create();
  module_color(wall, &Red);
  for(i=1; i<3; i++){
    for(j=0; j<5; j++){
      module_cube(wall, 1);
      module_translate(wall, 0.0,0.0,2.0);
    }
    module_identity(wall);
    module_translate(wall, 0.0,i*2.0,0.0);
  }

//square_tower
square_tower = module_create();
module_color(square_tower, &Grey);
for(i=0; i<3; i++){
  module_cube(square_tower, 1);
  module_translate(square_tower, 0.0, 2.0, 0.0);
}
module_color(square_tower, &Red);
module_translate(square_tower, 0.0, -6.0, 0.0);
module_scale(square_tower, 1.8,1.8,1.8);
module_rotateY(square_tower, cos(45*M_PI/180), sin(45*M_PI/180));
module_translate(square_tower, 0.0, 5.0, 0.0);
module_pyramid(square_tower, 4);


//circular_tower
circular_tower = module_create();
module_color(circular_tower, &Grey);
for(i=0; i<3; i++){
  module_cylinder(circular_tower, 10);
  module_translate(circular_tower, 0.0, 1.0, 0.0);
}
// module_translate(circular_tower, 0.0, -1.0, 0.0);
module_color(circular_tower, &Red);
module_pyramid(circular_tower, 10);


//pyramid_tower
pyramid_tower = module_create();
module_color(pyramid_tower, &Blue);
for(i=0; i<20; i++){
  module_scale(pyramid_tower, 0.5+i*0.5,0.5+i*0.5,0.5+i*0.5);
  module_pyramid(pyramid_tower, 3);
  module_identity(pyramid_tower);
}



  //castle
  castle = module_create();
  
  module_scale(castle, 4,4,4);
  module_translate(castle, 6, -1 ,6);
  module_module(castle, circular_tower);

  module_identity(castle);
  module_module( castle, square_tower);
  module_translate(castle, 0.0,0.0,2.0);
  module_module(castle, wall);
  module_translate(castle, 0.0,0.0,-2.0);
  
  module_translate(castle, 0.0, 0.0, 12);
  module_module( castle, square_tower);
  module_translate(castle, 0.0, 0.0, -12);

  module_rotateY(castle, cos(M_PI/2), sin(M_PI/2));
  module_translate(castle, 2.0, 0.0, 12);
  module_module(castle, wall);

  module_translate(castle, 10.0, 0.0, 0.0);
  module_module(castle, square_tower);

  module_identity(castle);
  module_rotateY(castle, cos(M_PI), sin(M_PI));
  module_translate(castle, 12,0,12);
  module_module(castle, square_tower);
  
  module_translate(castle, 0,0,-2);
  module_module(castle, wall);

  module_identity(castle);
  module_rotateY(castle, cos(M_PI/2), sin(M_PI/2));
  module_translate(castle, 2, 0, 0);
  module_module(castle, wall);
  module_translate(castle, 10, 0, 0);
  module_module(castle, square_tower);


//scene
    scene = module_create();
    module_translate(scene, 6, 0,-6);
    module_module(scene, castle);
    module_identity(scene);

    module_rotateY(scene, cos(60*M_PI/180), sin(60*M_PI/180));
    module_module(scene, pyramid_tower);

    module_identity(scene);

    module_translate(scene, 6, 0,-6);
    module_rotateY(scene, cos(120*M_PI/180), sin(120*M_PI/180));
    module_module(scene, castle);

    module_rotateY(scene, cos(120*M_PI/180), sin(120*M_PI/180));
    module_module(scene, castle);





  //animation
  point_set3D(&viewCenter, 0, 0.5, 0);
  view_rotate_circle(&poly_vrp, &viewCenter, 50, 40, 20, 0, 0);
  for(k=0; k<50; k++){
    point_set3D( &(view.vrp), poly_vrp.vertex[k].val[0], poly_vrp.vertex[k].val[1], poly_vrp.vertex[k].val[2]);
    vector_set( &(view.vpn), -view.vrp.val[0], -view.vrp.val[1], -view.vrp.val[2] );
    matrix_setView3D( &vtm, &view );
    


    // create the image and drawstate
    src = image_create( 360, 640 );
    ds = drawstate_create();
    ds->shade = ShadeFrame;

    // draw into the scene
    module_draw( scene, &vtm, &gtm, ds, src );

    // write out the scene
    sprintf(filename, "frame_%.2d.ppm",k);
    image_write( src, filename );
  }
  system("convert -delay 10 ./frame_* ../images/tower.gif");

	//free the polygon data
  polygon_clear( &p );

	// free the modules
  module_delete( castle );
  module_delete( wall );
  module_delete(square_tower);
  module_delete(circular_tower);

	// free the drawstate
	free(ds);

	// free the image
  image_free( src );

  return(0);
}