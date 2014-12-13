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
	Polygon tree[4];
	Polygon tempTree;
	Point v[13];
	Point tempV[4];
	Color color[4];
	Image* src;
	char filename[100];

	//setting the colors for the tree
	Color_set(&color[0], 0.56, 0.93, 0.56);//green
	Color_set(&color[1], 0.80, 0.52, 0.24);//brown
	Color_set(&color[2], 0.19, 0.80, 0.19);//dark green
	Color_set(&color[3], 0.54, 0.27, 0.07);//dark brown
	

	//initialize polygons
	polygon_init(&tree[0]);
	polygon_init(&tree[1]);
	polygon_init(&tree[2]);
	polygon_init(&tree[3]);

	//triangle of the tree
	point_set3D( &v[0], 0, 0, 5);//top of the tree
	point_set3D( &v[1], -2, 0, 2);
	point_set3D( &v[2], 2, 0, 2);
	point_set3D( &v[3], 1, 0, 2);//root
	point_set3D( &v[4], -1, 0, 2);
	point_set3D( &v[5], 1, 0, 0);
	point_set3D( &v[6], -1, 0, 0);
	//second tree
	point_set3D( &v[7], 0, -2, 2);
	point_set3D( &v[8], 0, 2, 2);
	point_set3D( &v[9], 0, 1, 2);//root
	point_set3D( &v[10], 0, -1, 2);
	point_set3D( &v[11], 0, 1, 0);
	point_set3D( &v[12], 0, -1, 0);

	//first tree
	polygon_set(&tree[0], 3, &v[0]);//triangle
	polygon_set(&tree[1], 4, &v[3]);//root

	//second tree
	//triangle
	point_copy(&tempV[0], &v[0]);
  	point_copy(&tempV[1], &v[7]);
  	point_copy(&tempV[2], &v[8]);
  	point_copy(&tempV[3], &v[12]);
  	polygon_set(&tree[2], 3, &tempV[0]);
  	//root
  	point_copy(&tempV[0], &v[9]);
  	point_copy(&tempV[1], &v[10]);
  	point_copy(&tempV[2], &v[11]);
  	point_copy(&tempV[3], &v[12]);
  	polygon_set(&tree[3], 4, &tempV[0]);

  	//setting the viewpoint and the view structure
  	point_set3D( &(view.vrp), 3, 7, 3);
  	vector_set( &(view.vpn), -view.vrp.val[0], -view.vrp.val[1], -view.vrp.val[2]);
  	vector_set( &(view.vup), 0, 0, 1 );
	view.d = 1;  // focal length
	view.du = 2;
	view.dv = view.du * (float)rows / cols;
	view.f = 0; // front clip plane
	view.b = 7; // back clip plane
	view.screenx = cols;
	view.screeny = rows;

	matrix_setView3D( &vtm, &view );

	// use a temprary polygon to transform stuff
  	polygon_init( &tempTree);

  	//draw
  	printf("Drawing the trees\n");
  	int i,j;
  	double x= -5;
  	for (j=0; j<50; j++){
  		src = image_create( rows, cols );
  		point_set3D( &(view.vrp), x, 5, 3);
  		vector_set( &(view.vpn), -view.vrp.val[0], -view.vrp.val[1], -view.vrp.val[2]+j/10.0);
  		matrix_setView3D( &vtm, &view );

	  	for(i=0;i<4;i++) {
	    	polygon_copy( &tempTree, &tree[i] );
	    	matrix_xformPolygon( &vtm, &tempTree );
	    	// normalize by homogeneous coordinate before drawing
	    	polygon_normalize( &tempTree );
	    	polygon_drawFill( &tempTree, src, color[i]);
	    	//polygon_print( &tempTree, stdout );
	  	}
	  	x+=0.1;
	  	sprintf(filename, "tree_%04d.ppm", j);
	  	//write the image
  		image_write(src, filename);
  		//free resources
  		image_free(src);
  	}
  	system("convert -delay 10 tree_*.ppm ../images/tree.gif");
	system("rm -f tree_*");
}
















