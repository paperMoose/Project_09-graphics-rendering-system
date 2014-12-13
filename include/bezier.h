/*
Erfan & Ryan
Date: 4 November 2014
File: bezier.h
*/

#ifndef __BEZIER_H__
#define __BEZIER_H__

#include "point.h"
#include "color.h"
#include "Image.h"

typedef struct{
	Point ctrl[4];
	int zBuffer;
}BezierCurve;

typedef struct{
	Point ctrl[4][4];
	int zBuffer;
}BezierSurface;

void bezierCurve_init(BezierCurve *b);
void bezierSurface_init(BezierSurface *b);
void bezierCurve_set(BezierCurve *b, Point *vlist);
void bezierSurface_set(BezierSurface *b, Point *vlist);
void bezierCurve_zBuffer(BezierCurve *p, int flag);
void bezierSurface_zBuffer(BezierCurve *p, int flag);
void bezierCurve_draw(BezierCurve *b, Image *src, Color c);

//auxilary functions
void de_Casteljau(BezierCurve *b, Image *src, Color c);
void de_Casteljau_module(BezierCurve *b, BezierCurve *m, BezierCurve *n);
void average(Point* a, Point* b, Point* c);


#endif