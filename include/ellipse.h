/*
Erfan Azad
Date: 24 Sep 2014
ellipse.h
*/
#ifndef __ELLIPSE_H__
#define __ELLIPSE_H__

#include "point.h"
#include "Image.h"
#include "color.h"

typedef struct{
	double ra; //major axis radius
	double rb; //minor axis radius
	Point c;    //center point
	//double a – (optional) angle of major axis relative to the X-axis
}Ellipse;

void ellipse_set(Ellipse *e, Point tc, double ta, double tb);
void ellipse_draw(Ellipse *e, Image *src, Color colr);
void ellipse_drawDash(Ellipse *e, Image *src, Color colr);
void ellipse_drawFill(Ellipse *e, Image *src, Color p);
void ellipse_plotPoints(Image* src, double xCenter, double yCenter, double x, double y, Color colr);
void ellipse_plotFillLines(Image* src, double xCenter, double yCenter, double x, double y, Color colr);

#endif