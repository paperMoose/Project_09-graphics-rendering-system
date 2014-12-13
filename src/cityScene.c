/*
Ryan brandt
  Fall 2014

  Example code for the z-buffering!

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
	Vector nlist[4]; 
	Lighting *l;
	Point lp;
	Matrix VTM;
	Matrix GTM;
	Module *chassis;
	Module *scene;
	Point p[4];
	Polygon poly;
	float angle;
	int rows = 400;
	int cols = 400;
	int i;

	Color Green;
	Color Turquoise;
	Color Blue;
	Color BlueGrey;
	Color Sun;

	DrawState *ds;
	View3D view;

	Color_set( &Green, 39/255.0, 255/255.0, 15/255.0 );
	Color_set( &Turquoise, 0, 238/255.0, 255/255.0 );
	Color_set( &Blue, 0, 0, 1 );
	Color_set(&BlueGrey, 0.2, 0.25, 0.3);
	Color_set(&Sun, 0.9, 0.85, 0.8);

	// initialize the image
	src = image_create(rows, cols);

	polygon_init(&poly);

	// initialize matrices
	// matrix_identity(&GTM);
	// matrix_identity(&VTM);

	// set the View parameters
	// point_set3D(&(view.vrp), 0.0, 0.0, -20.0);
	// vector_set(&(view.vpn), 0.0, 0.0, 1.0);
	// vector_set(&(view.vup), 0.0, 1.0, 0.0);
	// view.d = 2.0;
	// view.du = 1.0;
	// view.dv = 1.0;
	// view.f = 0.0;
	// view.b = 50;
	// view.screenx = cols;
	// view.screeny = rows;
	// matrix_setView3D(&VTM, &view);


	point_set3D(&p[0],0,0,1);
	point_set3D(&p[1],100,0,1);
	point_set3D(&p[2],100,100,1);
	point_set3D(&p[3],0,100,1);
	polygon_set(&poly,4,p);
	polygon_set(&poly,4,p);
	polygon_setSided(&poly, 0);
	int k;
	for(k=0; k<4; k++){
		vector_set(&nlist[k], 0, 0, 1);
	}
	polygon_setNormals(&poly, 4, nlist);


	// print out VTM
	// printf("Final VTM: \n");
	// matrix_print(&VTM, stdout);

	chassis = module_create();
	module_polygon(chassis,&poly);


	// make a scene with lots of cube sets
	scene = module_create();
	module_module(scene,chassis);


	ds = drawstate_create();
	// ds->shade = ShadeDepth;
	ds->shade = ShadeGouraud;

	l = lighting_create();
  	lighting_add( l, LightAmbient, &BlueGrey, NULL, NULL, 0.0, 0.0 );
  	lighting_add( l, LightPoint, &Sun, NULL, &lp, 0.0, 0.0 );

	polygon_drawShade(&poly, src, ds, l);
	// for(i=0;i<36;i++) {
	// 	char buffer[256];

	// 	image_reset( srcGouroud
	// 	matrix_identity(&GTM);

	// 	matrix_rotateY(&GTM, cos(i*2*M_PI/36.0), sin(i*2*M_PI/36.0));
		// module_draw(scene, &VTM, &GTM, ds, /*NULL,*/ src);
	


		// write out the image
		// sprintf(buffer, "frame-%03d.ppm", i);
		image_write(src, "test.ppm");
	// }

	// free stuff here
	module_delete( scene );
	image_free( src );


	return(0);
}
