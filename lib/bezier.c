/*
Erfan & Ryan
Date: 4 November 2014
File: bezier.c
*/

#include <stdio.h>
#include <stdlib.h>
#include "bezier.h"
#include "point.h"
#include "color.h"
#include "Image.h"
#include "line.h"

//sets the zbuffer flag to 1 and the curve points to the X-axis between 0 and 1.
void bezierCurve_init(BezierCurve *b){
	if(NULL != b){
		b->zBuffer = 1;
		int i;
		for(i=0; i<4; i++){
			b->ctrl[i].val[0] = 0.5;
		}
	}
}

//sets the zbuffer flag to 1 and the surface to the X-Z plane between (0, 0) and (1, 1).
void bezierSurface_init(BezierSurface *b){
	if(NULL != b){
		b->zBuffer = 1;
		int i, j;
		for(i=0; i<4; i++){
			for(j=0; j<4; j++){
				b->ctrl[i][j].val[0] = 0.5;
				b->ctrl[i][j].val[2] = 0.5;
			}
		}
	}
}

//sets the control points of the BezierCurve to the four points in the vlist array.
void bezierCurve_set(BezierCurve *b, Point *vlist){
	if((NULL != b) && (NULL != vlist)){
		int i;
		for(i=0; i<4; i++){
			point_copy(&b->ctrl[i], &vlist[i]);
		}
	}
}

//sets the control points of the BezierSurface to the 16 points in the vlist array.
void bezierSurface_set(BezierSurface *b, Point *vlist){
	if((NULL != b) && (NULL != vlist)){
		int i,j;
		for(i=0; i<4; i++){
			for(j=0; j<4; j++){
				point_copy(&b->ctrl[i][j], &vlist[(i*4)+j]);
			}
		}
	}
	b->zBuffer = 1;
}

//sets the z-buffer flag to the given value.
void bezierCurve_zBuffer(BezierCurve *p, int flag){
	if(NULL != p){
		p->zBuffer = flag;
	}
}

//sets the z-buffer flag to the given value.
void bezierSurface_zBuffer(BezierCurve *p, int flag){
	if(NULL != p){
		p->zBuffer = flag;
	}
}

/*
 draws the Bezier curve, given in screen coordinates, into the image using the given color. The
function should be adaptive so that it uses an appropriate number of line segments to draw the curve.
For example, if the bounding box of the control points is less than 10 pixels in the largest dimension,
then it is reasonable to draw the lines between the control points as an approximation to the curve
*/
void bezierCurve_draw(BezierCurve *b, Image *src, Color c){
	if((NULL !=  b) && (NULL != src)){
		double threshold = 10;
		double maxX = b->ctrl[0].val[0];
		double minX = b->ctrl[0].val[0];
		double maxY = b->ctrl[0].val[1];
		double minY = b->ctrl[0].val[1];
		int i;
		for(i=0; i<4; i++){
			if(b->ctrl[i].val[0] > maxX){
				maxX = b->ctrl[i].val[0];
			}
			if(b->ctrl[i].val[1] > maxY){
				maxY = b->ctrl[i].val[1];
			}
			if(b->ctrl[i].val[0] < minX){
				minX = b->ctrl[i].val[0];
			}
			if(b->ctrl[i].val[1] < maxY){
				minY = b->ctrl[i].val[1];
			}
		}
		if((maxX - minX < threshold) && (maxY - minY < threshold)){
			//draw
			Line l[3];
			line_set(&l[0], b->ctrl[0], b->ctrl[1]);
			line_set(&l[1], b->ctrl[1], b->ctrl[2]);
			line_set(&l[2], b->ctrl[2], b->ctrl[3]);

			line_draw(&l[0], src, c);
			line_draw(&l[1], src, c);
			line_draw(&l[2], src, c);
		}
		else{
			de_Casteljau(b, src, c);
		}
	}
}

// sub divides
void de_Casteljau(BezierCurve *b, Image *src, Color c){
	Point temp[6];
	average(&b->ctrl[0],&b->ctrl[1],&temp[0]);
	average(&b->ctrl[1],&b->ctrl[2],&temp[1]);
	average(&b->ctrl[2],&b->ctrl[3],&temp[2]);

	average(&temp[0],&temp[1],&temp[3]);
	average(&temp[1],&temp[2],&temp[4]);

	average(&temp[3],&temp[4], &temp[5]); //point on the curve

	BezierCurve curve1;
	BezierCurve curve2;

	curve1.zBuffer = 1;
	curve2.zBuffer = 1;

	point_copy(&curve1.ctrl[0], &b->ctrl[0]);
	point_copy(&curve1.ctrl[1], &temp[0]);
	point_copy(&curve1.ctrl[2], &temp[3]);
	point_copy(&curve1.ctrl[3], &temp[5]);

	point_copy(&curve2.ctrl[0], &temp[5]);
	point_copy(&curve2.ctrl[1], &temp[4]);
	point_copy(&curve2.ctrl[2], &temp[2]);
	point_copy(&curve2.ctrl[3], &b->ctrl[3]);

	bezierCurve_draw(&curve1, src, c);
	bezierCurve_draw(&curve2, src, c);
}

//to be use in the module_bezierCurve()
// input: BezierCurve pointer b which holds 4 control points
// input: BezierCurve pointer m and n
// 
//it sets m and n (zBuffer flag) and creates 8 points, 4 control points for m and 4points for n
void de_Casteljau_module(BezierCurve *b, BezierCurve *m, BezierCurve *n){
	Point temp[6];
	average(&b->ctrl[0],&b->ctrl[1],&temp[0]);
	average(&b->ctrl[1],&b->ctrl[2],&temp[1]);
	average(&b->ctrl[2],&b->ctrl[3],&temp[2]);

	average(&temp[0],&temp[1],&temp[3]);
	average(&temp[1],&temp[2],&temp[4]);

	average(&temp[3],&temp[4], &temp[5]); //point on the curve

	m->zBuffer = 1;
	n->zBuffer = 1;

	point_copy(&m->ctrl[0], &b->ctrl[0]);
	point_copy(&m->ctrl[1], &temp[0]);	
	point_copy(&m->ctrl[2], &temp[3]);
	point_copy(&m->ctrl[3], &temp[5]);

	point_copy(&n->ctrl[0], &temp[5]);
	point_copy(&n->ctrl[1], &temp[4]);
	point_copy(&n->ctrl[2], &temp[2]);
	point_copy(&n->ctrl[3], &b->ctrl[3]);
}

void average(Point* a, Point* b, Point* c){
	double avgX = (a->val[0] + b->val[0])/2;
	double avgY = (a->val[1] + b->val[1])/2;
	double avgZ = (a->val[2] + b->val[2])/2;
	point_set3D(c, avgX, avgY, avgZ);
}

