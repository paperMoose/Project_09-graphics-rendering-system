/*
Erfan Azad
Date: Nov 3 2014
File: task4_4.c
Discription: creates a simple 2d image and zooms in and out and pans around the screen
making a simple gif.
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "view.h"
#include "matrix.h"
#include "color.h"
#include "point.h"
#include "polyline.h"
#include "polygon.h"
#include "line.h"

int main(int argc, char* argv[]){
	const int rows = 500;
	const int cols = 500;
	View2D view;
	Matrix vtm;
	Matrix ltm;
	Image *src;
	Polygon head, tempPoly;
	Line body[5];
	Line tempLine;
	Point p[11];
	Color white;
	int i,k=0;
	char filename[100];

	Color_set(&white, 1.0,1.0,1.0);

	point_set2D(&p[0], -0.5, 1);//head
	point_set2D(&p[1], 0.5, 1);
	point_set2D(&p[2], 0.5, 0);
	point_set2D(&p[3], -0.5, 0);


	point_set2D(&p[4], 0, 0);//body
	point_set2D(&p[5], 0, -1);//first devision
	point_set2D(&p[6], 1, -1.5);//RH
	point_set2D(&p[7], -1, -1.5);//LH
	point_set2D(&p[8], 0, -3);//end of body
	point_set2D(&p[9], -0.5, -4);//RF
	point_set2D(&p[10], 0.5, -4);//LF

	polygon_init(&head);
	polygon_set(&head, 4, &p[0]);

	line_set(&body[0], p[4], p[8]);//main body
	line_set(&body[1], p[5], p[6]);//RH
	line_set(&body[2], p[5], p[7]);//LH
	line_set(&body[3], p[8], p[9]);//RF
	line_set(&body[4], p[8], p[10]);//LF

	//setting up the view
	point_set2D( &(view.vrp), 0, 0);
	view.dx = 10;
	vector_set( &(view.x), 1.0, 0.0, 0.0 );
	view.screenx = cols;
	view.screeny = rows;

	matrix_setView2D( &vtm, &view );

	for(k=0; k<10; k++){
		src = image_create( rows, cols );

		matrix_identity(&ltm);
		matrix_rotateZ(&ltm, cos(k*5*M_PI/180), sin(k*5*M_PI/180));
		polygon_copy(&tempPoly, &head);
		matrix_xformPolygon(&ltm, &tempPoly);

		matrix_xformPolygon(&vtm, &tempPoly);
		polygon_draw(&tempPoly, src, white);
		for(i=0; i<5; i++){
			line_copy(&tempLine, &body[i]);
			matrix_xformLine(&vtm, &tempLine);
			line_draw(&tempLine, src, white);
		}

		sprintf(filename, "frame-%04d.ppm", k);
		image_write( src, filename);
		

		//setting up the new vtm
		point_set2D( &(view.vrp), 0, 0-(k/5.0));
		matrix_setView2D( &vtm, &view );
		view.dx += 0.5;

		image_free(src);
	}
	system("convert -delay 20 ./frame-* ../images/task5_4.gif");
	system("rm -f ./frame-*");

	//free resources
	polygon_clear(&head);
	
	





	



	return 0;
}



