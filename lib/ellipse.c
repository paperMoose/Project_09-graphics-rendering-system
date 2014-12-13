/*
Erfan Azad
Date: 24 Sep 2014
ellipse.c
This file contains method definition for the ellipse.h header file
*/

/*
initialize an ellipse to location tc and radii ta and tb.
tc is the midpoint
ta is major axis radius
tb is minor axis radius
*/
#include <stdio.h>
#include <stdlib.h>
#include "ellipse.h"
#include "point.h"
#include "color.h"
#include "Image.h"
#include "line.h"


void ellipse_set(Ellipse *e, Point tc, double ta, double tb){
	if(NULL == e){
		printf("ERROR: ellipse_set >> Ellipse pointer is NULL.\n");
		exit(-1);
	}
	e->ra = ta;
	e->rb = tb;
	point_copy(&e->c, &tc);
}

/*
 draw ellipse into src using color p
*/
void ellipse_draw(Ellipse *e, Image *src, Color colr){
	if(NULL == e){
		printf("ERROR: ellipse_set >> Ellipse pointer is NULL.\n");
		exit(-1);
	}
	double cx = e->c.val[0];
	double cy = e->c.val[1];
	double Rx = e->ra;
	double Ry = e->rb;

	double x = 0;
	double y = -Ry;
	double px = 2 * Ry * Ry;
	double py = 2 * Rx * Rx * -y;
	double p;

	ellipse_plotPoints(src,cx,cy, x, y, colr);

	p = Ry*Ry - Rx*Rx * Ry + (Rx*Rx)/4 + Ry*Ry + px;

	while (px < py){
		x--;
		px = px + 2 * Ry * Ry;
		if (p < 0){
			p = p + Ry*Ry + px;
		}else{
			y++;
			py = py - 2 * Rx * Rx;
			p = p + Ry * Ry + px - py;
		}
		ellipse_plotPoints(src,cx, cy, x, y, colr);
	}

	p = Ry*Ry * (x*x + x) + Rx*Rx * (y*y -2*y + 1) -Rx*Rx * Ry*Ry+ Rx*Rx - py;

	while (y < 0){
		y++;
		py = py - 2 * Rx * Rx;
		if (p > 0){
			p = p + Rx * Rx - py;
		}else{
			x--;
			px = px + 2 * Ry * Ry;
			p = p + Rx * Rx - py + px;
		}
		ellipse_plotPoints(src, cx, cy, x, y, colr);
	}
}

void ellipse_drawDash(Ellipse *e, Image *src, Color colr){
	if(NULL == e){
		printf("ERROR: ellipse_set >> Ellipse pointer is NULL.\n");
		exit(-1);
	}
	double cx = e->c.val[0];
	double cy = e->c.val[1];
	double Rx = e->ra;
	double Ry = e->rb;

	double x = 0;
	double y = -Ry;
	double px = 2 * Ry * Ry;
	double py = 2 * Rx * Rx * -y;
	double p;

	ellipse_plotPoints(src,cx,cy, x, y, colr);

	p = Ry*Ry - Rx*Rx * Ry + (Rx*Rx)/4 + Ry*Ry + px;

	while (px < py){
		x--;
		px = px + 2 * Ry * Ry;
		if (p < 0){
			p = p + Ry*Ry + px;
		}else{
			y++;
			py = py - 2 * Rx * Rx;
			p = p + Ry * Ry + px - py;
		}
		if((int)(x)%4 == 0){
			ellipse_plotPoints(src,cx, cy, x, y, colr);
		}
	}

	p = Ry*Ry * (x*x + x) + Rx*Rx * (y*y -2*y + 1) -Rx*Rx * Ry*Ry+ Rx*Rx - py;

	while (y < 0){
		y++;
		py = py - 2 * Rx * Rx;
		if (p > 0){
			p = p + Rx * Rx - py;
		}else{
			x--;
			px = px + 2 * Ry * Ry;
			p = p + Rx * Rx - py + px;
		}
		if((int)(y)%4 == 0){
			ellipse_plotPoints(src, cx, cy, x, y, colr);
		}
	}
}

/*
 draw a filled ellipse into src using color p
*/
void ellipse_drawFill(Ellipse *e, Image *src, Color colr){
	if(NULL == e){
		printf("ERROR: ellipse_set >> Ellipse pointer is NULL.\n");
		exit(-1);
	}
	double cx = e->c.val[0];
	double cy = e->c.val[1];
	double Rx = e->ra;
	double Ry = e->rb;

	double x = 0;
	double y = -Ry;
	double px = 2 * Ry * Ry;
	double py = 2 * Rx * Rx * -y;
	double p;

	ellipse_plotFillLines(src,cx, cy, x, y, colr);

	p = Ry*Ry - Rx*Rx * Ry + (Rx*Rx)/4 + Ry*Ry + px;

	while (px < py){
		x--;
		px = px + 2 * Ry * Ry;
		if (p < 0){
			p = p + Ry*Ry + px;
		}else{
			y++;
			py = py - 2 * Rx * Rx;
			p = p + Ry * Ry + px - py;
		}
		ellipse_plotFillLines(src,cx, cy, x, y, colr);
	}

	p = Ry*Ry * (x*x + x) + Rx*Rx * (y*y -2*y + 1) -Rx*Rx * Ry*Ry+ Rx*Rx - py;

	while (y < 0){
		y++;
		py = py - 2 * Rx * Rx;
		if (p > 0){
			p = p + Rx * Rx - py;
		}else{
			x--;
			px = px + 2 * Ry * Ry;
			p = p + Rx * Rx - py + px;
		}
		ellipse_plotFillLines(src, cx, cy, x, y, colr);
	}
}

/*
This is an auxilary function that is used in the ellipse_draw function.
plots four dots, each in one quadrant,  given a center point and x and y radius.
*/
void ellipse_plotPoints(Image* src, double yCenter, double xCenter, double y, double x, Color colr){
	if(NULL == src){
		printf("ERROR: ellipse_set >> Ellipse pointer is NULL.\n");
		exit(-1);
	}
	Point p;
	point_set2D(&p, xCenter+x, yCenter+y);
	point_draw(&p,src,colr);

	point_set2D(&p, xCenter-x ,yCenter+y);
	point_draw(&p,src,colr);

	point_set2D(&p, xCenter+x, yCenter-y);
	point_draw(&p,src,colr);

	point_set2D(&p, xCenter-x, yCenter-y);
	point_draw(&p,src,colr); 
}

/*
This is an auxilary function to draw lines for filling the ellipse.
it is used in ellipse_drawFill() function.
*/
void ellipse_plotFillLines(Image* src, double xCenter, double yCenter, double x, double y, Color colr){
	Line l;
	line_set2D(&l, xCenter+x, yCenter+y, xCenter-x, yCenter+y);
	line_draw(&l, src, colr);

	line_set2D(&l, xCenter+x, yCenter-y, xCenter-x, yCenter-y);
	line_draw(&l, src, colr);
}




