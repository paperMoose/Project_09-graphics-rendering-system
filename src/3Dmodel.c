/*
Erfan Azad
25 Sep 2014
3Dmodel.c
This file creates 3d shapes using the files in the library, mostly lines, circles and ellipses.
*/

#include <stdio.h>
#include <stdlib.h>
#include "polyline.h"
#include "point.h"
#include "color.h"
#include "Image.h"
#include "line.h"
#include "circle.h"
#include "ellipse.h"

int shape(Image* src, Color color, double* x_coord, double* y_coord);
int shape(Image* src, Color color, double* x_coord, double* y_coord){
	Line l;
	int i;
	for(i=0; i< 3; i++){ //front side
		line_set2D(&l, x_coord[i], y_coord[i], x_coord[i+1], y_coord[i+1]);
		line_draw(&l, src, color);
	}

	line_set2D(&l, x_coord[3], y_coord[3], x_coord[0], y_coord[0]);
	line_draw(&l, src, color);


	line_set2D(&l, x_coord[4], y_coord[4], x_coord[5], y_coord[5]);
	line_draw(&l, src, color);

	line_set2D(&l, x_coord[5], y_coord[5], x_coord[6], y_coord[6]);
	line_draw(&l, src, color);

	line_set2D(&l, x_coord[6], y_coord[6], x_coord[7], y_coord[7]);
	line_drawDash(&l, src, color);

	line_set2D(&l, x_coord[7], y_coord[7], x_coord[4], y_coord[4]);
	line_drawDash(&l, src, color);



	line_set2D(&l, x_coord[0], y_coord[0], x_coord[4], y_coord[4]);
	line_draw(&l, src, color);

	line_set2D(&l, x_coord[1], y_coord[1], x_coord[5], y_coord[5]);
	line_draw(&l, src, color);

	line_set2D(&l, x_coord[2], y_coord[2], x_coord[6], y_coord[6]);
	line_draw(&l, src, color);

	line_set2D(&l, x_coord[3], y_coord[3], x_coord[7], y_coord[7]);
	line_drawDash(&l, src, color);

	//side coens
	int j=0;
	for(j=0; j<2; j++){
		line_set2D(&l, x_coord[j], y_coord[j], x_coord[8], y_coord[8]);
		line_draw(&l, src, color);
		if(j+4 == 4){
			line_set2D(&l, x_coord[j+4], y_coord[j+4], x_coord[8], y_coord[8]);
			line_drawDash(&l, src, color);
		}else{
			line_set2D(&l, x_coord[j+4], y_coord[j+4], x_coord[8], y_coord[8]);
			line_draw(&l, src, color);
		}
	}

	int k;
	for(k=1; k<3; k++){
		line_set2D(&l, x_coord[k], y_coord[k], x_coord[9], y_coord[9]);
		line_draw(&l, src, color);
		if(k+4 == 6){
			line_set2D(&l, x_coord[k+4], y_coord[k+4], x_coord[9], y_coord[9]);
			line_drawDash(&l, src, color);
		}else{
			line_set2D(&l, x_coord[k+4], y_coord[k+4], x_coord[9], y_coord[9]);
			line_draw(&l, src, color);
		}
	}

	int m;
	for(m=2; m<4; m++){
		line_set2D(&l, x_coord[m], y_coord[m], x_coord[10], y_coord[10]);
		line_draw(&l, src, color);
		if(m+4 == 7){
			line_set2D(&l, x_coord[m+4], y_coord[m+4], x_coord[10], y_coord[10]);
			line_drawDash(&l, src, color);
		}else{
			line_set2D(&l, x_coord[m+4], y_coord[m+4], x_coord[10], y_coord[10]);
			line_draw(&l, src, color);
		}
	}

	int n;
	for(n=0; n<4; n+=3){
		line_set2D(&l, x_coord[n], y_coord[n], x_coord[11], y_coord[11]);
		line_draw(&l, src, color);
		if(n+4 == 7){
			line_set2D(&l, x_coord[n+4], y_coord[n+4], x_coord[11], y_coord[11]);
			line_drawDash(&l, src, color);
		}else{
			line_set2D(&l, x_coord[n+4], y_coord[n+4], x_coord[11], y_coord[11]);
			line_draw(&l, src, color);
		}
	}



	return 0;
}

int main(int argc, char *argv[]) {
	int rows = 500;
	int cols = 500;
	const int numPoints = 12;
	double x_coord[numPoints] = {200, 250, 250, 200, 220, 270, 270, 220, 235, 350, 235, 120};
	double y_coord[numPoints] = {200, 200, 250, 250, 180, 180, 230, 230, 100, 225, 340, 215`};
	Image* src = image_create(rows, cols);
	
	Color White;
	Color Yellow;
	Color Red;
	Color Blue;

	Color_set(&White, 1.0, 1.0, 1.0);
	Color_set(&Yellow, 1.0, 1.0, 0.0);
	Color_set(&Red, 1.0, 0.0, 0.0);
	Color_set(&Blue, 0.0, 0.0, 1.0);

	printf("Making the cube\n");
	shape(src, White, x_coord, y_coord);

	image_write( src, "../images/3Dmodel.ppm" );

  	image_free( src );


}
