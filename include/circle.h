//
//  circle.h
//  
//
//  Created by Ryan Brandt on 9/22/14.
//
//

#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include "point.h"

typedef struct {
	double r;
	Point c;
}Circle;

void circle_set(Circle *c, Point tc, double tr);
void circle_draw(Circle *c, Image *src, Color p);
void circle_drawFill(Circle *c, Image *src, Color colr);
void circle_PlotPoints(Image* src, int xCenter, int yCenter, int x, int y, Color c);
void circle_PlotFillLines(Image* src, int xCenter, int yCenter, int x, int y, Color c);
void circle_drawDash(Circle *c, Image *src, Color colr);


#endif /* defined(__CIRCLE_H__) */
