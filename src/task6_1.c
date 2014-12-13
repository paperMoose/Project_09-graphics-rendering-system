/*
Erfan Azad
task6 image1
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "polygon.h"
#include "line.h"
#include "point.h"
#include "Image.h"
#include "color.h"


int main(int argc, char *argv[]){
	Image* src;
	int offset = 100;
	const int rows = 1000;
  	const int cols = 1000;
  	float r1, r2, r3, r4, r5, r6, r7, r8, r9;
  	int i;
	Color RED;
	Color GREEN;
	Color BLUE;
	Color WHITE;
	Color c1;
	Color c2;
	Color c3;
	Polygon* poly;
	Point points[3];
	char filename[100];
	
	Color_set(&RED, 1.0, 0.0, 0.0);
	Color_set(&GREEN, 0.0, 1.0, 0.0);
	Color_set(&BLUE, 0.0, 0.0, 1.0);
	Color_set(&WHITE, 1.0,1.0,1.0);

	for(i=0; i<offset+1; i++){
		src = image_create(rows, cols);

		//drawing the first triangle
		printf("Preparing and drawing triangle 0\n");
		point_set2D(&points[0], 300, (100 - offset + i));
		point_set2D(&points[1], 700, (100 - offset + i));
		point_set2D(&points[2], 500, (500 - offset + i));

		poly = polygon_createp(3, points);
		r1 = rand()/((double) RAND_MAX);
		r2 = rand()/((double) RAND_MAX);
		r3 = rand()/((double) RAND_MAX);
		r4 = rand()/((double) RAND_MAX);
		r5 = rand()/((double) RAND_MAX);
		r6 = rand()/((double) RAND_MAX);
		r7 = rand()/((double) RAND_MAX);
		r8 = rand()/((double) RAND_MAX);
		r9 = rand()/((double) RAND_MAX);

		Color_set(&c1, r1, r2, r3);
		Color_set(&c2, r4, r5, r6);
		Color_set(&c3, r7, r8, r9);

		polygon_drawFillB_Gradient(poly, src, c1, c2, c3);


		//drawing the second triangle
		printf("Preparing and drawing triangle 1\n");
		point_set2D(&points[0], 700+offset-i, 100-(offset-i)/2);
		point_set2D(&points[1], 900+offset-i, 500-(offset-i)/2);
		point_set2D(&points[2], 500+offset-i, 500-(offset-i)/2);
		

		poly = polygon_createp(3, points);
		r1 = rand()/((double) RAND_MAX);
		r2 = rand()/((double) RAND_MAX);
		r3 = rand()/((double) RAND_MAX);
		r4 = rand()/((double) RAND_MAX);
		r5 = rand()/((double) RAND_MAX);
		r6 = rand()/((double) RAND_MAX);
		r7 = rand()/((double) RAND_MAX);
		r8 = rand()/((double) RAND_MAX);
		r9 = rand()/((double) RAND_MAX);

		Color_set(&c1, r1, r2, r3);
		Color_set(&c2, r4, r5, r6);
		Color_set(&c3, r7, r8, r9);

		polygon_drawFillB_Gradient(poly, src, c1, c2, c3);


		//drawing the third triangle
		printf("Preparing and drawing triangle 2\n");
		point_set2D(&points[0], 900+offset-i, 500+(offset-i)/2);
		point_set2D(&points[1], 700+offset-i, 900+(offset-i)/2);
		point_set2D(&points[2], 500+offset-i, 500+(offset-i)/2);

		poly = polygon_createp(3, points);
		r1 = rand()/((double) RAND_MAX);
		r2 = rand()/((double) RAND_MAX);
		r3 = rand()/((double) RAND_MAX);
		r4 = rand()/((double) RAND_MAX);
		r5 = rand()/((double) RAND_MAX);
		r6 = rand()/((double) RAND_MAX);
		r7 = rand()/((double) RAND_MAX);
		r8 = rand()/((double) RAND_MAX);
		r9 = rand()/((double) RAND_MAX);

		Color_set(&c1, r1, r2, r3);
		Color_set(&c2, r4, r5, r6);
		Color_set(&c3, r7, r8, r9);

		polygon_drawFillB_Gradient(poly, src, c1, c2, c3);


		//drawing the forth triangle
		printf("Preparing and drawing triangle 3\n");
		point_set2D(&points[0], 300, 900 + offset - i);
		point_set2D(&points[1], 700, 900 + offset - i);
		point_set2D(&points[2], 500, 497 + offset - i);

		poly = polygon_createp(3, points);
		r1 = rand()/((double) RAND_MAX);
		r2 = rand()/((double) RAND_MAX);
		r3 = rand()/((double) RAND_MAX);
		r4 = rand()/((double) RAND_MAX);
		r5 = rand()/((double) RAND_MAX);
		r6 = rand()/((double) RAND_MAX);
		r7 = rand()/((double) RAND_MAX);
		r8 = rand()/((double) RAND_MAX);
		r9 = rand()/((double) RAND_MAX);

		Color_set(&c1, r1, r2, r3);
		Color_set(&c2, r4, r5, r6);
		Color_set(&c3, r7, r8, r9);

		polygon_drawFillB_Gradient(poly, src, c1, c2, c3);


		//drawing the fifth triangle
		printf("Preparing and drawing triangle 4\n");
		point_set2D(&points[0], 100-offset+i, 500+(offset-i)/2);
		point_set2D(&points[1], 300-offset+i, 900+(offset-i)/2);
		point_set2D(&points[2], 500-offset+i, 500+(offset-i)/2);

		poly = polygon_createp(3, points);
		r1 = rand()/((double) RAND_MAX);
		r2 = rand()/((double) RAND_MAX);
		r3 = rand()/((double) RAND_MAX);
		r4 = rand()/((double) RAND_MAX);
		r5 = rand()/((double) RAND_MAX);
		r6 = rand()/((double) RAND_MAX);
		r7 = rand()/((double) RAND_MAX);
		r8 = rand()/((double) RAND_MAX);
		r9 = rand()/((double) RAND_MAX);

		Color_set(&c1, r1, r2, r3);
		Color_set(&c2, r4, r5, r6);
		Color_set(&c3, r7, r8, r9);

		polygon_drawFillB_Gradient(poly, src, c1, c2, c3);



	//drawing the sixth triangle
		printf("Preparing and drawing triangle 5\n");
		point_set2D(&points[0], 100-offset+i, 500-(offset-i)/2);
		point_set2D(&points[1], 301-offset+i, 100-(offset-i)/2);
		point_set2D(&points[2], 503-offset+i, 500-(offset-i)/2);

		poly = polygon_createp(3, points);
		r1 = rand()/((double) RAND_MAX);
		r2 = rand()/((double) RAND_MAX);
		r3 = rand()/((double) RAND_MAX);
		r4 = rand()/((double) RAND_MAX);
		r5 = rand()/((double) RAND_MAX);
		r6 = rand()/((double) RAND_MAX);
		r7 = rand()/((double) RAND_MAX);
		r8 = rand()/((double) RAND_MAX);
		r9 = rand()/((double) RAND_MAX);

		Color_set(&c1, r1, r2, r3);
		Color_set(&c2, r4, r5, r6);
		Color_set(&c3, r7, r8, r9);

		polygon_drawFillB_Gradient(poly, src, c1, c2, c3);

		if(i%5==0){
			sprintf(filename, "../images/task6_2_%3.0d.ppm",i);
			image_write(src, filename);
		}
		image_dealloc(src);

	}	
	system("convert -delay 10 -loop 0   ../images/task6_2_*.ppm   ../images/animation.gif");
	system("rm -f ../images/task6_2*");

	//clean up

	polygon_free(poly);
  	image_free(src);


	return 0;
}